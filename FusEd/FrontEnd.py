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
import LineIndex


count   = 0


class FrontEnd:
    """
    """


    def __init__(self):
        """
        """
        self.stdscr = curses.initscr()
        self.top    = 10
        self.left   = 0

        curses.noecho()
        #curses.echo()

        begin_x = 20
        begin_y = 7
        height = 5
        width = 40
        win = curses.newwin(height, width, begin_y, begin_x)
        tb = curses.textpad.Textbox(win)
        #text = tb.edit()
        #curses.addstr(4,1,text.encode('utf_8'))

        self.stdscr.clear()
        self.stdscr.keypad(True)

        if curses.can_change_color():
            # init_color(n, r, g, b)
            # n=0 is background
            curses.start_color()
            curses.init_color(0, 255, 255, 255)

    def RedrawBuffer(self):
    
        fileName    = 'tmp/MediumSizeFile'
        with open(fileName,'rb') as f:
            self.stdscr.clear()
            for i in range(0, self.height-1):

                index   = LineIndex.IndexOfLine(fileName, self.top+i)
                f.seek(index, os.SEEK_SET)
                line    = f.readline().decode('utf-8').replace('\n','')

                displayLine = '%3d %s'%(i+self.top,line)
                self.stdscr.addstr(i, 0, displayLine[:self.width])


        status  = '%d %d'%(self.left, self.top)
        self.stdscr.addstr(self.height-1, 1, status)

        self.stdscr.refresh()


    def Iterate(self):
        """
        """

        self.height, self.width = self.stdscr.getmaxyx()
        self.RedrawBuffer()

        c = self.stdscr.getch()
        if c == ord('p'): 
            pass
        elif c == ord('q'): 
            return False
        elif c == curses.KEY_UP:
            self.top   = self.top - 1
        elif c == curses.KEY_DOWN:
            self.top   = self.top + 1
        elif c == curses.KEY_NPAGE:
            self.top   = self.top + self.height
        elif c == curses.KEY_PPAGE:
            self.top   = self.top - self.height

        if self.top < 0:
            self.top    = 0

        self.maxLines   = 114
        if self.top+self.height > self.maxLines:
            self.top    = self.maxLines-self.height

        return True




def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])
    curses.endwin()
    




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






