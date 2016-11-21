def my_generator():
    for i in range(10):
        yield i, i*i


for i, j in my_generator():
    print i, j
