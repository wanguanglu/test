#!/usr/bin/env python3

infile = open('input.txt', 'r')
outfile = open('output.txt', 'w')

for line in infile:
    line = line.strip()
    print(line)
    outfile.write('%s\n' % line)

infile.close()
outfile.close()
