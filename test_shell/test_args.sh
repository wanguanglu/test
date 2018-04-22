#!/bin/bash

arg_num=$#
echo "arg number: " $arg_num

args1=$1
args2=$2

echo "args1 is: " $args1
echo "args2 is: " $args2

# another way to visit
echo "args1 is " ${!1}
