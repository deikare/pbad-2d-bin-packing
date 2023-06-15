from argparse import ArgumentParser
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from os import listdir
from os.path import isfile, join
import os


def read_filename_or_directory() -> (str, bool):
    parser = ArgumentParser(prog="python3 printer.py")
    parser.add_argument("filename")
    parser.add_argument("-d", "--directory", action="store_true")
    args = parser.parse_args()

    return (args.filename, args.directory)


def read_input(filepath: str) -> (int, int, list[(int, int, int, int), int]):
    width = None
    height = None
    boxes = []
    fval = None

    with open(file=filepath) as f:
        line = f.readline()
        width, height = map(int, line.strip().split(","))

        for line in f:
            if "," not in line:
                fval = float(line.strip())
                break
            values = tuple(map(int, line.strip().split(",")))
            boxes.append(values)

        f.close()

    return (width, height, boxes, fval)


def draw_palette(
    width: int,
    height: int,
    boxes: list((int, int, int, int)),
    fval: float,
    directory: str = None,
    filename: str = None,
):
    fig, ax = plt.subplots()
    ax.set_aspect("equal")

    # Draw container
    container = Rectangle((0, 0), width, height, fill=False, edgecolor="black")
    ax.add_patch(container)

    # Draw boxes
    for box in boxes:
        x, y, box_width, box_height = box
        rect = Rectangle(
            (x, y),
            box_width,
            box_height,
            fill=True,
            edgecolor="black",
            facecolor="gray",
        )
        ax.add_patch(rect)

    plt.xlim(0, width)
    plt.ylim(0, height)
    plt.gca().set_aspect("equal", adjustable="box")
    plt.title(f"width={width}, height={height}, fval={fval}")
    # plt.show()
    if directory and filename:
        output_path = directory + "plot_" + os.path.splitext(filename)[0] + ".png"
        plt.savefig(output_path)
    plt.close()


if __name__ == "__main__":
    (path, directory_flag) = read_filename_or_directory()
    if directory_flag:
        files = [
            f
            for f in listdir(path)
            if isfile(join(path, f)) and os.path.splitext(f)[1] == ".txt"
        ]
        print(files)
        if path[-1] != "/":
            path += "/"
        for filename in files:
            filepath = path + filename
            palette_input = read_input(filepath)
            draw_palette(
                palette_input[0],
                palette_input[1],
                palette_input[2],
                palette_input[3],
                path,
                filename,
            )
    else:
        palette_input = read_input(path)
        draw_palette(
            palette_input[0], palette_input[1], palette_input[2], palette_input[3]
        )
