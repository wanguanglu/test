#!/usr/bin/env python3

import os
import sys

args = sys.argv

pout = int(args[1])

print pout

os.write(pout, 'test')
