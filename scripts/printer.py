from argparse import ArgumentParser
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle


def createParser() -> ArgumentParser:
    parser = ArgumentParser(prog="python3 printer.py")
    parser.add_argument("filename")

    return parser


def read_input(parser: ArgumentParser) -> (int, int, list[(int, int, int, int)]):
    width = None
    height = None
    boxes = []
    args = parser.parse_args()
    filename = args.filename

    with open(file=filename) as f:
        line = f.readline()
        width, height = map(int, line.strip().split(","))

        for line in f:
            values = tuple(map(int, line.strip().split(",")))
            boxes.append(values)

        f.close()

    return (width, height, boxes)


def draw_palette(width: int, height: int, boxes: list((int, int, int, int))):
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
    plt.show()


if __name__ == "__main__":
    parser = createParser()
    palette_input = read_input(parser)
    draw_palette(palette_input[0], palette_input[1], palette_input[2])
