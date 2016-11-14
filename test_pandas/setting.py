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

print "setting new column"
s1 = pd.Series([1,2,3,4,5,6], index=pd.date_range('20130102', periods=6))
df["E"] = s1
print df
print
print

print "setting values by label"
df.at["20130101", "A"] = 0
print df
print
print

print "setting values by position"
df.iat[0, 0] = 1.0
print df
print
print


print "assign by numpy array"
df.loc[:,"D"] = np.array([5] * len(df))
print df
print
print

print "where operation"
df2 = df.copy()
df2[df2 > 0] = -df2
print df2
