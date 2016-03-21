#/usr/bin/python


import os
import sys

testfile = open('test.txt', 'w')
sys.stdout = testfile
print 'test'
print 'Jusr For Test'

testfile.close()

sys.stdout = sys.__stdout__

#for line in sys.stdin:
#    print line

testfile = open('test.txt', 'r')
sys.stdin = testfile
for line in sys.stdin:
    print line

testfile.close()
