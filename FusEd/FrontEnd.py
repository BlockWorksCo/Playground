



import signal
import curses
import curses.textpad
import os
import time
import FusEd
import threading
import sys
import subprocess


class FrontEnd:
    """
    """


    def __init__(self):
        """
        """
        stdscr = curses.initscr()

        #curses.noecho()
        #curses.echo()

        begin_x = 20
        begin_y = 7
        height = 5
        width = 40
        win = curses.newwin(height, width, begin_y, begin_x)
        tb = curses.textpad.Textbox(win)
        text = tb.edit()
        curses.addstr(4,1,text.encode('utf_8'))

    
    def MainLoop(self):
        """
        """

        hw = "Hello world!"
        while 1:
            c = stdscr.getch()
            if c == ord('p'): 
                pass
            elif c == ord('q'): 
                break # Exit the while()
            elif c == curses.KEY_HOME: 
                x = y = 0

        curses.endwin()







if __name__ == '__main__':

    try:
        os.mkdir('./tmp')
    except OSError:
        pass

    dirName,fileName    = os.path.split(sys.argv[1])

    signal.signal(signal.SIGINT, signal.default_int_handler)

    t = threading.Thread(target=FusEd.FUSEThread, args=(dirName,))
    t.daemon    = True;
    t.start()

    #FrontEnd().Run()
    try:
        while True:
            time.sleep(1)
            print('Tick...')
            #print(open('./tmp/'+fileName).read())

    except KeyboardInterrupt:
        subprocess.Popen(['fusermount','-uz','tmp'])







