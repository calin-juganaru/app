import argparse
from PIL import Image

# =============================================================================

def compare(x, y, e):
    image_1 = Image.open(x)
    image_2 = Image.open(y)

    assert image_1.size == image_2.size, 'Dimeniunile diferă'

    pairs = zip(image_1.getdata(), image_2.getdata())

    if len(image_1.getbands()) == 1:
        dif = list(abs(p1 - p2) for p1, p2 in pairs)
    else:
        dif = list(abs(c1 - c2) for p1, p2 in pairs for c1, c2 in zip(p1, p2))

    assert len(list(filter(lambda x: x > e, dif))) == 0, 'Imaginile diferă'

    print('Corect!')

# =============================================================================

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('x')
    parser.add_argument('y')
    parser.add_argument('--e', type=int, default=2, help='Diferenta maxima.')
    args = parser.parse_args()
    compare(args.x, args.y, args.e)

if __name__ == '__main__':
    main()

# =============================================================================