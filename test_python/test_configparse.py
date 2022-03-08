#!/usr/bin/env python3

import os
import sys
import configparser

cf = configparser.ConfigParser()
cf.read('test.conf')

print('sections:', cf.sections())

for section in cf.sections():
    print('section:' + section)
    print(cf.options(section))
    print(cf.items(section))
