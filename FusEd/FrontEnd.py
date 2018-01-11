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
import Cursor
from DataSource import *
import logging


count   = 0


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
        self.top    = 0
        self.left   = 0
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

        self.fh         = open('./tmp/MediumSizeFile', 'rb')
        self.RedrawBuffer()

        self.stdscr.move(0,4)


    def RedrawBuffer(self):
    
        handles  = EDFS.GetHandles()
        EDFS.RegenerateLineIndex('tmp/MediumSizeFile')

        self.contentWin.clear()
        numberOfLines   = LineIndex.NumberOfLines(self.fileName)

        bY,bX                   = self.contentWin.getbegyx()

        for i in range(0, self.height-1):

            if self.top+i < 0:
                break

            if self.top+i >= numberOfLines:
                break

            index   = LineIndex.IndexOfLine(self.fileName, self.top+i)
            self.fh.seek(index, os.SEEK_SET)
            line    = self.fh.readline().decode('utf-8').replace('\n','')

            self.leftBorder.addstr(i,0, '%3d'%(i+self.top) )                

            displayLine = '%s'%(line)
            self.contentWin.addstr(i, 0, displayLine[self.left:self.left+self.width])


        self.statusWin.clear()
        self.y, self.x          = self.stdscr.getyx()
        lineLength  = LineIndex.LengthOfLine(self.fileName, self.top+self.y-bY)
        status  = 'pos: %d %d lines: %d length: %d index: %d '%(self.left+self.x-bX, self.top+self.y-bY,LineIndex.NumberOfLines(self.fileName),lineLength, LineIndex.IndexOfLine(self.fileName, self.top+self.y-bY) )
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
        self.y, self.x          = self.stdscr.getyx()

        #
        #
        #
        self.RedrawBuffer()
        self.cursor.SetXY(self.left+self.x-bX, self.top+self.y-bY)

        #
        #
        #
        self.y, self.x          = self.stdscr.getyx()
        self.logger.debug('%d %d %d   %d %d %d '%(self.y,self.top,self.height, self.x,self.left,self.width))

        #
        #
        #
        self.logger.debug('waiting for keypress.')
        c = self.stdscr.getch()
        self.logger.debug('got keypress %d'%(c))

        if c == 0x1b: 
            self.logger.debug('escape pressed... exiting.')
            return False

        elif c == curses.KEY_DC:
            self.cursor.Delete()

        elif c == curses.KEY_BACKSPACE:
            self.cursor.Backspace()

        elif c == curses.KEY_LEFT:
            if self.x >  bX:
                self.x  -= 1
            else:
                if self.left > 0:
                    self.left   -= 1

        elif c == curses.KEY_RIGHT:
            if self.x < self.width+2:
                self.x  += 1
            else:
                self.left   += 1
                self.x      = self.width+2

        elif c == curses.KEY_UP:
            if self.y >  bY:
                self.y  -= 1
            else:
                if self.top > 0:
                    self.top    -= 1

        elif c == curses.KEY_DOWN:
            if self.y < self.height-2:
                self.y  += 1
            else:
                if self.top < LineIndex.NumberOfLines(self.fileName) - self.height:
                    self.top   += 1
                    self.y      = self.height-2

        elif c == curses.KEY_NPAGE:
            self.top   += self.height-1
            if self.top+self.y+(self.height-1) > LineIndex.NumberOfLines(self.fileName):
                self.top    = LineIndex.NumberOfLines(self.fileName) - (self.height-1)
                if self.top < 0:
                    self.top    = 0
            self.y      = self.height-2

        elif c == curses.KEY_PPAGE:
            self.top    -= self.height-1
            if self.top < 0:
                self.top    = 0
            if self.top+self.y < 0:
                self.top    = 0
            self.y      = 0

        elif c == curses.KEY_HOME:
            self.x      = bX
            self.left   = 0

        elif c == curses.KEY_END:
            self.cursor.End()
            EDFS.RegenerateLineIndex('tmp/MediumSizeFile')
            lineLength  = LineIndex.LengthOfLine(self.fileName, self.top+self.y)
            self.x      = lineLength+bX
            self.logger.debug('<< %d >>'%(lineLength))

        else:
            self.cursor.Insert(c)
            self.x      += 1


        #
        #
        #
        self.logger.debug('0) %d %d %d   %d %d %d '%(self.y,self.top,self.height, self.x,self.left,self.width))

        lineLength  = LineIndex.LengthOfLine(self.fileName, self.top+self.y)
        if self.left+self.x-bX > lineLength:
            self.left   = lineLength
            self.x      = bX


        if self.x-bX > self.width:
            self.logger.debug('%d > %d '%(self.x-bX,self.width))
            self.left   = (self.x-bX) - (self.width-6)
            self.x  = self.width+bX-2
            self.logger.debug('-- %d --'%(self.x))

        self.logger.debug('1) %d %d %d   %d %d %d '%(self.y,self.top,self.height, self.x,self.left,self.width))
        self.stdscr.move(self.y, self.x)
        self.logger.debug('2) %d %d %d   %d %d %d '%(self.y,self.top,self.height, self.x,self.left,self.width))

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





