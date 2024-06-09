import os
import csv
import torchvision.transforms.functional as FT
from torch import Tensor
from PIL import Image


def create_data_csv(high_res_dir: str, low_res_dir: str, csv_path: str) -> None:
    """
    Create csv file for div2k dataset.

    Args:
        high_res_dir (str): Directories of the high ersolution images.
        low_res_dir (str): Directories of the low resolution images.
        csv_path (str): Path of the output csv file.
    """

    data = list()
    for hr_file, lr_file in zip(sorted(os.listdir(high_res_dir)), sorted(os.listdir(low_res_dir))):
        hr_path = os.path.join(high_res_dir, hr_file)
        lr_path = os.path.join(low_res_dir, lr_file)

        data.append({"low_res": lr_path, "high_res": hr_path})

    with open(csv_path, "w") as file:
        writer = csv.DictWriter(file, fieldnames=["low_res", "high_res" ])
        writer.writerows(data)


def png_to_tensor(image_path: str) -> Tensor:
    """
    Convert PNG image to a torch tensor.

    Args:
        image_path (str): Path of the png image.
    
    Returns:
        (Tensor): Tensor resulting from the png conversion.
    """

    image = Image.open(image_path).convert("RGB")
    tensor = FT.to_tensor(image)

    return tensor

