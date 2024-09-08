import torch
from torch import nn
import torchvision
import math


class ConvBlock(nn.Module):
    """
    A convolutional block, comprising convolutional, BN, activation layers.
    """

    def __init__(self, in_channels, out_channels, kernel_size, stride=1, batch_norm=False, activation=None):
        """
        Args:
            in_channels: number of input channels
            out_channels: number of output channe;s
            kernel_size: kernel size
            stride: stride
            batch_norm: include a BN layer?
            activation: Type of activation; None if none
        """

        super(ConvBlock, self).__init__()

        if activation is not None:
            activation = activation.lower()
            assert activation in {"prelu", "leakyrelu", "tanh"}
        
        # A container that will hold the layers in this convolutional block
        layers = list()

        # A convolutional layer
        layers.append(nn.Conv2d(in_channels=in_channels,
                                out_channels=out_channels,
                                kernel_size=kernel_size,
                                stride=stride,
                                padding=(kernel_size // 2)))
        
        # A batch normalization layer
        if batch_norm is True:
            layers.append(nn.BatchNorm2d(num_features=out_channels))

        # An activation layer
        if activation == "prelu":
            layers.append(nn.PReLU())
        elif activation == "leakyrelu":
            layers.append(nn.LeakyReLU())
        elif activation == "tanh":
            layers.append(nn.Tanh())

        # Put together the convolutional block as a sequence of layers in this container
        self.conv_block = nn.Sequential(*layers)

    def forward(self, input):
        """
        Forward propagation.

        Args:
            input: input images, a tensor of size (N, in_channels, w, h)
        Returns:
            output images, a tensor of size (N, out_channels, w, h)
        """

        output = self.conv_block(input)

        return output
        

class PixelShuffleBlock(nn.Module):
    """
    A subpixel convolutional block, comprising convolutional, pixel-shuffle, and PReLU activation layers.
    """

    def __init__(self, kernel_size=3, n_channels=64, scaling_factor=2):
        """
        Args:
            kernel_size: kernel size of the convolution
            n_channels: number of input and output channels
            scaling_factor: factor to scale input images by (along both dimensions)
        """

        super(PixelShuffleBlock, self).__init__()

        # A convolutional layer that increases the number of channels by scaling factor^2, followed by pixel shuffle and PReLU
        self.conv = nn.Conv2d(in_channels=n_channels,
                              out_channels=(n_channels * (scaling_factor ** 2)),
                              kernel_size=kernel_size,
                              padding=kernel_size // 2)
        
        # This additional channels are shufled to form additional pixels, upscaling each dimension bt the scaling factor
        self.pixel_shuffle = nn.PixelShuffle(upscale_factor=scaling_factor)
        self.prelu = nn.PReLU()

    def forward(self, input):
        """
        Forward propagation.

        Args:
            input: input images, a tensor of size (N, n_channels, w, h)
        Returns:
            scaled output images, a tensor of size (N, n_channels, w * scaling factor, h * scaling factor)
        """

        output = self.conv(input)           # (N, n_channels * scaling factor^2, w, h)
        output = self.pixel_shuffle(output) # (N, n_channels, w * scaling factor, h * scaling factor)
        output = self.prelu(output)         # (N, n_channels, w * scaling factor, h * scaling factor)

        return output
    

class ResidualBlock(nn.Module):
    """
    A residual block, comprising two convolutional blocks with a residual connection across them.
    """

    def __init__(self, kernel_size=3, n_channels=64):
        """
        Args:
            kernel_size: kernel size
            n_channels: number of input and output channels (same because the input must be added to the output)
        """

        super(ResidualBlock, self).__init__()

        # The first convolutional block
        self.conv_block1 = ConvBlock(in_channels=n_channels,
                                     out_channels=n_channels,
                                     kernel_size=kernel_size,
                                     batch_norm=True,
                                     activation="PReLU")
        
        # The second convolutional block
        self.conv_block2 = ConvBlock(in_channels=n_channels,
                                     out_channels=n_channels,
                                     kernel_size=kernel_size,
                                     batch_norm=True,
                                     activation=None)
    
    def forward(self, input):
        """
        Forward propagation.

        Args:
            input: input images, a tensor of size (N, n_channels, w, h)
        Returns:
            output images, a tensor of size (N, n_channels, w, h)
        """

        output = self.conv_block1(input)    # (N, n_channels, w, h)
        output = self.conv_block2(output)   # (N, n_channels, w, h)
        output = output + input             # (N, n_channels, w, h)

        return output


class SRResNet(nn.Module):
    """
    The SRResNet, as defined in the paper.
    """

    def __init__(self, large_kernel_size=9, small_kernel_size=3, n_channels=64, n_blocks=16, scaling_factor=4):
        """
        Args:
            large_kernel_size: kernel size of the first and last convolutions which transform the inputs and outputs
            small_kernel_size: kernel size of all convolutions in-between, i.e. those in the residual and subpixel convolutional blocks
            n_channels: number of channels in-between, i.e. the input and output channels for the residual and subpixel convolutional blocks
            n_blocks: number of residual blocks
            scaling_factor: factor to scale input images by (along both dimensions) in the subpixel convolutional block
        """

        super(SRResNet, self).__init__()

        # Scaling factor must be 2, 4 or 8
        scaling_factor = int(scaling_factor)
        assert scaling_factor in {2, 4, 8}, "The scaling factor can only be 2, 4 or 8!"

        # The first convolutional block
        self.conv_block1 = ConvBlock(in_channels=3,
                                     out_channels=n_channels,
                                     kernel_size=large_kernel_size,
                                     batch_norm=False,
                                     activation="PReLU")
        
        # A sequence of n_blocks residual blocks, each containing a skip-connection across the block
        self.residual_blocks = nn.Sequential(
            *[ResidualBlock(kernel_size=small_kernel_size, n_channels=n_channels) for _ in range(n_blocks)]
        )

        # Another convolutional block
        self.conv_block2 = ConvBlock(in_channels=n_channels,
                                     out_channels=n_channels,
                                     kernel_size=small_kernel_size,
                                     batch_norm=True,
                                     activation=None)
        
        # Upscaling is done by sub-pixel convolution, with each such block upscaling by a factor of 2
        n_pixel_suffle_blocks = int(math.log2(scaling_factor))
        self.pixel_suffle_blocks = nn.Sequential(
            *[PixelShuffleBlock(kernel_size=small_kernel_size, n_channels=n_channels, scaling_factor=2) for _ in range(n_pixel_suffle_blocks)]
        )

        # The last convolutional block
        self.conv_block3 = ConvBlock(in_channels=n_channels, out_channels=3, kernel_size=large_kernel_size, batch_norm=False, activation="Tanh")

    def forward(self, lr_images):
        """
        Forward prop.

        Args:
            lr_images: low-resolution input images, a tensor of size (N, 3, w, h)
        Returns:
            super-resolution output images, a tensor of size (N, 3, w * scaling factor, h * scaling factor)
        """

        output = self.conv_block1(lr_images)        # (N, 3, w, h)
        residual = output                           # (N, n_channels, w, h)
        output = self.residual_blocks(output)       # (N, n_channels, w, h)
        output = self.conv_block2(output)           # (N, n_channels, w, h)
        output = output + residual                  # (N, n_channels, w, h)
        output = self.pixel_suffle_blocks(output)   # (N, n_channels, w * scaling factor, h * scaling factor)
        sr_images = self.conv_block3(output)        # (N, 3, w * scaling factor, h * scaling factor)

        return sr_images


class Generator(nn.Module):
    """
    The generator in the SRGAN, as defined in the paper. Architecture identical to the SRResNet.
    """

    def __init__(self, large_kernel_size=9, small_kernel_size=3, n_channels=64, n_blocks=16, scaling_factor=4):
        """
        Args:
            large_kernel_size: kernel size of the first and last convolutions which transform the inputs and outputs
            small_kernel_size: kernel size of all convolutions in-between, i.e. those in the residual and subpixel convolutional blocks
            n_channels: number of channels in-between, i.e. the input and output channels for the residual and subpixel convolutional blocks
            n_blocks: number of residual blocks
            scaling_factor: factor to scale input images by (along both dimensions) in the subpixel convolutional block
        """

        super(Generator, self).__init__()

        # The generator is simply a SRResNet
        self.net = SRResNet(large_kernel_size=large_kernel_size,
                            small_kernel_size=small_kernel_size,
                            n_channels=n_channels,
                            n_blocks=n_blocks,
                            scaling_factor=scaling_factor)
    
    def initialize_with_srresnet(self, srresnet_checkpoint):
        """
        Initialize with weights from a trained SRResNet.
        
        Args:
            srresnet_checkpoint: checkpoint filepath
        """

        srresnet = torch.load(srresnet_checkpoint)["model"]
        self.net.load_state_dict(srresnet.state_dict())

    def forward(self, lr_imgs):
        """
        Forward prop.

        Args:
            lr_imgs: low-resolution input images, a tensor of size (N, 3, w, h)
        Returns:
            super-resolution output images, a tensor of size (N, 3, w * scaling factor, h * scaling factor)
        """

        sr_imgs = self.net(lr_imgs)

        return sr_imgs
    

class Discriminator(nn.Module):
    """
    The discriminator in the SRGAN, as defined in the paper.
    """

    def __init__(self, kernel_size=3, n_channels=64, n_blocks=8, fc_size=1024):
        """
        Args:
            kernel_size: kernel size in all convolutional blocks
            n_channels: number of output channels in the first convolutional block, after which it is doubled in every 2nd block thereafter
            n_blocks: number of convolutional blocks
            fc_size: size of the first fully connected layer
        """

        super(Discriminator, self).__init__()

        in_channels = 3

        # A series of convolutional blocks
        # The first, third, fifth (and so on) convolutional blocks increase the number of channels but retain image size
        # The second, fourth, sixth (and so on) convolutional blocks retain the same number of channels nut halve image size
        # The firs convolutional block is unique because it does not employ batch normalization
        conv_blocks = list()
        for i in range(n_blocks):
            out_channels = (n_channels if i is 0 else in_channels * 2) if i % 2 is 0 else in_channels
            conv_blocks.append(
                ConvBlock(in_channels=in_channels,
                          out_channels=out_channels,
                          kernel_size=kernel_size,
                          stride=1 if i % 2 is 0 else 2,
                          batch_norm=i is not 0,
                          activation="LeakyReLU")
            )
            in_channels = out_channels
        self.conv_blocks = nn.Sequential(*conv_blocks)

        # An adaptative pool layer resizes it to a standard size
        # For the default input size of 96 and 8 convolutional blocks, this will have no effect
        self.adaptative_pool = nn.AdaptiveAvgPool2d((6, 6))

        self.fc1 = nn.Linear(out_channels * 6 * 6, fc_size)
        self.leaky_relu = nn.LeakyReLU(0.2)
        self.fc2 = nn.Linear(1024, 1)

        # Don't need a sigmoid layer because the sigmoid operation is performed by PyTorch's nn.BCEWithLogitsLoss()
    
    def forward(self, imgs):
        """
        Forward propagation.

        Args:
            imgs: high-resolution or super-resolution images which must be classified as such, a tensor of size (N, 3, w * scaling factor, h * scaling factor)
        Returns:
            a score (logit) for whether it is a high-resolution image, a tensor of size (N)
        """

        batch_size = imgs.size(0)
        output = self.conv_blocks(imgs)
        output = self.adaptative_pool(output)
        output = self.fc1(output.view(batch_size, -1))
        output = self.leaky_relu(output)
        logit = self.fc2(output)

        return logit
    

class TruncatedVGG19(nn.Module):
    """
    A truncated VGG19 network, such that its output is the 'feature map obtained by the j-th convolution (after activation)
    before the i-th maxpooling layer within the VGG19 network', as defined in the paper.

    Used to calculate the MSE loss in this VGG feature-space, i.e. the VGG loss.
    """

    def __init__(self, i, j):
        """
        Args:
            i: the index i in the definition above
            j: the index j in the definition above
        """

        super(TruncatedVGG19, self).__init__()

        vgg19 = torchvision.models.vgg19(pretrained=True)

        maxpool_counter = 0
        conv_counter = 0
        truncate_at = 0

        # Iterate through the convolutional section "features" of the VGG19
        for layer in vgg19.features.children():
            truncate_at += 1

            # Count the number of maxpool layers and the convolutional layers after each maxpool
            if isinstance(layer, nn.Conv2d):
                conv_counter += 1
            if isinstance(layer, nn.MaxPool2d):
                maxpool_counter += 1
                conv_counter = 0

            # Break if we reach the jth convolution after the (i - 1)th maxpool
            if (maxpool_counter == i - 1) and (conv_counter == j):
                break

        assert (maxpool_counter == i - 1) and (conv_counter == j), f"One or both of i={i} and j={j} are not valid choices for the VGG19!"

        # Truncate the jth convolution (+ activation) before the ith maxpool layer
        self.truncated_vgg19 = nn.Sequential(*list(vgg19.features.children())[:truncate_at + 1])

    def forward(self, input):
        """
        Forward propagation

        Args:
            input: high-resolution or super-resolution images, a tensor of size (N, 3, w * scaling factor, h * scaling factor)
        Returns:
            the specified VGG19 feature map, a tensor of size (N, feature_map_channels, feature_map_w, feature_map_h)
        """

        output = self.truncated_vgg19(input)    # (N, feature_map_channels, feature_map_w, feature_map_h)

        return output

