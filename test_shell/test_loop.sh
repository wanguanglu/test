#!/bin/bash

vec=(1 2 3)
for i in ${vec[*]}
do
  echo $i
done

echo 
echo 

i=0
max=10
while [ $i -le $max ]
do
  echo $i
  i=$(($i+1))
done
