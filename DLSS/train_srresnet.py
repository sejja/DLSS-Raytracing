import time
import torch.backends.cudnn as cudnn
import torch
from torch import nn
from models import SRResNet
from dataset import SRDataset
from utils import *


# Data parameters
data_dir = "./data/COCO/" # folder with JSON data files
crop_size = 96          # crop size of target HR images
scaling_factor = 4      # the scaling factor for the generator; the input LR images will be downsampled from the target HR images by this factor

# Model parameters
large_kernel_size = 9   # kernel size of the first and last convolutions which transform the inputs and outputs
small_kernel_size = 3   # kernel size of all convolutions in-between, i.e. those in the residual and subpixel convolutional blocks
n_channels = 64         # number of channels in-between, i.e. the input and output channels for the residual and subpixel convolutional blocks
n_blocks = 16           # number of residual blocks

# Learning parameters
checkpoint = None       # path to model checkpoint, None if none
batch_size = 16         # batch size
start_epoch = 0         # start at this epoch
iterations = 1e5        # number of training iterations
workers = 8             # number of workers for loading data in the DataLoader
print_freq = 500        # print training status once every __ batches
lr = 1e-4               # learning rate
grad_clip = None        # clip if gradients are exploding

# Dafault device
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using {device} for SRResNet training")

cudnn.benchmark = True


def main():
    """
    Training.
    """

    global start_epoch, epoch, checkpoint

    # Initialize model or load checkpoint
    if checkpoint is None:

        model = SRResNet(large_kernel_size=large_kernel_size,
                         small_kernel_size=small_kernel_size,
                         n_channels=n_channels,
                         n_blocks=n_blocks,
                         scaling_factor=scaling_factor)
        
        # Initialize the optimizer
        optimizer = torch.optim.Adam(params=filter(lambda p: p.requires_grad, model.parameters()), lr=lr)
    
    else:

        checkpoint = torch.load(checkpoint)
        start_epoch = checkpoint["epoch"] + 1
        model = checkpoint["model"]
        optimizer = checkpoint["optimizer"]

    # Move to default device
    model = model.to(device)
    criterion = nn.MSELoss().to(device)

    # Custom dataloaders
    train_dataset = SRDataset(data_dir,
                              split="train",
                              crop_size=crop_size,
                              scaling_factor=scaling_factor,
                              lr_img_type="imagenet-norm",
                              hr_img_type="[-1, 1]")
    train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=workers, pin_memory=True)

    # Total number of epochs to train for
    epochs = int(iterations // len(train_loader) + 1)

    # Epoch
    for epoch in range(start_epoch, epochs):

        # One epochs training
        train(train_loader=train_loader,
              model=model,
              criterion=criterion,
              optimizer=optimizer,
              epoch=epoch)
        
        # Save checkpoint
        torch.save({
            "epoch": epoch,
            "model": model,
            "optimizer": optimizer
        }, "./models/checkpoint_srresnet.pth.tar")

def train(train_loader, model, criterion, optimizer, epoch):
    """
    One epoch's training.

    Args:
        train_loader: DataLoader for training data
        model: model
        criterion: content loss function (Mean Squared-Error loss)
        optimizer: optimizer
        epoch: epoch number
    """

    # Set model into train mode
    model.train()

    batch_time = AverageMeter() # forward prop. + back prop. time
    data_time = AverageMeter()  # data loading time
    losses = AverageMeter()     # loss

    start = time.time()

    # Batches
    for i, (lr_imgs, hr_imgs) in enumerate(train_loader):

        # Keep track of data loading times
        data_time.update(time.time() - start)

        # Move batches to default device
        lr_imgs = lr_imgs.to(device)    # (batch_size (N), 3, 24, 24), imagenet-normed
        hr_imgs = hr_imgs.to(device)    # (batch_size (N), 3, 96, 96), in [-1, 1]

        # Forward-Prop
        sr_imgs = model(lr_imgs)    # (N, 3, 96, 96), in [-1, 1]

        # Loss
        loss = criterion(sr_imgs, hr_imgs)

        # Backward-Prop
        optimizer.zero_grad()
        loss.backward()

        # Clip gradients
        if grad_clip is not None:
            clip_gradient(optimizer, grad_clip)
        
        # Update model
        optimizer.step()

        # Keep track of loss
        losses.update(loss.item(), lr_imgs.size(0))

        # Keep track of batch time
        batch_time.update(time.time() - start)

        # Reset start time
        start = time.time()

        # Print status
        if i % print_freq == 0:
            print(f"Epoch[{epoch}][{i}/{len(train_loader)}]----"
                  f"Batch Time {batch_time.val:.3f} ({batch_time.avg:.3f})----"
                  f"Data Time {data_time.val:.3f} ({data_time.avg:.3f})----"
                  f"Loss {losses.val:.4f} ({losses.avg:.4f})")
        
        # free some memory since their histories may be stored
        del lr_imgs, hr_imgs, sr_imgs


if __name__ == "__main__":
    main()

