import pandas as pd
import numpy  as np

tuples = list(zip(*[['bar', 'bar', 'baz', 'baz',
                     'foo', 'foo', 'qux', 'qux'],
                     ['one', 'two', 'one', 'two',
                     'one', 'two', 'one', 'two']]))
print "tuples"
print tuples

index = pd.MultiIndex.from_tuples(tuples, names=['first', 'second'])
df = pd.DataFrame(np.random.randn(8, 2), index=index, columns=['A', 'B'])
print "df"
print df
print
print

print "stacked"
stacked = df.stack()
print stacked
print
print

print "unstack"
print stacked.unstack()
print
print

print stacked.unstack(0)
print
print

print stacked.unstack(1)
print
print


df = pd.DataFrame({'A' : ['one', 'one', 'two', 'three'] * 3,
                   'B' : ['A', 'B', 'C'] * 4,
                   'C' : ['foo', 'foo', 'foo', 'bar', 'bar', 'bar'] * 2,
                   'D' : np.random.randn(12),
                   'E' : np.random.randn(12)})
print df

print "pivot table"
print pd.pivot_table(df, values='D', index=['A', 'B'], columns=['C'])
print
print

print "time series"
rng = pd.date_range('1/1/2012', periods=10, freq='S')
print rng
ts = pd.Series(np.random.randint(0, 500, len(rng)), index=rng)
print ts
print
print ts.resample('5Min').sum()
