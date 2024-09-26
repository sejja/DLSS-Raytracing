import os
import torch
from utils import *

# Model checkpoints
srgan_checkpoint = "./models/checkpoint_srgan.pth.tar"
srresnet_checkpoint = "./models/checkpoint_srresnet.pth.tar"

# Load models
srresnet = torch.load(srresnet_checkpoint)['model'].to(device)
srresnet.eval()
srgan_generator = torch.load(srgan_checkpoint)['generator'].to(device)
srgan_generator.eval()

srresnet = srresnet.to("cpu")
srgan_generator = srgan_generator.to("cpu")

torch.save(srresnet, "srresnet.pth")
torch.save(srgan_generator, "srgan.pth")
