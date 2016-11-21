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

#df.to_csv("df.csv")

df2 = pd.read_csv("df2.csv", skip_blank_lines=False)
print df2
print

for i in range(len(df2)):
    print df2[i:i+1]
print

df2[df2 == np.nan] = -1
print df2
