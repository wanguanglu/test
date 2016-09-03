import sys
import time
import collections

def test_counter():
    a = ['1', '1', '2', '2', '3', '3', '3']
    counter=collections.Counter(a)
    print counter.most_common(2)

def test_deque():
    test = collections.deque()

    test.append('a')
    test.append('b')
    test.appendleft('c')

    print test
    print test.count('a')

    print test.pop()
    print test.popleft()

    
    fancy_loading = collections.deque('>--------------------')
    
    while True:
        print '\r%s' % ''.join(fancy_loading),
        fancy_loading.rotate(1)
        sys.stdout.flush()
        time.sleep(0.08)


if __name__ == '__main__':
    test_counter()
    test_deque()
