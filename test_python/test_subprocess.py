#/usr/bin/python

import os
import sys
import subprocess

#proc = subprocess.Popen("pwd")
#command = [sys.executable, 
#        os.path.dirname(os.path.abspath(__file__)) + os.sep + "test_var.py"]

#proc = subprocess.Popen(command, stdout=subprocess.PIPE)
#print proc.stdout   #proc.stdout will be None

#proc = subprocess.Popen("pwd")

pin, pout = os.pipe()

command = [sys.executable, 
        os.path.dirname(os.path.abspath(__file__)) + os.sep + "test_pipe.py", str(pin)]

proc = subprocess.Popen(command)

print os.read(pout, 30)

