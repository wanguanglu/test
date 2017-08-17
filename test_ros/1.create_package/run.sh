#!/bin/bash

ROOT="$(cd `dirname $0`; pwd)"
echo $ROOT

# make source dir
mkdir src

# create package
cd ${ROOT}/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp

# catkin make
cd ${ROOT}
catkin_make

# source
source ${ROOT}/devel/setup.bash

# display rospack
echo "show depends1"
rospack depends1 beginner_tutorials 
echo
echo

echo "show depends"
rospack depends beginner_tutorials
echo
echo

# display package.xml
echo "display package.xml"
roscd beginner_tutorials/
cat package.xml
echo
echo
