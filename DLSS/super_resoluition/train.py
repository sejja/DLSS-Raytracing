import torch

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

print(f"Using device: {device}")

a = torch.tensor(
   [[1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]],
    dtype=torch.float32,
    device=device
)

b = a @ a

print(b)
