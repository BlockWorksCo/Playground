#!/usr/bin/env python3




import atexit
import signal
import curses
import curses.textpad
import os
import time
import EDFS
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
        #text = tb.edit()
        #curses.addstr(4,1,text.encode('utf_8'))

    
    def Iterate(self):
        """
        """

        c = stdscr.getch()
        if c == ord('p'): 
            pass
        elif c == ord('q'): 
            return False
        elif c == curses.KEY_HOME: 
            x = y = 0

        return True




def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])
    




if __name__ == '__main__':

    try:
        os.mkdir('./tmp')
    except OSError:
        pass

    dirName,fileName    = os.path.split(sys.argv[1])

    EDFS.RunEDFS()

    frontEnd    = FrontEnd()
    try:
        while frontEnd.Iterate() == True:
            pass

        curses.endwin()

    except KeyboardInterrupt:
        sys.exit(-1)






