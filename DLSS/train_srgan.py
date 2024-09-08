import time
import torch.backends.cudnn as cudnn
from torch import nn
import torch.utils
from models import Generator, Discriminator, TruncatedVGG19
from dataset import SRDataset
from utils import *

# Data parameters
data_dir = "./data/COCO/" # Directory with json files
crop_size = 96          # Crop size of target HR images
scaling_factor = 4      # The scaling factor for the generator

# Generator parameters
large_kernel_size_g = 9 # kernel size of the first and last convolutions which transform the inputs and outputs
small_kernel_size_g = 3 # kernel size of all convolutions in-between, i.e. those in the residual and subpixel convolutional blocks
n_channels_g = 64       # number of channels in-between, i.e. the input and output channels for the residual and subpixel convolutional blocks
n_blocks_g = 16         # number of residual blocks
srresnet_checkpoint = "./models/checkpoint_srresnet.pth.tar"    # filepath of the trained SRResNet checkpoint used for initialization

# Discriminator parameters
kernel_size_d = 3       # kernel size in all convolutional blocks
n_channels_d = 64       # number of output channels in the first convolutional block, after which it is doubled in every 2nd block thereafter
n_blocks_d = 8          # number of convolutional blocks
fc_size_d = 1024        # size of the first fully connected layer

# Learning parameters
checkpoint = "./models/checkpoint_srgan.pth.tar" # path to model (SRGAN) checkpoint, None if none
batch_size = 16         # batch size
start_epoch = 0         # start at this epoch
iterations = 1e5        # number of training iterations
workers = 8             # number of workers for loading data in the DataLoader
vgg19_i = 5             # the index i in the definition for VGG loss; see paper or models.py
vgg19_j = 4             # the index j in the definition for VGG loss; see paper or models.py
beta = 1e-3             # the coefficient to weight the adversarial loss in the perceptual loss
print_freq = 500        # print training status once every __ batches
lr = 1e-4               # learning rate
grad_clip = None        # clip if gradients are exploding

# Default device
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using {device} for SRGAN training")

cudnn.benchmark = True


