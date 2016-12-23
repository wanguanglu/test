import numpy as np

a = [[1, 2], [2, 3]]
b = np.array(a)
print b
print

print np.expand_dims(b, -1)
print

print np.expand_dims(a, -1)


