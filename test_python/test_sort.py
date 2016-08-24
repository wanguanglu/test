# -*- coding:gbk -*-
#/usr/bin/python

import os
import sys

def my_cmp(E1, E2):
    return -cmp(E1[1], E2[1])
        
def score(E):
    return E[1]

#test tuple
test = tuple()
lst = list()

test = ('korean', 2)
lst.append(test)
test = ('china', 1)
lst.append(test)
test = ('japan', 3)
lst.append(test)

#原始用法
#lst.sort(my_cmp)

#现在用法
lst.sort(key=score)

for cont, num in lst:
    print('%s\t%d'%(cont, num))
