#/usr/bin/python

import os
import sys
import ConfigParser


cf = ConfigParser.ConfigParser()
cf.read('test.conf')

print 'sections:', cf.sections()

for section in cf.sections():
    print 'section:' + section
    print cf.options(section)
    print cf.items(section)
