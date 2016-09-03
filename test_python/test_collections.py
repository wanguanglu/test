
import collections

def test_counter():
    a = ['1', '1', '2', '2', '3', '3', '3']
    counter=collections.Counter(a)
    print counter.most_common(2)


if __name__ == '__main__':
    test_counter()
