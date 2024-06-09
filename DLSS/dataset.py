import csv
from torchvision import transforms
from torch.utils.data import Dataset
from torch import Tensor
from typing import Tuple
from utils import png_to_tensor


class SRDataset(Dataset):

    def __init__(self, data_csv: str, crop_size: Tuple[int, int] = (64, 64), scaling: int = 4) -> None:
        
        self.data = list()
        with open(data_csv, "r") as file:
            
            reader = csv.reader(file)

            for row in reader:
                self.data.append(row)

        self.crop_size = crop_size
        self.scaling = scaling

    def __len__(self) -> int:
        return len(self.data)
    
    def __getitem__(self, index) -> Tuple[Tensor, Tensor]:
        
        lr_path, hr_path = self.data[index]

        lr_image = png_to_tensor(lr_path)
        hr_image = png_to_tensor(hr_path)

        # Adjust crop size if one of the dimensions is smaller than the default crop size
        adjusted_crop_size = (min(lr_image.size(1), self.crop_size[0]), min(lr_image.size(2), self.crop_size[1]))

        i, j, h, w = transforms.RandomCrop.get_params(lr_image, output_size=adjusted_crop_size)
        lr_image = transforms.functional.crop(lr_image, i, j, h, w)
        hr_image = transforms.functional.crop(hr_image, i * self.scaling, j * self.scaling, h * self.scaling, w * self.scaling)

        # Resize images if needed
        lr_image = transforms.functional.resize(lr_image, self.crop_size)
        hr_image = transforms.functional.resize(hr_image, (self.crop_size[0] * self.scaling, self.crop_size[1] * self.scaling))


        return lr_image, hr_image
    

if __name__ == "__main__":

    import matplotlib.pyplot as plt

    dataset = SRDataset("data/train_x8.csv", crop_size=(256, 256), scaling=8)

    for low_res, high_res in dataset:

        low_res = low_res.permute(1, 2, 0)
        high_res = high_res.permute(1, 2, 0)

        fig, axes = plt.subplots(1, 2, figsize=(12,6))

        axes[0].imshow(low_res)
        axes[0].set_title("Low Resolution")

        axes[1].imshow(high_res)
        axes[1].set_title("High Resolution")

        plt.tight_layout()
        plt.show()

