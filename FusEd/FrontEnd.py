



import curses
import os
import time
import FusEd
import threading
import sys





if __name__ == '__main__':

    try:
        os.mkdir('./tmp')
    except OSError:
        pass

    dirName,fileName    = os.path.split(sys.argv[1])

    t = threading.Thread(target=FusEd.FUSEThread, args=(dirName,))
    t.daemon    = True;
    t.start()

    while True:
        time.sleep(1)
        print('Tick...')
        #print(open('./tmp/'+fileName).read())







