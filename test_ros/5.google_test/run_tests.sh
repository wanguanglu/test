#!/bin/bash
catkin_make run_tests --source .
if [[ $? -ne 0 ]]
then
    echo "*****************"
    echo "Unit Test Run Fail!"
    exit 1
fi
catkin_test_results build/test_results

# If you want to run in a custom way:
#
# -- all unit tests from a package --
# catkin_make run_tests_packagename
#
# -- all unit tests from a package of a specific type --
# catkin_make run_tests_packagename_gtest
#
# -- a individual gtest target --
# catkin_make run_tests_packagename_gtest_mytest
