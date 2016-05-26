



def Oops():
    print("Ooopps...")


def Blaa(a):
    for i in range(10):
        print("Blaa %f %d"%(a,i))
    Oops()
    return 3.14


if __name__ == '__main__':
    print('Sequence One.')
    Blaa(123);
