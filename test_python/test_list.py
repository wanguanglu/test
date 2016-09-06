import os
import sys


if __name__ == '__main__':
    list1 = ['physics', 'chemistry', 1997, 2000]
    list2 = [1, 2, 3, 4, 5 ]
    
    print list1[0]
    print list1[:2]
    
    list1.append('math')
    print list1

    print 'length of list1 is %d'%len(list1)

    if 'math' in list1:
        print 'math is in list1'
        print 'count of math in list1 is %d'%list1.count('math')

    print
    print 'items in list1'
    for item in list1:
        print item
    print

    print("test enumerate")
    for x,y in enumerate(list1):
        print(x, y)
