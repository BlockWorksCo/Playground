#!/usr/bin/env python3



import unittest
import os
import time
import logging


class CursorWindow:

    def __init__(self, left,top,width,height, x=0,y=0):
        self.logger     = logging.getLogger('CursorWindow')

        self.left   = left
        self.top    = top
        self.width  = width
        self.height = height

        self.cx     = 0
        self.cy     = 0



    def MoveTo(self, x,y):

        dx  = x - self.cx
        dy  = y - self.cy








class Tests(unittest.TestCase):

    def test_one(self):

        window  = CursorWindow(0,0,100,100, 0,0)
        window.MoveTo(10,10)

        self.assertEqual( window.left,  0 )
        self.assertEqual( window.top,   0 )
        self.assertEqual( window.cx,    0 )
        self.assertEqual( window.cy,    0 )



if __name__ == '__main__':

    unittest.main()
    



