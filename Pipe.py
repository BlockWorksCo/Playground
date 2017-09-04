

import os
import sys


FIFO = '/var/run/mypipe'


try:
    os.mkfifo(FIFO)
except:
    pass



while True:
    print('waiting for writer')
    with open(FIFO) as fifo:
        print('opened')
        while True:
            line = fifo.read()
            if len(line) > 0:
                print(line)
            else:
                break


