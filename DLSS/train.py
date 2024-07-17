import torch
import torch.nn as nn
import multiprocessing
from model import SRResNet, SRResNet_v2
from dataset import SRDataset
from torch.utils.data import DataLoader, random_split


def train(model: nn.Module, train_dl: DataLoader, epochs: int = 10, learning_rate: float = 0.001, device: str = "cpu"):

    criterion = nn.MSELoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
    scheduler = torch.optim.lr_scheduler.OneCycleLR(
        optimizer,
        max_lr=0.001,
        steps_per_epoch=len(train_dl),
        epochs=epochs,
        anneal_strategy="linear"
    )

    for epoch in range(epochs):
        epoch_loss = 0.0
        n_iters = len(train_dl)
        for batch in train_dl:

            X_batch = batch[0].to(device)
            y_batch = batch[1].to(device)

            # Reset Gradients
            optimizer.zero_grad()

            # Forward pass
            pred = model(X_batch)
            loss = criterion(pred, y_batch)

            # Backward pass
            loss.backward()
            optimizer.step()
            scheduler.step()

            epoch_loss += loss.item() / n_iters

        print(f"Epoch {epoch + 1:>2}: loss -> {epoch_loss:.4f}")


def eval(model: nn.Module, test_dl: DataLoader, device: str = "cpu"):

    criterion = nn.MSELoss()

    with torch.no_grad():
        loss = 0.0
        n_iters = len(test_dl)
        for batch in test_dl:

            X_batch = batch[0].to(device)
            y_batch = batch[1].to(device)

            pred = model(X_batch)
            loss += criterion(pred, y_batch).item() / n_iters

    print(f"loss -> {loss:.2f}")


def main(
    model_path: str,
    dataset_csv: str,
    scaling: int = 4,
    train_split: float = 0.8,
    epochs: int = 10,
    learning_rate: int = 0.001,
    batch_size: int = 32,
    device: str = "cpu"
    ):

    # Loading dataset
    dataset = SRDataset(dataset_csv, crop_size=(720, 720), scaling=scaling)

    # Test train split
    train_len = int(len(dataset) * train_split)
    test_len = len(dataset) - train_len

    train_ds, test_ds = random_split(dataset, [train_len, test_len])

    num_cpus = multiprocessing.cpu_count()
    train_dl = DataLoader(train_ds, batch_size, shuffle=True, num_workers=num_cpus)
    test_dl = DataLoader(test_ds, batch_size, shuffle=False, num_workers=num_cpus)


    # Training
    model = SRResNet_v2(input_channels=3, n_channels=64, n_blocks=4, scaling=scaling)
    model = model.to(device)
    
    train(model, train_dl, epochs=epochs, learning_rate=learning_rate, device=device)

    model.eval()
    eval(model, test_dl, device=device)


    # Compile and save model with torchscript
    model.to("cpu")
    model = torch.jit.script(model)
    torch.jit.save(model, model_path)


if __name__ == "__main__":

    torch.manual_seed(1234)

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print(f"Using device {device}")

    main(
        model_path="models/SRREsNetx2.pth",
        dataset_csv="data/train_x2.csv",
        scaling=2,
        train_split=0.8,
        epochs=20,
        learning_rate=0.001,
        batch_size=4,
        device=device
    )
