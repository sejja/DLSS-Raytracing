from torch import nn
from torch import Tensor
from torch.nn import Module


class ConvBlock(Module):

    def __init__(
            self,
            in_channels: int,
            out_channels: int,
            kernel_size: int,
            stride: int = 1,
            batchnorm: bool = False,
            activation: str = None
        ) -> None:
        super(ConvBlock, self).__init__()

        layers = list()

        layers.append(nn.Conv2d(in_channels, out_channels, kernel_size, stride, padding="same", bias = not batchnorm))

        if batchnorm is True:
            layers.append(nn.BatchNorm2d(out_channels))

        if activation == "relu":
            layers.append(nn.ReLU())
        elif activation == "prelu":
            layers.append(nn.PReLU())
        elif activation == "leakyrelu":
            layers.append(nn.LeakyReLU(0.1))
        elif activation == "tanh":
            layers.append(nn.Tanh())

        self.conv_block = nn.Sequential(*layers)

    def forward(self, x) -> Tensor:
        return self.conv_block(x)
    

class ResBlock(Module):

    def __init__(self, n_channels: int, kernel_size: int) -> None:
        super(ResBlock, self).__init__()

        self.conv1 = ConvBlock(
            n_channels,
            n_channels,
            kernel_size,
            batchnorm=True,
            activation="relu"
        )
        
        self.conv2 = ConvBlock(
            n_channels,
            n_channels,
            kernel_size,
            batchnorm=True,
            activation=None
        )

    def forward(self, x) -> Tensor:

        output = self.conv1(x)
        output = self.conv2(output)
        output = output + x

        return output
    

class PixelSuffle(Module):

    def __init__(self, n_channels: int, scaling: int = 2) -> None:
        super(PixelSuffle, self).__init__()

        # Convlutional layer increases the number of chanels by scaling**2
        self.conv = nn.Conv2d(
            in_channels=n_channels,
            out_channels=n_channels * (scaling ** 2),
            kernel_size=3,
            padding="same"
        )

        # The extra channels are suffled to form a higher resolution image
        self.pixel_suffle = nn.PixelShuffle(upscale_factor=scaling)
        self.relu = nn.ReLU()

    def forward(self, x) -> Tensor:

        output = self.conv(x)
        output = self.pixel_suffle(output)
        output = self.relu(output)

        return output


class SRResNet(Module):

    def __init__(self, input_channels: int, n_channels: int, n_blocks: int, scaling: int = 4) -> None:
        super(SRResNet, self).__init__()

        self.conv1 = ConvBlock(
            in_channels=input_channels,
            out_channels=n_channels,
            kernel_size=9,
            batchnorm=False,
            activation="relu"
        )

        blocks = list()
        for _ in range(n_blocks):
            blocks.append(ResBlock(n_channels, 3))
        self.res_blocks = nn.Sequential(*blocks)

        self.conv2 = ConvBlock(
            in_channels=n_channels,
            out_channels=n_channels,
            kernel_size=3,
            batchnorm=True,
            activation=None
        )

        self.subpixel_conv = PixelSuffle(
            n_channels=n_channels,
            scaling=scaling
        )

        self.conv3 = ConvBlock(
            in_channels=n_channels,
            out_channels=3,
            kernel_size=3,
            batchnorm=False,
            activation="relu"
        )

    def forward(self, x):

        output = self.conv1(x)
        residual = output
        output = self.res_blocks(output)
        output = self.conv2(output)
        output = output + residual
        output = self.subpixel_conv(output)
        output = self.conv3(output)

        return output
    

class SRResNet_v2(Module):

    def __init__(self, input_channels: int, n_channels: int, n_blocks: int, scaling: int = 4) -> None:
        super(SRResNet_v2, self).__init__()

        self.upsample = nn.Upsample(scale_factor=scaling)

        self.conv1 = ConvBlock(
            in_channels=input_channels,
            out_channels=n_channels,
            kernel_size=3,
            batchnorm=False,
            activation="relu"
        )

        blocks = list()
        for _ in range(n_blocks):
            blocks.append(ResBlock(n_channels, 3))
        self.res_blocks = nn.Sequential(*blocks)

        self.conv2 = ConvBlock(
            in_channels=n_channels,
            out_channels=n_channels,
            kernel_size=3,
            batchnorm=True,
            activation=None
        )

        self.conv3 = ConvBlock(
            in_channels=n_channels,
            out_channels=3,
            kernel_size=3,
            batchnorm=False,
            activation="relu"
        )

    def forward(self, x):

        output = self.upsample(x)
        output = self.conv1(output)
        residual = output
        output = self.res_blocks(output)
        output = self.conv2(output)
        output = output + residual
        output = self.conv3(output)

        return output
    
