import numpy as np

# a step in viterbi, cool

a = [[1, 2], [3, 4]]

b = [1, 1.5]

print np.expand_dims(b, 1)
print

v = np.expand_dims(b, 1) + a
print v
print

print np.max(v, 0)

