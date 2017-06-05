import numpy as np

label_array = [1, 2, 3, 4]

a = dict()
a['a'] = 0
a['b'] = 1

#label_array = np.array(label_array)

print np.array(label_array == a['b'], float)

# np.full (size, value, type)
a = np.full(10, 3, dtype=np.int32)
print a
