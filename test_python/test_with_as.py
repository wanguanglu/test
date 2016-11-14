import sys


class test:  
    def __enter__(self):  
        print("enter")  
        return 1  
    
    def __exit__(self, *args):  
        print("exit")  
        return True  

with test() as t:  
    print("t is not the result of test(), it is __enter__ returned")  
    print("t is 1, yes, it is {0}".format(t))  

    raise NameError("Hi there")  

    print("Never here")  

print
print

class test2:  
    def __enter__(self):  
        print("enter")  
        return self

    def __str__(self):
        return "test2"
    
    def __exit__(self, *args):  
        print("exit")  
        return True  

with test2() as t:  
    print t
