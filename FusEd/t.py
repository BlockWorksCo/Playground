



import time
import multiprocessing
from multiprocessing.managers import BaseManager, NamespaceProxy







class TestClass(object):

    handles = []

    def SetHandles(self,handles):
        self.handles  = handles

    def GetHandles(self):
        return self.handles


def Process(fs):

    while True:
        time.sleep(1.0)
        print('tick...')
        print(fs.handles)





class MyManager(BaseManager): pass

class TestProxy(NamespaceProxy):
    # We need to expose the same __dunder__ methods as NamespaceProxy,
    # in addition to the b method.
    _exposed_ = ('__getattribute__', '__setattr__', '__delattr__', 'SetHandles','GetHandles')

    def SetHandles(self, handles):
        callmethod = object.__getattribute__(self, '_callmethod')
        return callmethod('SetHandles', (handles,))

    def GetHandles(self):
        callmethod = object.__getattribute__(self, '_callmethod')
        return callmethod('GetHandles')

MyManager.register('test', TestClass, TestProxy)



if __name__ == '__main__':

    manager = MyManager()
    manager.start()

    mt = manager.test()
    mt.a = []

    t = multiprocessing.Process(target=Process, args=(mt,) )
    t.daemon    = True;
    t.start()

    mt.SetHandles([1,2,3])
    while True:
        time.sleep(1.0)
        print('tock...')
        ll  = mt.GetHandles()
        ll.append(',')
        mt.SetHandles( ll )

