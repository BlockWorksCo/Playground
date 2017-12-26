#!/usr/bin/env python3

import logging
import EDFS
import LineIndex




class Cursor:

    def __init__(self, fileName):
        self.logger         = logging.getLogger('Cursor')
        self.fileName       = fileName
        self.x              = 0
        self.y              = 0


    def Left(self):
        if self.x > 0:
            self.x          = self.x - 1


    def Right(self):
        self.x          = self.x + 1


    def Up(self):
        if self.y > 0:
            self.y      = self.y - 1


    def Down(self):
        if self.y < LineIndex.NumberOfLines(self.fileName):
            self.y      = self.y + 1


    def PageUp(self, n):
        pass


    def PageDown(self, n):
        pass


    def Home(self):
        self.x      = 0
        self.left   = 0


    def End(self):
        index   = LineIndex.IndexOfLine(self.fileName, self.top+self.y)
        self.fh.seek(index, os.SEEK_SET)
        line    = self.fh.readline().decode('utf-8').replace('\n','')
        self.x      = len(line)


    def Insert(self):
        text    = ('%c'%c).encode('utf-8')
        ds      = StringDataSource(text, 0,len(text))

        handles = EDFS.GetHandles()
        fh,spans= handles['/MediumSizeFile']
        origin  = spans[0][0]
        offset  = origin + LineIndex.IndexOfLine( 'tmp/MediumSizeFile', self.y ) + self.x
        spans   = EDFS.InsertSpan(spans, (offset,offset+len(text), ds) )
        handles['/MediumSizeFile']    = (fh,spans)
        EDFS.SetHandles(handles)
        EDFS.RegenerateLineIndex('tmp/MediumSizeFile')

        self.x  = self.x + len(text1)


    def Add(self):
        pass


    def Backspace(self):
        self.x          = self.x - 1

        handles = EDFS.GetHandles()
        fh,spans= handles['/MediumSizeFile']
        origin  = spans[0][0]
        offset  = origin + LineIndex.IndexOfLine( 'tmp/MediumSizeFile', self.y ) + self.x
        spans   = EDFS.RemoveData(spans, offset,offset+1 )
        handles['/MediumSizeFile']    = (fh,spans)
        EDFS.SetHandles(handles)
        EDFS.RegenerateLineIndex('tmp/MediumSizeFile')


    def Delete(self):
        handles = EDFS.GetHandles()
        fh,spans= handles['/MediumSizeFile']
        origin  = spans[0][0]
        offset  = origin + LineIndex.IndexOfLine( 'tmp/MediumSizeFile', self.y ) + self.x
        spans   = EDFS.RemoveData(spans, offset,offset+1 )
        handles['/MediumSizeFile']    = (fh,spans)
        EDFS.SetHandles(handles)
        EDFS.RegenerateLineIndex('tmp/MediumSizeFile')




    def check(self):

        #
        #
        #
        if self.top < 0:
            self.top    = 0

        if self.left < 0:
            self.left    = 0

        self.maxLines   = LineIndex.NumberOfLines(self.fileName)

        if self.top+self.height > self.maxLines:
            self.top    = self.maxLines-self.height

        if self.top < 0:
            self.top    = 0

        #print('** LineNumber %d + %d, %d **'%(self.top,self.y, self.maxLines))
        index   = LineIndex.IndexOfLine(self.fileName, self.top+self.y)
        self.fh.seek(index, os.SEEK_SET)
        line    = self.fh.readline().decode('utf-8').replace('\n','')
        if self.x > len(line):
            self.x  = len(line)



if __name__ == '__main__':
    pass

