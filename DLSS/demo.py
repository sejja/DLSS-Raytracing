import os
import torch
from utils import *
from PIL import Image, ImageDraw, ImageFont

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# # Model checkpoints
# srgan_checkpoint = "./models/checkpoint_srgan.pth.tar"
# srresnet_checkpoint = "./models/checkpoint_srresnet.pth.tar"

# # Load models
# srresnet = torch.load(srresnet_checkpoint)['model'].to(device)
# srresnet.eval()
# srgan_generator = torch.load(srgan_checkpoint)['generator'].to(device)
# srgan_generator.eval()

srresnet = torch.load("./models/srresnet.pth").to(device)
srresnet.eval()
srgan_generator = torch.load("./models/srgan.pth").to(device)
srgan_generator.eval()

def visualize_sr(img, i, halve=False):
    """
    Visualizes the super-resolved images from the SRResNet and SRGAN for comparison with the bicubic-upsampled image
    and the original high-resolution (HR) image, as done in the paper.
    """

    # Load image, downsample to obtain low-res version
    hr_img = Image.open(img, mode="r")
    hr_img = hr_img.convert('RGB')
    if halve:
        hr_img = hr_img.resize((int(hr_img.width / 2), int(hr_img.height / 2)), Image.LANCZOS)
    
    lr_img = hr_img.resize((int(hr_img.width / 4), int(hr_img.height / 4)), Image.BICUBIC)

    # Bicubic Upsampling
    bicubic_img = lr_img.resize((hr_img.width, hr_img.height), Image.BICUBIC)

    # Super-resolution (SR) with SRResNet
    sr_img_srresnet = srresnet(convert_image(lr_img, source='pil', target='imagenet-norm').unsqueeze(0).to(device))
    sr_img_srresnet = sr_img_srresnet.squeeze(0).cpu().detach()
    sr_img_srresnet = convert_image(sr_img_srresnet, source='[-1, 1]', target='pil')

    # Super-resolution (SR) with SRGAN
    sr_img_srgan = srgan_generator(convert_image(lr_img, source='pil', target='imagenet-norm').unsqueeze(0).to(device))
    sr_img_srgan = sr_img_srgan.squeeze(0).cpu().detach()
    sr_img_srgan = convert_image(sr_img_srgan, source='[-1, 1]', target='pil')

    # Create grid
    margin = 40
    grid_img = Image.new('RGB', (2 * hr_img.width + 3 * margin, 2 * hr_img.height + 3 * margin), (255, 255, 255))

    # Font
    draw = ImageDraw.Draw(grid_img)
    try:
        font = ImageFont.truetype("calibril.ttf", size=23)
    except OSError:
        font = ImageFont.load_default()

    # Place bicubic-upsampled image
    grid_img.paste(bicubic_img, (margin, margin))
    text_bbox = draw.textbbox((0, 0), "Bicubic", font=font)
    text_size = (text_bbox[2] - text_bbox[0], text_bbox[3] - text_bbox[1])
    draw.text(xy=[margin + bicubic_img.width / 2 - text_size[0] / 2, margin - text_size[1] - 5], text="Bicubic", font=font, fill='black')

    # Place SRResNet image
    grid_img.paste(sr_img_srresnet, (2 * margin + bicubic_img.width, margin))
    text_bbox = draw.textbbox((0, 0), "SRResNet", font=font)
    text_size = (text_bbox[2] - text_bbox[0], text_bbox[3] - text_bbox[1])
    draw.text(xy=[2 * margin + bicubic_img.width + sr_img_srresnet.width / 2 - text_size[0] / 2, margin - text_size[1] - 5], text="SRResNet", font=font, fill='black')

    # Place SRGAN image
    grid_img.paste(sr_img_srgan, (margin, 2 * margin + sr_img_srresnet.height))
    text_bbox = draw.textbbox((0, 0), "SRGAN", font=font)
    text_size = (text_bbox[2] - text_bbox[0], text_bbox[3] - text_bbox[1])
    draw.text(xy=[margin + bicubic_img.width / 2 - text_size[0] / 2, 2 * margin + sr_img_srresnet.height - text_size[1] - 5],
              text="SRGAN", font=font, fill='black')

    # Place original HR image
    grid_img.paste(hr_img, (2 * margin + bicubic_img.width, 2 * margin + sr_img_srresnet.height))
    text_bbox = draw.textbbox((0, 0), "Original HR", font=font)
    text_size = (text_bbox[2] - text_bbox[0], text_bbox[3] - text_bbox[1])
    draw.text(xy=[2 * margin + bicubic_img.width + sr_img_srresnet.width / 2 - text_size[0] / 2,
                  2 * margin + sr_img_srresnet.height - text_size[1] - 1], text="Original HR", font=font, fill='black')

    # Save the image using PIL instead of matplotlib
    output_path = f"./data/COCO/superres/{i}.jpeg"
    grid_img.save(output_path, format="JPEG")

    return grid_img


if __name__ == '__main__':

    data_dir = "./data/COCO/"
    
    with open(os.path.join(data_dir, "test_images.json"), "r") as file:
        images = sorted(json.load(file))

    for i, img in enumerate(images):
        visualize_sr(img, i)

    # visualize_sr("./data/COCO/val2014/COCO_val2014_000000579655.jpg", 0)
    
