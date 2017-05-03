import numpy as np

a = [[1.1, 1.2, 1.3],
    [2.1,  2.2, 2.3]]

b = [ [1.4, 1.5],
      [2.4, 2.5]]

c = [ [1.6],
      [1.7]
    ]

print np.concatenate([a, b, c], axis=1)
