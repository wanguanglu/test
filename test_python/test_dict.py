#!/usr/bin/env python3
"""
dict is an common struct in python to store key/value pair

"""


def traverse(dic):
    for name, tel in dic.items():
        print('name:%s\ttelephone:%s' % (name, tel))
    print('\n')


tel_dic = {
    'wanguanglu': '13426128392',
    'dongrunsha': '13426128396',
    'zhouxian': '13426128391'
}

#traverse items
print('traverse items')
traverse(tel_dic)

#add item
print('add items')
tel_dic['zhangsan'] = '13326583450'
traverse(tel_dic)

#del item
print('del items')
del tel_dic['zhangsan']
traverse(tel_dic)

#traverse keys
print('traverse keys')
for key in tel_dic.keys():
    print(key)
print('\n')

print('traverse values')
for value in tel_dic.values():
    print(value)
print('\n')

#copy dict
print('copy dict')
y = tel_dic.copy()
traverse(y)

#haskey
print('test haskey')
print('whether key \'wanguanglu\' exist in dict')
print(tel_dic.has_key('wanguanglu'))
print('whether key \'zhangsan\' exist in dic')
print(tel_dic.has_key('zhangsan'))
print('\n')

#iterator_items
for it in tel_dic.iteritems():
    print(it)
print('\n')

#setdefault
tel_dic.setdefault('zhangsan', 'NULL')
print(tel_dic['zhangsan'])

reverse = dict(zip(tel_dic.values(), tel_dic.keys()))
print(reverse)
print()
traverse(reverse)
print()

#dict clear
tel_dic.clear()
traverse(tel_dic)
print(tel_dic)
