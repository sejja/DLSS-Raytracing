from PIL import Image
from typing import Tuple
import os
import json
import random
import torchvision.transforms.functional as FT
import torch
import math

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using {device} for utilities")

# Constants
rgb_weights = torch.FloatTensor([65.481, 128.553, 24.966]).to(device)
imagenet_mean = torch.FloatTensor([0.485, 0.456, 0.406]).unsqueeze(1).unsqueeze(2)
imagenet_std = torch.FloatTensor([0.229, 0.224, 0.225]).unsqueeze(1).unsqueeze(2)
imagenet_mean_cuda = torch.FloatTensor([0.485, 0.456, 0.406]).to(device).unsqueeze(0).unsqueeze(2).unsqueeze(3)
imagenet_std_cuda = torch.FloatTensor([0.229, 0.224, 0.225]).to(device).unsqueeze(0).unsqueeze(2).unsqueeze(3)


def create_data_json(train_dirs: Tuple[str], test_dirs: Tuple[str], min_size: int, out_dir: str):
    """
    Create lists for images in the training set and each of the test sets.

    Args:
        train_folders: folders containing the training images (these will be merged)
        test_folders: folders containing the test images (each test folder will form its own test set)
        min_size: minimum width and height of images to be considered
        output_folder: save data lists here
    """

    train_images = list()
    for d in train_dirs:
        for i in os.listdir(d):
            img_path = os.path.join(d, i)
            img = Image.open(img_path, mode="r")
            if (img.width >= min_size) and (img.height >= min_size):
                train_images.append(img_path)

    with open(os.path.join(out_dir, "train_images.json"), "w") as out_file:
        json.dump(train_images, out_file)

    test_images = list()
    for d in test_dirs:
        for i in os.listdir(d):
            img_path = os.path.join(d, i)
            img = Image.open(img_path, mode="r")
            if (img.width >= min_size) and (img.height >= min_size):
                test_images.append(img_path)
            
    with open(os.path.join(out_dir, "test_images.json"), "w") as out_file:
        json.dump(test_images, out_file)


def convert_image(img: Image, source: str, target: str):
    """
    Convert an image from a source format to a target format.

    Args:
        img: image
        source: source format, one of 'pil' (PIL image), '[0, 1]' or '[-1, 1]' (pixel value ranges)
        target: target format, one of 'pil' (PIL image), '[0, 255]', '[0, 1]', '[-1, 1]' (pixel value ranges),
                   'imagenet-norm' (pixel values standardized by imagenet mean and std.),
                   'y-channel' (luminance channel Y in the YCbCr color format, used to calculate PSNR and SSIM)
    Returns:
        converted image
    """

    assert source in {'pil', '[0, 1]', '[-1, 1]'}, f"Cannot convert from source format {source}!"
    assert target in {'pil', '[0, 255]', '[0, 1]', '[-1, 1]', 'imagenet-norm', 'y-channel'}, f"Cannot convert to target format {target}!"

    # Convert from source to [0, 1]
    if source == "pil":
        img = FT.to_tensor(img)
    elif source == "[-1, 1]":
        img = (img + 1.0) / 2.0

    # Convert from [0, 1] to target
    if target == "pil":
        img = FT.to_pil_image(img)
    elif target == "[0, 255]":
        img = 255.0 * img
    elif target == "[-1, 1]":
        img = 2.0 * img - 1.0
    elif target == "imagenet-norm":
        if img.ndimension() == 3:
            img = (img - imagenet_mean) / imagenet_std
        elif img.ndimension() == 4:
            img = (img - imagenet_mean_cuda) / imagenet_std_cuda
    elif target == "y-channel":
        img = torch.matmul(255.0 * img.permute(0, 2, 3, 1)[:, 4:-4, 4:-4, :], rgb_weights) / 255.0 + 16.0
    
    return img


class ImageTransform(object):
    """
    Image transform pipeline.
    """

    def __init__(self, split, crop_size, scaling_factor, lr_img_type, hr_img_type):
        """
        Args:
            split: one of 'train' or 'test'
            crop_size: crop size of HR images
            scaling_factor: LR images will be downsampled from the HR images by this factor
            lr_img_type: the target format for the LR image; see convert_image() above for available formats
            hr_img_type: the target format for the HR image; see convert_image() above for available formats
        """

        self.split = split.lower()
        self.crop_size = crop_size
        self.scaling_factor = scaling_factor
        self.lr_img_type = lr_img_type
        self.hr_img_type = hr_img_type

        assert self.split in {"train", "test"}

    def __call__(self, img):
        """
        Args:
            img: a PIL source image from which the HR image will be cropped, and then downsampled to create the LR image
        Returns:
            LR and HR images in the specified format
        """

        # Crop
        if self.split == "train":
            # take a random fixed-size crop of the image, which will serve as the high resolution (HR) image
            left = random.randint(1, img.width - self.crop_size)
            top = random.randint(1, img.height - self.crop_size)
            right = left + self.crop_size
            bottom = top + self.crop_size
            hr_img = img.crop((left, top, right, bottom))
        else:
            # Take the largest possible center-crop of it such that its dimensions are perfectly divisible by the scaling factor
            x_remainder = img.width % self.scaling_factor
            y_remainder = img.height % self.scaling_factor
            left = x_remainder // 2
            top = x_remainder // 2
            right = left + (img.width - x_remainder)
            bottom = top + (img.height - y_remainder)
            hr_img = img.crop((left, top, right, bottom))

        # Downsize this crop to obtain the low-resolution version of it
        lr_img = hr_img.resize((int(hr_img.width / self.scaling_factor), int(hr_img.height / self.scaling_factor)), Image.BICUBIC)

        assert (hr_img.width == lr_img.width * self.scaling_factor) and (hr_img.height == lr_img.height * self.scaling_factor)

        # Convert the LR and HR images to the requred type
        lr_img = convert_image(lr_img, source="pil", target=self.lr_img_type)
        hr_img = convert_image(hr_img, source="pil", target=self.hr_img_type)

        return lr_img, hr_img


class AverageMeter(object):
    """
    Keeps track of most recent, average, sum, and count of a metric.
    """

    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count


def clip_gradient(optimizer: torch.optim.Optimizer, grad_clip: float):
    """
    Clips gradients computed during backpropagation to avoid explosion of gradients.

    Args:
        optimizer: optimizer with the gradients to be clipped
        grad_clip: clip value
    """

    for group in optimizer.param_groups:
        for param in group["params"]:
            if param.grad is not None:
                param.grad.data.clamp_(-grad_clip, grad_clip)


def save_checkpoint(state, filename):
    """
    Save model checkpoint.

    Args:
        state: checkpoint contents
    """

    torch.save(state, filename)


def adjust_learning_rate(optimizer: torch.optim.Optimizer, shrink_factor: float):
    """
    Shrinks learning rate by a specified factor.

    Args:
        optimizer: optimizer whose learning rate must be shrunk.
        shrink_factor: factor in interval (0, 1) to multiply learning rate with.
    """

    for param_group in optimizer.param_groups:
        param_group["lr"] = param_group["lr"] * shrink_factor

