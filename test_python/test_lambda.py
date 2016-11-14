
g = lambda x: x*x

print g(3.0)

i = range(10)
a = zip(i, map(g, i))

print a
