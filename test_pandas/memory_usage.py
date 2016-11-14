import pandas as pd
import numpy  as np

dtypes = ['int64', 'float64', 'datetime64[ns]', 'timedelta64[ns]',
          'complex128', 'object', 'bool']
n    = 5000
data = dict([ (t, np.random.randint(100, size=n).astype(t)) for t in dtypes])
df   = pd.DataFrame(data)

df['categorical'] = df['object'].astype('category')

print "simple memory info."
df.info()

print
print

print "deep memeory info."
df.info(memory_usage="deep")

print
print

print "each column memory."
print df.memory_usage()

print
print

print "total memory usage."
print df.memory_usage().sum()
