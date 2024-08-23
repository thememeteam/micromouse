#!/bin/python3

from mazelib import *
from mazelib.generate.AldousBroder import AldousBroder

m = Maze()
m.generator = AldousBroder(16, 16)
m.generate()
m.generate_entrances()

f = open('maze.txt', 'w', newline='\n')
f.write(str(m))
f.close()

print(str(m).replace('\n', ''))