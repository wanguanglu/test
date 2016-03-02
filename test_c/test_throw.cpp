/***************************************************************************
 * 
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_throw.cpp
 * @author wanguanglu(com@baidu.com)
 * @date 2012/01/09 15:24:33
 * @brief 
 *  
 **/

#include <iostream>

using namespace std;

class Test
{
public:
    Test()
    {
        throw 1;
    }
};

int main(int argc, char* argv[])
{
    try
    {
        Test* t = new Test();
    }catch(...)
    {
        cout<<"Catch it"<<endl;
    }
    return 0;
}




/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
