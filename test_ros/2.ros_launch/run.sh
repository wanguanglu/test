#!/bin/bash

ROOT=/home/wanguanglu/work/ros_study/2.ros_launch

# make source dir
cd ${ROOT}
mkdir src

# create package
cd ${ROOT}/src
catkin_create_pkg beginner_tutorials std_msgs rospy roscpp

# catkin make
cd ${ROOT}
catkin_make

# source
source ${ROOT}/devel/setup.bash

cp turtlemimic.launch ${ROOT}/src/beginner_tutorials/
roslaunch beginner_tutorials turtlemimic.launch