def main():
    """
    Training.
    """

    global start_epoch, epoch, checkpoint, srresnet_checkpoint

    # Initialize model or load checkpoint
    if checkpoint is None:

        # Generator
        generator = Generator(large_kernel_size=large_kernel_size_g,
                              small_kernel_size=small_kernel_size_g,
                              n_channels=n_channels_g,
                              n_blocks=n_blocks_g,
                              scaling_factor=scaling_factor)
        
        # Initialize generator network with pretrained SRResNet
        generator.initialize_with_srresnet(srresnet_checkpoint=srresnet_checkpoint)

        # Initialize generator's optimizer
        optimizer_g = torch.optim.Adam(params=filter(lambda p: p.requires_grad, generator.parameters()), lr=lr)

        # Discriminator
        discriminator = Discriminator(kernel_size=kernel_size_d,
                                      n_channels=n_channels_d,
                                      n_blocks=n_blocks_d,
                                      fc_size=fc_size_d)
        
        # Initialize discriminator's optimizer
        optimizer_d = torch.optim.Adam(params=filter(lambda p: p.requires_grad, discriminator.parameters()), lr=lr)
    
    else:
        
        checkpoint = torch.load(checkpoint)
        start_epoch = checkpoint["epoch"] + 1
        generator = checkpoint["generator"]
        discriminator = checkpoint["discriminator"]
        optimizer_g = checkpoint["optimizer_g"]
        optimizer_d = checkpoint["optimizer_d"]

    # Truncated VGG19 network to be used in the loss calculation
    truncated_vgg19 = TruncatedVGG19(i=vgg19_i, j=vgg19_j)
    truncated_vgg19.eval()

    # Loss functions
    content_loss_criterion = nn.MSELoss()
    adversarial_loss_criterion = nn.BCEWithLogitsLoss()

    # Move to default device
    generator = generator.to(device)
    discriminator = discriminator.to(device)
    truncated_vgg19 = truncated_vgg19.to(device)
    content_loss_criterion = content_loss_criterion.to(device)
    adversarial_loss_criterion = adversarial_loss_criterion.to(device)

    # Custom dataloaders
    train_dataset = SRDataset(data_dir,
                              split="train",
                              crop_size=crop_size,
                              scaling_factor=scaling_factor,
                              lr_img_type="imagenet-norm",
                              hr_img_type="imagenet-norm")
    train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=workers, pin_memory=True)

    # Total number of epohs to train for
    epochs = int(iterations // len(train_loader) + 1)

    # Epochs
    for epoch in range(start_epoch, epochs):

        # At the halfway point, reduce learning rate to a tenth
        if epoch == int((iterations / 2) // len(train_loader) + 1):
            adjust_learning_rate(optimizer_g, 0.1)
            adjust_learning_rate(optimizer_d, 0.1)

        # One epoch's training
        train(train_loader=train_loader,
              generator=generator,
              discriminator=discriminator,
              truncated_vgg19=truncated_vgg19,
              content_loss_criterion=content_loss_criterion,
              adversarial_loss_criterion=adversarial_loss_criterion,
              optimizer_g=optimizer_g,
              optimizer_d=optimizer_d,
              epoch=epoch)
        
        # Save checkpoint
        torch.save({
            "epoch": epoch,
            "generator": generator,
            "discriminator": discriminator,
            "optimizer_g": optimizer_g,
            "optimizer_d": optimizer_d
        }, "./models/checkpoint_srgan.pth.tar")


def train(train_loader, generator, discriminator, truncated_vgg19, content_loss_criterion, adversarial_loss_criterion, optimizer_g, optimizer_d, epoch):
    """
    One epoch's training.

    Args:
        train_loader: train dataloader
        generator: generator
        discriminator: discriminator
        truncated_vgg19: truncated VGG19 network
        content_loss_criterion: content loss function (Mean Squared-Error loss)
        adversarial_loss_criterion: adversarial loss function (Binary Cross-Entropy loss)
        optimizer_g: optimizer for the generator
        optimizer_d: optimizer for the discriminator
        epoch: epoch number
    """

    # Set models into train mode
    generator.train()
    discriminator.train()

    batch_time = AverageMeter() # forward prop. + back prop. time
    data_time = AverageMeter()  # data loading time
    losses_c = AverageMeter()   # content loss
    losses_a = AverageMeter()   # adversarial loss in the generator
    losses_d = AverageMeter()   # adversarial loss in the discriminator

    start = time.time()

    # Batches
    for i, (lr_imgs, hr_imgs) in enumerate(train_loader):
        
        # Keep track of data loading times
        data_time.update(time.time() - start)

        # Move batches to default device
        lr_imgs = lr_imgs.to(device)    # (batch_size (N), 3, 24, 24), imagenet-normed
        hr_imgs = hr_imgs.to(device)    # (batch_size (N), 3, 96, 96), imagenet-normed

        ##########################
        #    GENERATOR UPDATE    #
        ##########################

        # Generate
        sr_imgs = generator(lr_imgs)                                                # (N, 3, 96, 96), in [-1, 1]
        sr_imgs = convert_image(sr_imgs, source='[-1, 1]', target='imagenet-norm')  # (N, 3, 96, 96), imagenet-norme

        # Calculate VGG feature maps for the super-resolved (SR) and high-resolution (HR) images
        sr_imgs_in_vgg_space = truncated_vgg19(sr_imgs)
        hr_imgs_in_vgg_space = truncated_vgg19(hr_imgs).detach()    # detached because they're constant, targets

        # Discriminate super-resolved (SR) images
        sr_discriminated = discriminator(sr_imgs)   # N

        # Calculate the perceptual loss
        content_loss = content_loss_criterion(sr_imgs_in_vgg_space, hr_imgs_in_vgg_space)
        adversarial_loss = adversarial_loss_criterion(sr_discriminated, torch.ones_like(sr_discriminated))
        perceptual_loss = content_loss + beta * adversarial_loss

        # Back-Prop
        optimizer_g.zero_grad()
        perceptual_loss.backward()

        # Clip gradients
        if grad_clip is not None:
            clip_gradient(optimizer_g, grad_clip)

        # Update generator
        optimizer_g.step()


        # Keep track of losses
        losses_c.update(content_loss.item(), lr_imgs.size(0))
        losses_a.update(adversarial_loss.item(), lr_imgs.size(0))

        ##############################
        #    DISCRIMINATOR UPDATE    #
        ##############################

        # Discriminate super-resolution (SR) and high-resolution (HR) images
        hr_discriminated = discriminator(hr_imgs)
        sr_discriminated = discriminator(sr_imgs.detach())

        # Binary Cross-Entropy loss
        adversarial_loss = adversarial_loss_criterion(sr_discriminated, torch.zeros_like(sr_discriminated)) + \
                            adversarial_loss_criterion(hr_discriminated, torch.ones_like(hr_discriminated))

        # Back-prop
        optimizer_d.zero_grad()
        adversarial_loss.backward()

        # Clip gradients
        if grad_clip is not None:
            clip_gradient(optimizer_d, grad_clip)

        # Update discriminator
        optimizer_d.step()

        # Keep track of losses
        losses_d.update(adversarial_loss.item(), hr_imgs.size(0))

        # Keep track of batch times
        batch_time.update(time.time() - start)

        # Reset start time
        start = time.time()

        # Print status
        if i % print_freq == 0:
            print(f"Epoch[{epoch}][{i}/{len(train_loader)}]----"
                  f"Batch Time {batch_time.val:.3f} ({batch_time.avg:.3f})----"
                  f"Data Time {data_time.val:.3f} ({data_time.avg:.3f})----"
                  f"Cont. Loss {losses_c.val:.4f} ({losses_c.avg:.4f})----"
                  f"Adv. Loss {losses_a.val:.4f} ({losses_a.avg:.4f})----"
                  f"Disc. Loss {losses_d.val:.4f} ({losses_d.avg:.4f})")
        
        # free some memory since their histories may be stored
        del lr_imgs, hr_imgs, sr_imgs, hr_imgs_in_vgg_space, sr_imgs_in_vgg_space, hr_discriminated, sr_discriminated


if __name__ == "__main__":
    main()

