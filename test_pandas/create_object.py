import pandas as pd
import numpy  as np

print "create Series Object."
s = pd.Series([1, 3, 5, np.nan, 6, 8])
print s
print
print

print "Create DataFrame Object."
dates = pd.date_range('20130101', periods=6)
print dates
print
print

df = pd.DataFrame(np.random.randn(6,4), index=dates, columns=list('ABCD'))
print df
print
print

df2 = pd.DataFrame({ 'A' : 1.,
    'B' : pd.Timestamp('20130102'),
    'C' : pd.Series(1,index=list(range(4)),dtype='float32'),
    'D' : np.array([3] * 4,dtype='int32'),
    'E' : pd.Categorical(["test","train","test","train"]),
    'F' : 'foo' })
print df2
print
print

print df2.dtypes
print
print
