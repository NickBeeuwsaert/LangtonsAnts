#!/usr/bin/env python
import struct
import random
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--width', '-W', type=int, default=256)
parser.add_argument('--height', '-H', type=int, default=256)
parser.add_argument('--tile-size', type=int, default=2)
parser.add_argument('--num-ants', '-n', type=int, default=256)

options = parser.parse_args()

w, h = options.width, options.height
with open("struct.ant", "w+") as output:
    #header
    output.write(struct.pack("<HHH", w, h, options.tile_size))
    num_ants = options.num_ants
    output.write(struct.pack("<I", num_ants))
    for ant in range(num_ants):
        x, y, direction = [random.randint(1, n-1) for n in w,h,3]
        output.write(struct.pack("<HHb", x, y, direction))
