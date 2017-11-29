from multiprocessing.managers import BaseManager, NamespaceProxy

class TestClass(object):
    def __init__(self, a):
        self.a = a

    def b(self):
        print(self.a)

class MyManager(BaseManager): pass

class TestProxy(NamespaceProxy):
    # We need to expose the same __dunder__ methods as NamespaceProxy,
    # in addition to the b method.
    _exposed_ = ('__getattribute__', '__setattr__', '__delattr__', 'b')

    def b(self):
        callmethod = object.__getattribute__(self, '_callmethod')
        return callmethod('b')

MyManager.register('test', TestClass, TestProxy)

if __name__ == '__main__':
    manager = MyManager()
    manager.start()
    t = TestClass(1)
    print( t.a)
    mt = manager.test(2)
    print(mt.a)
    mt.a = 5
    mt.b()

