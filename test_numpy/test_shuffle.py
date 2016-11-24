import numpy as np

if __name__ == "__main__":
    print np.random.permutation(range(10))

    index = np.arange(10)
    np.random.shuffle(index)
    print index
