a = ['china', 'japan', 'korean']

print zip(a)

print zip(a, range(len(a)) )

print zip(a, range(len(a)), range(1, len(a)) )

print zip(a, range(len(a)), range(1, len(a)+1) )

dic = dict(zip(a, range(len(a)) ))
print dic
