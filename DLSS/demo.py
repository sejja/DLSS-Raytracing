import torch
import matplotlib.pyplot as plt
from dataset import SRDataset

if __name__ == "__main__":

    model = torch.jit.load("models/SRResNetx8.pth")

    dataset = SRDataset("data/train_x8.csv", crop_size=(200, 200), scaling=8)

    for low_res, high_res in dataset:

        low_res = low_res.permute(1, 2, 0)
        
        high_res = low_res.permute(2, 0, 1).unsqueeze(dim=0)
        high_res = model(high_res)[0]
        high_res = high_res.permute(1, 2, 0).detach().numpy()

        fig, axes = plt.subplots(1, 2, figsize=(12, 6))

        axes[0].imshow(low_res)
        axes[0].axis("off")
        axes[0].set_title("Low Resolution")

        axes[1].imshow(high_res)
        axes[1].axis("off")
        axes[1].set_title("High Resolution")

        plt.tight_layout()
        plt.show()

