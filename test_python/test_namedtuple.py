#!/usr/bin/python
# -*- coding:utf-8 -*-

import collections

coordinate = collections.namedtuple('Coordinate', ['x', 'y'])  
co = coordinate(10,20)  
print co.x,co.y  
print co[0],co[1]  
co = coordinate._make([100,200])  
print co.x,co.y  
co = co._replace(x = 30)  
print co.x,co.y  


websites = [  
    ('Sohu', 'http://www.google.com/', u'张朝阳'),  
    ('Sina', 'http://www.sina.com.cn/', u'王志东'),  
    ('163', 'http://www.163.com/', u'丁磊')  
]  
              
Website = collections.namedtuple('Website', ['name', 'url', 'founder'])  

for website in websites:  
  website = Website._make(website)  
  print website  
