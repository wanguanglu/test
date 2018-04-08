#!/usr/bin/python
# -*- coding:utf-8 -*-

def square(x) :            # 计算平方数
  return x ** 2

print map(square, [1,2,3,4,5])   # 计算列表各个元素的平方

print map(lambda x: x ** 2, [1, 2, 3, 4, 5])  # 使用 lambda 匿名函数
 
# 提供了两个列表，对相同位置的列表数据进行相加
print map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10])
