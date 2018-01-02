#!/usr/bin/python3



import sys
import os
import unittest
import tempfile
import time
import logging


logger         = logging.getLogger('LineIndex')

def NumberOfLines(fileName):

    dirName,fn    = os.path.split(fileName)
    indexFileName   = tempfile.gettempdir()+'/'+fn+'.LineIndex'
    return os.path.getsize(indexFileName)/9


def IndexOfLine(fileName, lineNumber):

    dirName,fn    = os.path.split(fileName)
    indexFileName   = tempfile.gettempdir()+'/'+fn+'.LineIndex'

    with open(indexFileName,'rb') as inF:
        inF.seek(int(lineNumber*9), os.SEEK_SET)
        line    = inF.readline()
        return int(line, 16)


def LengthOfLine(fileName, lineNumber):
    index   = IndexOfLine(fileName, lineNumber)
    with open(fileName, 'rb') as fh:
        fh.seek(index, os.SEEK_SET)
        line    = fh.readline().decode('utf-8').replace('\n','')
    return len(line)


def GenerateLineIndex( fileName ):

    dirName,fn    = os.path.split(fileName)
    indexFileName   = tempfile.gettempdir()+'/'+fn+'.LineIndex'

    logger.debug('Generating line index for %s'%(fileName))

    with open(fileName,'rb') as inF, open(indexFileName,'wb') as outF:
        inF.seek(0,os.SEEK_SET)
        outF.seek(0,os.SEEK_SET)
        outF.write(b'%08x\n'%(0))
        lineCount   = 0
        for line in inF:
            offset  = inF.tell()
            #print('[%d]'%offset)
            outF.write(b'%08x\n'%(offset))
            lineCount   = lineCount+1

    logger.debug('%d lines'%(lineCount))



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
    



