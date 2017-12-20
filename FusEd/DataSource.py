#!/usr/bin/env python3



import unittest
import os
import time
import logging


class FileDataSource:

    def __init__(self, fh, rangeStart,rangeEnd):
        #self.logger     = logging.getLogger('FileDataSource')
        self.fh         = fh
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        readStart   = self.rangeStart+offset
        readEnd     = min(readStart+numberOfBytes, self.rangeEnd+offset)

        position    = os.lseek(self.fh, readStart, os.SEEK_SET)
        data        = os.read(self.fh, readEnd-readStart)

        return data

    def CombineWith(self, other):
        self.rangeEnd    = other.rangeEnd

        return self
        

    def SubDataSource(self, rangeStart,rangeEnd):
        
        newSource   = FileDataSource(self.fh, self.rangeStart+rangeStart, self.rangeStart+rangeEnd)
        return newSource



class StringDataSource:

    def __init__(self, text, rangeStart,rangeEnd):
        #self.logger     = logging.getLogger('StringDataSource')
        self.text       = text
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        readStart   = self.rangeStart+offset
        readEnd     = min(readStart+numberOfBytes, self.rangeEnd+offset)

        data        = self.text[readStart:readEnd]

        return data
        
    def CombineWith(self, other):
        self.text        = self.text[self.rangeStart:self.rangeEnd] + other.text[other.rangeStart:other.rangeEnd]
        self.rangeEnd    = self.rangeEnd + (other.rangeEnd-other.rangeStart)

        self.text       = self.text[self.rangeStart:self.rangeEnd]
        self.rangeStart = 0
        self.rangeEnd   = len(self.text)

        return self

    def SubDataSource(self, rangeStart,rangeEnd):
        
        return StringDataSource( self.text[rangeStart:rangeEnd], 0, rangeEnd-rangeStart )



class Tests(unittest.TestCase):

    def test_one(self):
        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))
        data        = dataSource.Read(10,5)
        self.assertEqual(data, 'klmno' )


    def test_two(self):
        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 5,len(text)-5)
        data        = dataSource.Read(10,5)
        self.assertEqual(data, 'pqrst' )


    def test_three(self):
        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 5,len(text)-5)
        data        = dataSource.Read(10,100)
        self.assertEqual(data, 'pqrstuvwxyz' )


    def test_four(self):
        text            = 'abcdefghijklmnopqrstuvwxyz'
        dataSource      = StringDataSource(text, 5,len(text)-5)
        subDataSource   = dataSource.SubDataSource(5,15)
        data            = subDataSource.Read(0,10)
        self.assertEqual(data, 'fghijklmno' )







if __name__ == '__main__':

    unittest.main()
    



