from utils import create_data_json


if __name__ == "__main__":

    train_dirs = ["data/COCO/train2014"]
    test_dirs = ["data/COCO/val2014"]
    out_dir = "data/COCO"
    min_size = 100

    create_data_json(train_dirs, test_dirs, min_size, out_dir)
    