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
        self.top    = 0
        self.left   = 0
        self.x      = 0
        self.y      = 0
        self.fileName    = 'tmp/MediumSizeFile'

        curses.noecho()

        screenHeight, screenWidth = self.stdscr.getmaxyx()

        self.statusWin  = curses.newwin( 1,screenWidth, screenHeight-1,0)
        self.leftBorder = curses.newwin( screenHeight-0,4, 0,0)
        self.contentWin = curses.newwin( screenHeight, screenWidth-3, 0,4)

        self.height, self.width = self.contentWin.getmaxyx()

        self.stdscr.clear()
        self.stdscr.keypad(True)

        if curses.can_change_color():
            # init_color(n, r, g, b)
            # n=0 is background
            curses.start_color()
            curses.init_color(0, 255, 255, 255)

        self.RedrawBuffer()


    def RedrawBuffer(self):
    
        with open(self.fileName,'rb') as f:

            self.contentWin.clear()

            for i in range(0, self.height-1):

                index   = LineIndex.IndexOfLine(self.fileName, self.top+i)
                f.seek(index, os.SEEK_SET)
                line    = f.readline().decode('utf-8').replace('\n','')

                self.leftBorder.addstr(i,0, '%3d'%(i+self.top) )                

                displayLine = '%s'%(line)
                self.contentWin.addstr(i, 0, displayLine[:self.width])


        self.statusWin.clear()
        status  = 'pos: %d %d lines: %d'%(self.left, self.top,LineIndex.NumberOfLines(self.fileName))
        self.statusWin.addstr(0,0, status)

        self.leftBorder.refresh()
        self.contentWin.refresh()
        self.statusWin.refresh()
        self.stdscr.refresh()


    def Iterate(self):
        """
        """

        self.height, self.width = self.contentWin.getmaxyx()
        bY,bX   = self.contentWin.getbegyx()

        self.RedrawBuffer()

        c = self.stdscr.getch()
        if c == ord('p'): 
            pass
        elif c == ord('q'): 
            return False
        elif c == curses.KEY_UP:
            if self.y > 0:
                self.y      = self.y - 1
            else:
                self.top    = self.top - 1
        elif c == curses.KEY_DOWN:
            if self.y < self.height-2:
                self.y      = self.y + 1
            else:
                self.top    = self.top + 1
        elif c == curses.KEY_NPAGE:
            self.top   = self.top + self.height
        elif c == curses.KEY_PPAGE:
            self.top   = self.top - self.height

        if self.top < 0:
            self.top    = 0

        self.maxLines   = LineIndex.NumberOfLines(self.fileName)

        if self.top+self.height > self.maxLines:
            self.top    = self.maxLines-self.height

        self.stdscr.move(bY+self.y, bX+self.x)

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






