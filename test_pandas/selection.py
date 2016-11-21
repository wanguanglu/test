import pandas as pd import numpy  as np


print "Create DataFrame Object."
dates = pd.date_range('20130101', periods=6)
print dates
print
print

df = pd.DataFrame(np.random.randn(6,4), index=dates, columns=list('ABCD'))
print df
print
print

print "get A"
print df["A"]
print
print

print "another function of get A"
print df.A
print
print

print "[start, end)"
print df[1:3]
print
print

print "[st_idx, ed_idx]"
print df["20130101" : "20130103"]
print
print

print "select by label"
print df.loc[dates[0]]
print
print

print df.loc["20130101"]
print
print

print "select by multi-axis"
print df.loc[:, ["A", "B"]]
print
print

print "label slicing"
print df.loc["20130103":"20130104", ["A", "B"]]
print
print


print "reduction"
print df.loc["20130103", ["A", "B"]]
print
print

print "scalar value"
print df.loc["20130103", "A"]
print
print

## selection by position
print "selection by position"
print df.iloc[3]
print
print

print df.iloc[3:5, 0:2]
print
print

print df.iloc[[1, 2, 4], [0, 2]]
print
print

print df.iloc[1:3, :]
print
print

print df.iloc[:, 1:3]
print
print


print df.iloc[0, 0]
print
print

print df.iat[0, 0]
print
print


## boolean indexing
print "boolean indexing"
print df[df["A"]>0]
print
print

print df[df.A > 0]
print
print

print df [df > 0]
print
print

## isin
print "isin() filtering"
df2 = df.copy()
df2["E"] = ["one", "two", "three", "four", "five", "six"]
print df2

print df2[df2['E'].isin(['two','four'])]
print
print
