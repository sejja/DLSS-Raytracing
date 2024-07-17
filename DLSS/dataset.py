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
                self.data.append(row[0])

        self.crop_size = crop_size
        self.scaling = scaling

    def __len__(self) -> int:
        return len(self.data)
    
    def __getitem__(self, index) -> Tuple[Tensor, Tensor]:
        
        hr_path = self.data[index]
        hr_image = png_to_tensor(hr_path)

        hr_image = transforms.functional.resize(hr_image, (self.crop_size[0], self.crop_size[1]))
        lr_image = transforms.functional.resize(hr_image, (self.crop_size[0] // self.scaling, self.crop_size[1] // self.scaling))

        return lr_image, hr_image
    

if __name__ == "__main__":

    import matplotlib.pyplot as plt

    dataset = SRDataset("data/train_x2.csv", crop_size=(1080, 1080), scaling=2)

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

