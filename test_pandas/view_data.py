import pandas as pd
import numpy  as np


print "Create DataFrame Object."
dates = pd.date_range('20130101', periods=6)
print dates
print
print

df = pd.DataFrame(np.random.randn(6,4), index=dates, columns=list('ABCD'))
print df
print
print


print "head of df"
print df.head(2)
print
print

print "tail of df"
print df.tail(2)
print
print

print "index of df"
print df.index
print
print

print "columns of df"
print df.columns
print
print

print "values of df"
print df.values
print
print

print "show statistic summary of df"
print df.describe()
print
print

print "transpos data"
print df.T
print
print

print "sort by an index"
print df.sort_index(axis=1, ascending=False)
print
print

print "sort by value"
print df.sort_values(by="B")
print
print
