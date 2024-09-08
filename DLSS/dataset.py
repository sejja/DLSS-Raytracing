import torch
from torch.utils.data import Dataset
import json
import os
from PIL import Image
from utils import ImageTransform


class SRDataset(Dataset):
    """
    A PyTorch Dataset to be used by a PyTorch DataLoader.
    """

    def __init__(self, data_dir, split, crop_size, scaling_factor, lr_img_type, hr_img_type):
        """
        Args:
            data_folder: folder with JSON data files
            split: one of 'train' or 'test'
            crop_size: crop size of target HR images
            scaling_factor: the input LR images will be downsampled from the target HR images by this factor; the scaling done in the super-resolution
            lr_img_type: the format for the LR image supplied to the model; see convert_image() in utils.py for available formats
            hr_img_type: the format for the HR image supplied to the model; see convert_image() in utils.py for available formats
        """

        self.data_dir = data_dir
        self.split = split.lower()
        self.crop_size = crop_size
        self.scaling_factor = int(scaling_factor)
        self.lr_img_type = lr_img_type
        self.hr_img_type = hr_img_type

        assert self.split in {"train", "test"}
        assert lr_img_type in {'[0, 255]', '[0, 1]', '[-1, 1]', 'imagenet-norm'}
        assert hr_img_type in {'[0, 255]', '[0, 1]', '[-1, 1]', 'imagenet-norm'}

        # If this is a training dataset, then crop dimensions must be perfectly divisible by the scaling factor
        # (If this is a test dataset, images are not cropped to a fixed size, so this variable isn't used)
        if self.split == "train":
            with open(os.path.join(data_dir, "train_images.json"), "r") as file:
                self.images = json.load(file)
        else:
            with open(os.path.join(data_dir, "test_images.json"), "r") as file:
                self.images = json.laod(file)
        
        # Select the correct set of transforms
        self.transform = ImageTransform(split=self.split,
                                         crop_size=self.crop_size,
                                         scaling_factor=self.scaling_factor,
                                         lr_img_type=self.lr_img_type,
                                         hr_img_type=self.hr_img_type)

    def __getitem__(self, i):
        """
        This method is required to be defined for use in the PyTorch DataLoader.

        Args:
            i: index to retrieve
        Returns:
            the 'i'th pair LR and HR images to be fed into the model
        """

        # Read image
        img = Image.open(self.images[i], mode="r")
        img = img.convert("RGB")
        lr_img, hr_img = self.transform(img)

        return lr_img, hr_img
    
    def __len__(self):
        """
        This method is required to be defined for use in the PyTorch DataLoader.

        Returns:
            size of this data (in number of images)
        """

        return len(self.images)
    

