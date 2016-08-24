#/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import os
import re



pattern = re.compile('\\[err:(\\-{0,1}\\d+)\\]')

str = 'NOTICE: 11-01 01:01:15:  ts * 1294027104 [ts.cpp:234][tid:1294027104][err:0][pkg_num:6][logid:595160830[595160830 595160830 595160830 595160830 595160830 595160830 ]][pdid:2][decoder:10.81.24.33:8001][retry:0[]][usr:1911914280, 3197117425][client:c522110bd0a8703c0764ed756ac9b283d53dad03||][cpid:22372228][cplen:9540][tsp:1288544471][rn:5[[0|ºÚÆïÊ¿|815][1|ºÚÆé|96][2|ºÚË¿|87][3|ºÚÉ«|0][4|ºÚÆï|0]]][timeuse(ms):[conn2Dec:0][st->wrAllData:2278][st->endDec:2496][st->wrAgent:2496][storeCorpus:3][workTime:2500]]'

str2 = '[err:-99]'

m = pattern.search(str)
if m != None:
    err_no = m.group(1)
    print(err_no)

    match_str=m.group(0)
    print(match_str)

m = pattern.match(str)
if m==None:
    print('not match')

m = pattern.match(str2)
if m != None:
    print(m.group(1))
