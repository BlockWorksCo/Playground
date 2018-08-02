#!/usr/bin/python3



import sys
import os
import unittest
import tempfile
import time
import logging


logger         = logging.getLogger('LineIndex')


def IndexFileNameFromFileName(fileName):
    dirName,fn    = os.path.split(fileName)
    indexFileName   = tempfile.gettempdir()+'/'+fn+'.LineIndex'
    indexFileName   = fn+'.LineIndex'

    return indexFileName


def NumberOfLines(fileName):

    return int(os.path.getsize(IndexFileNameFromFileName(fileName))/9)


def IndexOfLine(fileName, lineNumber):

    #logger.debug(open(IndexFileNameFromFileName(fileName)).read() )

    with open(IndexFileNameFromFileName(fileName),'rb') as inF:
        inF.seek(lineNumber*9, os.SEEK_SET)
        line    = inF.readline()
        return int(line, 16)


def LengthOfLine(fileName, lineNumber):
    index   = IndexOfLine(fileName, lineNumber)
    if lineNumber < NumberOfLines(fileName):
        index2  = IndexOfLine(fileName, lineNumber+1)
    else:
        index2  = os.path.getsize(fileName)
    return index2-index-1


def GenerateLineIndex( fileName ):

    indexFileName   = IndexFileNameFromFileName(fileName)

    logger.debug('Generating line index for %s into %s'%(fileName, indexFileName))

    with open(fileName,'rb') as inF, open(indexFileName,'wb',0) as outF:
        inF.seek(0,os.SEEK_SET)
        outF.seek(0,os.SEEK_SET)
        outF.write(b'%08x\n'%(0))
        lineCount   = 0
        for line in inF:
            offset  = inF.tell()
            outF.write(b'%08x\n'%(offset))
            lineCount   = lineCount+1

        outF.flush()
        outF.close()

    logger.debug('%d lines'%(lineCount))

    #logger.debug(open(IndexFileNameFromFileName(fileName)).read() )

    logger.debug('name: %s'%(IndexFileNameFromFileName(fileName)))
    logger.debug('0: %08x'%(IndexOfLine(fileName, 111)))
    logger.debug('1: %08x'%(IndexOfLine(fileName, 112)))
    logger.debug('2: %08x'%(IndexOfLine(fileName, 113)))
    



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
    



