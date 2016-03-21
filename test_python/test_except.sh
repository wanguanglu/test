#!/bin/bash

python test_except.py

ret=$?
if test $ret -ne 0
then
    echo "Exception in test_except.py"
    exit -1
fi
