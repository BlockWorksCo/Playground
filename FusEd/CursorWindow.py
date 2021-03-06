#!/usr/bin/env python3



import unittest
import os
import time
import logging


class CursorWindow:
    """
    (cx,cy) is the cursor position.
    left is the column number of the leftmost column.
    top is the line number of the top line.
    absolute position of cursor in file is (left+cx,top+cy).
    """

    def __init__(self, left,top,width,height, numberOfLines):
        self.logger     = logging.getLogger('CursorWindow')

        self.left               = left
        self.top                = top
        self.width              = width
        self.height             = height
        self.numberOfLines      = numberOfLines

        self.cx     = 0
        self.cy     = 0


    def SetNumberOfLines( self, numberOfLines ):
        self.logger.debug('numberofLines=%d'%(numberOfLines))
        self.numberOfLines  = numberOfLines

    def MoveRelative(self, dx,dy):
        pass


    def MoveAbsolute(self, x,y):
        """
        (x,y) are the desired absolute coordinates.
        This function produces modifies left,top,cx,cy such that (left+cx,top+cy) == (x,y)
        """

        if x < 0:
            x   = 0

        if y < 0:
            y   = 0

        if y > self.numberOfLines:
            y   = self.numberOfLines

        #
        # NOTE: We don't know about the numberOfLines in the file here, we should.
        #

        self.logger.debug('1) set absolute %d %d height is %d top is %d'%(x,y,self.height,self.top))
        self.cx = x - self.left
        self.cy = y - self.top

        if self.cx >= (self.width-1):
            self.left   = x - (self.width-1)
            self.cx     = x - self.left

        if self.cx < 0:
            self.left   += self.cx
            self.cx     = 0

        if self.cy >= self.height-1:
            self.top    = y - (self.height-1)
            self.cy     = y - self.top
            self.logger.debug('push top=%d'%(self.top))

        if self.cy < 0:
            self.top    += self.cy
            self.cy     = 0

        self.logger.debug('2) set absolute %d %d height is %d top is %d'%(x,y,self.height,self.top))

    def GetAbsolutePosition(self):

        return (self.left+self.cx, self.top+self.cy)


    def GetCursorPosition(self):

        self.logger.debug('get cursorpos %d %d'%(self.cx,self.cy))
        return (self.cx, self.cy)


    def GetOriginPosition(self):

        return (self.left, self.top)






class Tests(unittest.TestCase):

    def test_one(self):

        window  = CursorWindow(0,0,100,100)
        window.MoveAbsolute(10,10)

        self.assertEqual( window.left,  0 )
        self.assertEqual( window.top,   0 )
        self.assertEqual( window.cx,    10 )
        self.assertEqual( window.cy,    10 )


    def test_two(self):

        window  = CursorWindow(0,0,100,100)
        window.MoveAbsolute(110,10)

        self.assertEqual( window.left,  10 )
        self.assertEqual( window.top,   0 )
        self.assertEqual( window.cx,    100 )
        self.assertEqual( window.cy,    10 )


    def test_three(self):

        window  = CursorWindow(10,10,100,100)
        window.MoveAbsolute(5,15)

        self.assertEqual( window.left,  5 )
        self.assertEqual( window.top,   10 )
        self.assertEqual( window.cx,    0 )
        self.assertEqual( window.cy,    5 )


    def test_four(self):

        window  = CursorWindow(0,0,100,100)
        window.MoveAbsolute(10,110)

        self.assertEqual( window.left,  0 )
        self.assertEqual( window.top,   10 )
        self.assertEqual( window.cx,    10 )
        self.assertEqual( window.cy,    100 )


    def test_five(self):

        window  = CursorWindow(10,10,100,100)
        window.MoveAbsolute(15,5)

        self.assertEqual( window.left,  10 )
        self.assertEqual( window.top,   5 )
        self.assertEqual( window.cx,    5 )
        self.assertEqual( window.cy,    0 )


    def test_six(self):

        window  = CursorWindow(10,10,100,100)
        window.MoveAbsolute(15,15)

        self.assertEqual( window.left,  10 )
        self.assertEqual( window.top,   10 )
        self.assertEqual( window.cx,    5 )
        self.assertEqual( window.cy,    5 )






if __name__ == '__main__':

    unittest.main()
    



