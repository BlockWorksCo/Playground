#!/usr/bin/python3



import multiprocessing
import threading
import os
import time



def Thread(q):

    while True:
        print('thread(%d)...'%i2)
        #time.sleep(1)



def Process(q):

    global i2
    i2  = -1

    t = threading.Thread(target=Thread, args=(q,) )
    t.daemon    = True;
    t.start()

    while True:
        #print('process (%d)...'%i2)
        i2  = q.get()



q   = multiprocessing.Queue()

t = multiprocessing.Process(target=Process, args=(q,) )
t.daemon    = True;
t.start()

i   = 0
while True:
    q.put(i)
    print('main(%d)...'%i)
    #time.sleep(1)
    i = i+1


