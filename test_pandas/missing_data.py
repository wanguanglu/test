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

df1 = df.reindex(index=dates[0:4], columns=list(df.columns) + ['E'])
df1.loc[dates[0]:dates[1],'E'] = 1
print df1
print
print


print "drop any rows that have missing data"
print df1.dropna(how='any')
print
print

print "filling missing data"
print df1.fillna(value=5)
print
print


print " get the boolean mask where values are nan"
print pd.isnull(df1)
print
print
