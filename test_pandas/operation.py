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

print "get mean of column"
print df.mean()
print
print

print "get mean of row"
print df.mean(1)
print
print


print "sub"
s = pd.Series([1,3,5,np.nan,6,8], index=dates).shift(2)
print s
print
print

print df.sub(s, axis="index")
print
print


print "apply function"
print df.apply(np.cumsum)
print
print

print df.apply(lambda x: x.max() - x.min())
print
print


print "histogramming"
s = pd.Series(np.random.randint(0, 10, size=100))
print s.value_counts()
print
print

print "string method"
s = pd.Series(['A', 'B', 'C', 'Aaba', 'Baca', np.nan, 'CABA', 'dog', 'cat'])
print s.str.lower()
print
print


print "concat"
pieces = [df[:1], df[3:5], df[7:]]
print pd.concat(pieces)
print
print

print "join"
left = pd.DataFrame({'key': ['foo', 'foo'], 'lval': [1, 2]})
print "left"
print left
right = pd.DataFrame({'key': ['foo', 'foo'], 'rval': [4, 5]})
print "right"
print right
print pd.merge(left, right, on='key')
print
print

print "another join"
left = pd.DataFrame({'key': ['foo', 'bar'], 'lval': [1, 2]})
print "left"
print left
right = pd.DataFrame({'key': ['foo', 'bar'], 'rval': [4, 5]})
print "right"
print right
print pd.merge(left, right, on='key')
print
print


print "append"
df = pd.DataFrame(np.random.randn(8, 4), columns=['A','B','C','D'])
s  = pd.DataFrame(np.random.randn(1, 4), columns=['A','B','C','D'])
print "df"
print df
print "s"
print s
print df.append(s, ignore_index=True)
print
print

print "grouping"
df = pd.DataFrame({'A' : ['foo', 'bar', 'foo', 'bar',
                   'foo', 'bar', 'foo', 'foo'],
                   'B' : ['one', 'one', 'two', 'three',
                   'two', 'two', 'one', 'three'],
                   'C' : np.random.randn(8),
                   'D' : np.random.randn(8)})
print "df"
print df
print "group by A"
print df.groupby('A').sum()
print "group by A B"
print df.groupby(['A', 'B']).sum()
print
