import os
import torch
import csv
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import Dataset
from torch import Tensor
from typing import Tuple
from utils import png_to_tensor


class SRDataset(Dataset):

    def __init__(self, data_csv: str) -> None:
        
        self.data = list()
        with open(data_csv, "r") as file:
            
            reader = csv.reader(file)

            for row in reader:
                self.data.append(row)

    def __len__(self) -> int:
        return len(self.data)
    
    def __getitem__(self, index) -> Tuple[Tensor, Tensor]:
        
        lr_path, hr_path = self.data[index]

        return png_to_tensor(lr_path), png_to_tensor(hr_path)
    

if __name__ == "__main__":

    import matplotlib
    import matplotlib.pyplot as plt
    matplotlib.use('TkAgg')

    dataset = SRDataset("data/train.csv")

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

