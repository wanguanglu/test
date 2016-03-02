/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_long.cpp
 * @author wanguanglu(com@baidu.com)
 * @date 2011/12/27 10:45:11
 * @brief 
 *  
 **/

#include "stdio.h"

int main(int argc, char* argv[])
{
    long la = 0;
    printf("%d.\n", sizeof(long));

    int ia = 0x7FFFFFFF;
    printf("%d.\n", ia);

    la = ia*8;
    printf("%ld.\n", la);

    la = ((long)ia)*8;
    printf("%ld.\n", la);

    return 0;
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
