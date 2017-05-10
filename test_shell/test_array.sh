#!/bin/bash

array=(one two three)
echo "total values: " ${array[*]}

echo "array[0]: " ${array[0]}

array[3]=four
echo "total values: " ${array[*]}

unset array[1]
echo "after delete array[1]"
echo "total values: " ${array[*]}
