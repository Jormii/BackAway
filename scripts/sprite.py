import os
import numpy as np
from PIL import Image

INPUT_DIR = "./assets/"
OUTPUT_DIR = "./compilation/resources/sprites/"
IMG_EXTENSIONS = {".png"}


def raw_sprite(path, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    img = Image.open(path)
    width = np.int32(img.width)
    height = np.int32(img.height)
    bitmap = np.array(img)

    _, filename = os.path.split(path)
    filename_no_ext, _ = os.path.splitext(filename)
    output_path = os.path.join(output_dir, f"{filename_no_ext}.sprite")
    with open(output_path, "wb") as fd:
        fd.write(width.tobytes())
        fd.write(height.tobytes())
        fd.write(bitmap.tobytes())


def main():
    for filename in os.listdir(INPUT_DIR):
        _, extension = os.path.splitext(filename)
        if extension in IMG_EXTENSIONS:
            path = os.path.join(INPUT_DIR, filename)
            raw_sprite(path, OUTPUT_DIR)


if __name__ == "__main__":
    main()
