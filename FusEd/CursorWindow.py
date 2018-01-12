#!/usr/bin/env python3



import unittest
import os
import time
import logging


class CursorWindow:

    def __init__(self, left,top,width,height):
        self.logger     = logging.getLogger('CursorWindow')

        self.left   = left
        self.top    = top
        self.width  = width
        self.height = height

        self.cx     = 0
        self.cy     = 0



    def MoveRelative(self, dx,dy):
        pass


    def MoveAbsolute(self, x,y):

        self.cx = x
        self.cy = y

        if self.cx >= self.width:
            self.left   = self.cx - self.width
            self.cx     = x - self.left

        if self.cx < self.left:
            self.left   = self.cx
            self.cx     = 0








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
        window.MoveAbsolute(15,15)
        window.MoveAbsolute(5,15)

        self.assertEqual( window.left,  5 )
        self.assertEqual( window.top,   10 )
        self.assertEqual( window.cx,    0 )
        self.assertEqual( window.cy,    15 )



if __name__ == '__main__':

    unittest.main()
    



