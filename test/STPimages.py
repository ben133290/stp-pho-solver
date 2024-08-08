from PIL import Image

TILE_COLOR = (0, 0, 0)
NO_TILE_COLOR = (200, 200, 200)
BACKGROUND = (255, 255, 255)
BLANK = (0, 255, 100)
IMAGE_SIZE = 6


def create_sliding_tile_image(pattern, filename):
    image = Image.new('RGB', (4, 4), NO_TILE_COLOR)

    for tile in pattern:
        if tile != 0:  # don't draw blank
            x, y = tile % 4, tile // 4
            image.putpixel((x, y), TILE_COLOR)
        else:
            x, y = tile % 4, tile // 4
            image.putpixel((x, y), BLANK)

    large_image = image.resize((IMAGE_SIZE, IMAGE_SIZE), Image.Resampling.NEAREST)
    large_image.save(filename, "PNG")


def create_sliding_tile_image_full(patterns, filename, width, height):
    image = Image.new('RGB', (width, height), BACKGROUND)

    x_offset = 1
    y_offset = 1
    for pattern in patterns:
        for tile in range(16):

            if tile != 0 and tile in pattern:  # don't draw blank
                x, y = tile % 4, tile // 4
                image.putpixel((x_offset + x, y_offset + y), TILE_COLOR)
            elif tile == 0:  # draw blank
                x, y = tile % 4, tile // 4
                image.putpixel((x_offset + x, y_offset + y), BLANK)
            else:
                x, y = tile % 4, tile // 4
                image.putpixel((x_offset + x, y_offset + y), NO_TILE_COLOR)

        x_offset += 5
        if x_offset + 4 >= width:
            x_offset = 1
            y_offset += 5

    large_image = image#.resize((IMAGE_SIZE*width, IMAGE_SIZE*height), Image.Resampling.NEAREST)
    large_image.save(filename, "PNG")


def read_patterns(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    total_patterns = int(lines[0].strip())
    patterns = []
    index = 1

    for _ in range(total_patterns):
        num_count = int(lines[index].strip())
        index += 1
        pattern = list(map(int, lines[index].strip().split()))
        patterns.append(pattern)
        index += 1

    return patterns


def fun1(patterns):
    index = 0
    for pattern in patterns:
        create_sliding_tile_image(pattern, "p" + str(index) + ".png")
        index += 1


def fun2(patterns):
    create_sliding_tile_image_full(patterns, "patterns100.png", width=20*5+1, height=22*5+1)


if __name__ == '__main__':
    patterns = read_patterns("../opho_01_100.txt")
    fun2(patterns)
