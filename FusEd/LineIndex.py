#!/usr/bin/python3



import sys
import os
import unittest



def NumberOfLines(fileName):

    return os.path.getsize(fileName+'.LineIndex')/9


def IndexOfLine(fileName, lineNumber):

    indexFileName   = fileName+'.LineIndex'

    with open(indexFileName,'rb') as inF:
        inF.seek(int(lineNumber*9), os.SEEK_SET)
        line    = inF.readline()
        return int(line, 16)


def GenerateLineIndex( fileName ):

    indexFileName   = fileName+'.LineIndex'

    with open(fileName,'rb') as inF, open(indexFileName,'wb') as outF:

        outF.write(b'%08x\n'%(0))
        for line in inF:
            offset  = inF.tell()
            outF.write(b'%08x\n'%(offset))



class TestLineIndex(unittest.TestCase):

    def test_one(self):

        fileName    = 'TestFiles/MediumSizeFile'
        GenerateLineIndex( fileName )

        self.assertEqual( IndexOfLine(fileName, 0), 0)
        self.assertEqual( IndexOfLine(fileName, 1), 809)
        self.assertEqual( IndexOfLine(fileName, 2), 810)
        self.assertEqual( IndexOfLine(fileName, 3), 1379)
        self.assertEqual( IndexOfLine(fileName, 4), 1380)
        self.assertEqual( IndexOfLine(fileName, 5), 1769)
        self.assertEqual( IndexOfLine(fileName, 6), 1770)
        self.assertEqual( IndexOfLine(fileName, 7), 2293)
        self.assertEqual( IndexOfLine(fileName, 8), 2294)
        self.assertEqual( IndexOfLine(fileName, 9), 3028)



if __name__ == '__main__':

    unittest.main()
    



