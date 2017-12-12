#!/usr/bin/python3



import sys
import os
import unittest



def IndexOfLine(fileName, lineNumber):

    indexFileName   = fileName+'.LineIndex'

    with open(indexFileName,'rb') as inF:
        inF.seek(lineNumber*9, os.SEEK_SET)
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



if __name__ == '__main__':

    unittest.main()
    



