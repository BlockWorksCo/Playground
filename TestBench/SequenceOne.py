

import time
import zlib

globalX = 1.11
globalY = 2.22

def Oops():
    localsZ = 3.33
    localsA = 4.44
    print("Ooopps...")


def StepTwo():
    print('Step Two! part 1.')
    print('Step Two! part 2.')
    print('Step Two! part 3.')


def Blaa(a):
    localsZ = 3.33
    localsA = 4.44
    for i in range(10):
        time.sleep(0.5)
        StepTwo()
        print("Blaa %f %d"%(a,i))
        Oops()
    return 3.14


if __name__ == '__main__':
    print('Sequence One.')
    Blaa(123);
