

globalX = 1.11
globalY = 2.22

def Oops():
    localsZ = 3.33
    localsA = 4.44
    print("Ooopps...")


def Blaa(a):
    localsZ = 3.33
    localsA = 4.44
    for i in range(10):
        print("Blaa %f %d"%(a,i))
    Oops()
    return 3.14


if __name__ == '__main__':
    print('Sequence One.')
    Blaa(123);
