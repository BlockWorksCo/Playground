#!/usr/bin/env python3




import atexit
import signal
import curses
import os
import time
import EDFS
import threading
import sys
import subprocess
import LineIndex
import Cursor
import logging
import CursorWindow


class FrontEnd:
    """
    """


    def __init__(self, fileName):
        """
        """
        self.logger         = logging.getLogger('FrontEnd')
        self.cursor         = Cursor.Cursor(fileName)

        self.stdscr = curses.initscr()
        curses.def_shell_mode()
        self.fileName    = 'tmp/MediumSizeFile'

        curses.noecho()

        screenHeight, screenWidth = self.stdscr.getmaxyx()

        self.statusWin  = curses.newwin( 1,screenWidth, screenHeight-1,0)
        self.leftBorder = curses.newwin( screenHeight-0,4, 0,0)
        self.contentWin = curses.newwin( screenHeight, screenWidth-3, 0,4)

        self.height, self.width = self.contentWin.getmaxyx()
        bY,bX                   = self.contentWin.getbegyx()
        self.cursorWindow       = CursorWindow.CursorWindow(0,0,self.width-1,self.height-1)

        self.stdscr.clear()
        self.stdscr.keypad(True)

        if curses.can_change_color():
            # init_color(n, r, g, b)
            # n=0 is background
            curses.start_color()
            curses.init_color(0, 255, 255, 255)

        self.fh         = open('./tmp/MediumSizeFile', 'rb')
        self.RedrawBuffer()

        self.stdscr.move(0,4)


    def RedrawBuffer(self):
    
        handles  = EDFS.GetHandles()
        EDFS.RegenerateLineIndex('tmp/MediumSizeFile')

        #self.logger.debug(open('MediumSizeFile.LineIndex').read() )

        self.contentWin.clear()
        numberOfLines   = LineIndex.NumberOfLines(self.fileName)

        bY,bX                   = self.contentWin.getbegyx()

        for i in range(0, self.cursorWindow.height):

            if self.cursorWindow.top+i < 0:
                break

            if self.cursorWindow.top+i >= numberOfLines:
                break

            index   = LineIndex.IndexOfLine(self.fileName, self.cursorWindow.top+i)
            self.fh.seek(index, os.SEEK_SET)
            line    = self.fh.readline().decode('utf-8').replace('\n','')

            self.leftBorder.addstr(i,0, '%3d'%(i+self.cursorWindow.top) )                

            displayLine = '%s'%(line)
            self.contentWin.addstr(i, 0, displayLine[self.cursorWindow.left:self.cursorWindow.left+self.cursorWindow.width])


        self.statusWin.clear()
        self.y, self.x          = self.stdscr.getyx()
        lineLength  = LineIndex.LengthOfLine('tmp/MediumSizeFile', self.cursorWindow.top+self.cursorWindow.cy-bY)
        status  = 'pos: %d %d lines: %d length: %d index: %d '%(self.cursorWindow.left+self.cursorWindow.cx, self.cursorWindow.top+self.cursorWindow.cy,LineIndex.NumberOfLines(self.fileName),lineLength, LineIndex.IndexOfLine(self.fileName, self.cursorWindow.top+self.cursorWindow.cy) )
        self.statusWin.addstr(0,0, status)

        self.leftBorder.refresh()
        self.contentWin.refresh()
        self.statusWin.refresh()
        self.stdscr.refresh()


    def Iterate(self):
        """
        """


        self.height, self.width = self.contentWin.getmaxyx()
        bY,bX                   = self.contentWin.getbegyx()

        #
        #
        #
        self.RedrawBuffer()
        self.cursor.SetXY(self.cursorWindow.left+self.x-bX, self.cursorWindow.top+self.y-bY)

        #
        #
        #
        ax,ay   = self.cursorWindow.GetAbsolutePosition()

        #
        #
        #
        c = self.stdscr.getch()

        if c == 0x1b: 
            self.logger.debug('escape pressed... exiting.')
            return False

        elif c == curses.KEY_DC:
            self.cursor.Delete()

        elif c == curses.KEY_BACKSPACE:
            self.cursor.Backspace()
            if ax > 0:
                ax  -= 1

        elif c == curses.KEY_LEFT:
            if ax > 0:
                ax  -= 1

        elif c == curses.KEY_RIGHT:
            ax  += 1

        elif c == curses.KEY_UP:
            if ay > 0:
                ay  -= 1

        elif c == curses.KEY_DOWN:
            ay  += 1

        elif c == curses.KEY_NPAGE:
            ay  += self.height

        elif c == curses.KEY_PPAGE:
            ay  -= self.height

        elif c == curses.KEY_HOME:
            ax  = 0

        elif c == curses.KEY_END:
            lineLength  = LineIndex.LengthOfLine(self.fileName, self.cursorWindow.top+self.y)
            ax      = lineLength
            self.logger.debug('<< %d >>'%(lineLength))

        else:
            ax  += 1
            self.cursor.Insert(c)

        #
        #
        #
        self.cursorWindow.MoveAbsolute(ax,ay)
        cx,cy   = self.cursorWindow.GetCursorPosition()
        self.logger.debug('%d %d   %d %d   %d %d '%(cx,cy,self.cursorWindow.left,self.cursorWindow.top,self.cursorWindow.width,self.cursorWindow.height))
        self.stdscr.move(cy+bY,cx+bX)

        return True




def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])
    curses.endwin()
    pass
    




if __name__ == '__main__':

    try:
        os.mkdir('./tmp')
    except OSError:
        pass

    dirName,fileName    = os.path.split(sys.argv[1])


    EDFS.RunEDFS()

    logging.getLogger('FrontEnd').addHandler(logging.FileHandler('FrontEnd.log'))
    logging.getLogger('Spans').addHandler(logging.FileHandler('Spans.log'))
    logging.getLogger('FileDataSource').addHandler(logging.FileHandler('FileDataSource.log'))
    logging.getLogger('StringDataSource').addHandler(logging.FileHandler('StringDataSource.log'))
    logging.getLogger('EDFS').addHandler(logging.FileHandler('EDFS.log'))
    logging.getLogger('LineIndex').addHandler(logging.FileHandler('LineIndex.log'))
    logging.getLogger('Cursor').addHandler(logging.FileHandler('Cursor.log'))
    logging.getLogger('CursorWindow').addHandler(logging.FileHandler('CursorWindow.log'))

    logging.getLogger('FrontEnd').setLevel(logging.DEBUG)
    logging.getLogger('Spans').setLevel(logging.DEBUG)
    logging.getLogger('FileDataSource').setLevel(logging.DEBUG)
    logging.getLogger('StringDataSource').setLevel(logging.DEBUG)
    logging.getLogger('LineIndex').setLevel(logging.DEBUG)
    logging.getLogger('Cursor').setLevel(logging.DEBUG)
    logging.getLogger('CursorWindow').setLevel(logging.DEBUG)

    logging.getLogger('FrontEnd').info('Starting up')

    frontEnd    = FrontEnd(fileName)

    try:
        while frontEnd.Iterate() == True:
            pass

        curses.endwin()
        curses.reset_shell_mode()

    except KeyboardInterrupt:
        sys.exit(-1)
        curses.reset_shell_mode()

    curses.reset_shell_mode()





