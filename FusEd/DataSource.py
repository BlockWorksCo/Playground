


import unittest
import os


class FileDataSource:

    def __init__(self, fh, rangeStart,rangeEnd):
        self.fh         = fh
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        readStart   = self.rangeStart+offset
        readEnd     = min(readStart+numberOfBytes, self.rangeEnd+offset)

        os.lseek(self.fh, readStart, os.SEEK_SET)
        #print('offset=%d number=%d'%(offset,numberOfBytes))
        #print('reading %d bytes from %d (%d,%d)'%(numberOfBytes, os.lseek(self.fh,0,os.SEEK_CUR), readStart,readEnd ))
        data        = os.read(self.fh, readEnd-readStart)

        return data
        

    def SubDataSource(self, rangeStart,rangeEnd):
        
        return FileDataSource(self.fh, self.rangeStart+rangeStart, self.rangeStart+rangeEnd)



class StringDataSource:

    def __init__(self, text, rangeStart,rangeEnd):
        self.text       = text
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        readStart   = self.rangeStart+offset
        readEnd     = min(readStart+numberOfBytes, self.rangeEnd+offset)

        data        = self.text[readStart:readEnd]

        return data
        

    def SubDataSource(self, rangeStart,rangeEnd):
        
        return StringDataSource(self.text, self.rangeStart+rangeStart, self.rangeStart+rangeEnd)



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
        self.assertEqual(data, 'klmnopqrst' )







if __name__ == '__main__':

    unittest.main()
    



