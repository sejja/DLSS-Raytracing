import torch
import torch.nn as nn
import multiprocessing
from model import SRResNet, Discriminator
from dataset import SRDataset
from torch.utils.data import DataLoader, random_split


def train(generator: nn.Module, discriminator: nn.Module, train_dl: DataLoader, epochs: int = 10, g_lr: float = 0.001, d_lr: float = 0.001, device: str = "cpu"):

    # Losses
    criterion_adversarial = nn.BCELoss()    # Binary Cross Entropy Loss for discriminator network
    criterion_content = nn.MSELoss()        # MSE Loss for generator network (in addition to the loss calculated from the discriminator)
    
    # Optimizers
    optimizer_G = torch.optim.Adam(generator.parameters(), lr=g_lr)
    optimizer_D = torch.optim.Adam(discriminator.parameters(), lr=d_lr)

    # Learning rate schedulers
    scheduler_G = torch.optim.lr_scheduler.OneCycleLR(
        optimizer_G,
        max_lr=g_lr,
        steps_per_epoch=len(train_dl),
        epochs=epochs,
        anneal_strategy="linear"
    )

    scheduler_D = torch.optim.lr_scheduler.OneCycleLR(
        optimizer_D,
        max_lr=d_lr,
        steps_per_epoch=len(train_dl),
        epochs=epochs,
        anneal_strategy="linear"
    )

    for epoch in range(epochs):
        
        epoch_g_loss = 0.0
        epoch_d_loss = 0.0
        
        n_iters = len(train_dl)
        for batch in train_dl:

            X_batch = batch[0].to(device)
            y_batch = batch[1].to(device)

            # Ground truths
            valid = torch.ones((X_batch.size(0), 1, 1, 1), requires_grad=False).to(device)
            fake = torch.zeros((X_batch.size(0), 1, 1, 1), requires_grad=False).to(device)

            #######################
            ### TRAIN GENERATOR ###
            #######################

            optimizer_G.zero_grad()

            # Generate superresolution images
            gen_y = generator(X_batch)

            # Adversarial loss (How well the generator fooled the discriminator)
            g_adv_loss = criterion_adversarial(discriminator(gen_y), valid)

            # Content loss (How well the generated images match the ground truth)
            g_content_loss = criterion_content(gen_y, y_batch)

            # Total generator loss (combination of adversarial and content loss)
            g_loss = g_content_loss + 1e-3 * g_adv_loss

            # Update generator parameters
            g_loss.backward()
            optimizer_G.step()
            scheduler_G.step()

            ###########################
            ### TRAIN DISCRIMINATOR ###
            ###########################

            optimizer_D.zero_grad()

            # Discriminator loss for real images
            real_loss = criterion_adversarial(discriminator(y_batch), valid)

            # Discriminator loss for fake images
            fake_loss = criterion_adversarial(discriminator(gen_y.detach()), fake)

            # Total discriminator loss
            d_loss = (real_loss + fake_loss) / 2

            # Update discriminator parameters
            d_loss.backward()
            optimizer_D.step()
            scheduler_D.step()

            epoch_g_loss += g_loss.item() / n_iters
            epoch_d_loss += d_loss.item() / n_iters

        print(f"Epoch {epoch + 1:>2}: Generator loss -> {epoch_g_loss:.4f}, Discriminator loss -> {epoch_d_loss:.4f}")


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

    print(f"loss -> {loss:.4f}")


def main(
    model_path: str,
    dataset_csv: str,
    scaling: int = 4,
    train_split: float = 0.8,
    epochs: int = 10,
    g_learning_rate: int = 0.001,
    d_learning_rate: int = 0.001,
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
    generator = SRResNet(input_channels=3, n_channels=64, n_blocks=8, scaling=scaling).to(device)
    discriminator = Discriminator(input_channels=3, n_channels=64, n_blocks=8).to(device)
    
    train(generator, discriminator, train_dl, epochs=epochs, g_lr=g_learning_rate, d_lr=d_learning_rate, device=device)

    # Evaluation
    generator.eval()
    eval(generator, test_dl, device=device)

    # Compile and save model with torchscript
    generator.to("cpu")
    generator = torch.jit.script(generator)
    torch.jit.save(generator, model_path)


if __name__ == "__main__":

    torch.manual_seed(1234)

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print(f"Using device {device}")

    main(
        model_path="models/SRGANx2.pth",
        dataset_csv="data/DIV2K/train_x2.csv",
        scaling=2,
        train_split=0.8,
        epochs=20,
        g_learning_rate=0.00075,
        d_learning_rate=0.001,
        batch_size=2,
        device=device
    )

    # Current eval loss: 0.0008
