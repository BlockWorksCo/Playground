#!/usr/bin/env python3


import time
import glob
import sys
import signal
import atexit
import subprocess
import threading
import os
import errno
import multiprocessing
import queue
from multiprocessing.managers import BaseManager, NamespaceProxy
import LineIndex
import logging

from fuse import FUSE, FuseOSError, Operations

from Span import *
import unittest




class LineIndexFile:
    """
    Current index of all lines in file.
    """

    def __init__(self):
        self.numberOfLines  = 100000

    def length(self):
        return self.numberOfLines * 9

    def read(self, path, length, offset, fh):

        startLineNumber = int(offset/9)
        stopLineNumber = int(offset+length/9)+1
        adjustedOffset  = startLineNumber*9
        adjust          = offset - adjustedOffset

        # form the buffer
        readBuffer  = b''.join([b'%08x\n'%(num) for num in range(startLineNumber,stopLineNumber)])
        
        # take the right portion of the buffer.
        if offset < self.length():
            bytesToRead = min(length, self.length()-offset)
            return readBuffer[(adjust+adjustedOffset):(adjust+bytesToRead)]
        else:
            return None


class BufferFile:
    """
    Current editor file state.
    """

    def __init__(self):
        pass

    def length(self):
        return 20

    def read(self, path, length, offset, fh):

        if offset < self.length():
            bytesToRead = min(length, self.length()-offset)
            return b'b'*bytesToRead
        else:
            return None


class PatchFile:
    """
    current span-set as a patch file.
    """

    def __init__(self):
        pass

    def length(self):
        return 30

    def read(self, path, length, offset, fh):

        if offset < self.length():
            bytesToRead = min(length, self.length()-offset)
            return b'c'*bytesToRead
        else:
            return None


class OperationsFile:
    """
    list of operations for undo buffer.
    """

    def __init__(self):
        pass

    def length(self):
        return 40

    def read(self, path, length, offset, fh):

        if offset < self.length():
            bytesToRead = min(length, self.length()-offset)
            return b'd'*bytesToRead
        else:
            return None


class Passthrough(Operations, multiprocessing.managers.BaseProxy):


    def __init__(self, root):

        self.logger         = logging.getLogger('EDFS')
        self.root           = root
        self.handles        = {}

        #
        #
        #
        self.requestQsend,self.requestQrecv     = multiprocessing.Pipe()
        self.responseQsend,self.responseQrecv   = multiprocessing.Pipe()


    # Helpers
    # =======

    def SynchronousPut(self, rq):
        self.requestQsend.send( rq )
        try:
            #time.sleep(0.01)
            os.stat('tmp/q')
        except OSError:
            pass
        response    = self.responseQrecv.recv()

        return response


    def ProcessQ(self):
        if self.requestQrecv.poll() == True:
            rq,data   = self.requestQrecv.recv()
        
            if rq == 1:
                self.handles    = data
                rsp             = True

            elif rq == 2:
                rsp             = self.handles
          
            elif rq == 3:
                LineIndex.GenerateLineIndex(data)
                rsp             = True
                  
            else:
                sys.exit(-1)
                rsp             = None

            self.responseQsend.send(rsp)

        else:

            pass



    def _full_path(self, partial):
        if partial.startswith("/"):
            partial = partial[1:]
        path = os.path.join(self.root, partial)
        return path

    def LengthOfSpans(self, spans):
        lastSpan        = spans[-1]
        start,end,t     = lastSpan

        return end


    # Filesystem methods
    # ==================

    lineIndexFile   = LineIndexFile()
    bufferFile      = BufferFile()
    patchFile       = PatchFile()
    opsFile         = OperationsFile()

    exposedFileList = ['LineIndex','Patch','Ops','Buffer']
    exposedPathList = ['/LineIndex','/Patch','/Ops','/Buffer']
    exposedFile =   {
                        '/LineIndex':lineIndexFile,
                        '/Patch':patchFile,
                        '/Ops':opsFile,
                        '/Buffer':bufferFile,
                    }

    def access(self, path, mode):
        full_path = self._full_path(path)
        if not os.access(full_path, mode):
            raise FuseOSError(errno.EACCES)

    def chmod(self, path, mode):
        full_path = self._full_path(path)
        return os.chmod(full_path, mode)

    def chown(self, path, uid, gid):
        full_path = self._full_path(path)
        return os.chown(full_path, uid, gid)

    def getattr(self, path, fh=None):

        if path == '/q':
            self.ProcessQ()
            return {'st_mode':33204, 'st_ino':2, 'st_dev':62, 'st_nlink':1, 'st_uid':1000, 'st_gid':1000, 'st_size':27, 'st_atime':int(time.time()), 'st_mtime':int(time.time()), 'st_ctime':1511394708}

        elif path in self.exposedPathList:
            return {'st_mode':33204, 'st_ino':2, 'st_dev':62, 'st_nlink':1, 'st_uid':1000, 'st_gid':1000, 'st_size':self.exposedFile[path].length(), 'st_atime':int(time.time()), 'st_mtime':int(time.time()), 'st_ctime':1511394708}

        else:
            
            full_path = self._full_path(path)
            st = os.lstat(full_path)
            s= dict((key, getattr(st, key)) for key in ('st_atime', 'st_ctime',
                         'st_gid', 'st_mode', 'st_mtime', 'st_nlink', 'st_size', 'st_uid'))

            if path in list(self.handles.keys()):
                fh,spans        = self.handles[path]
                s['st_size']    = self.LengthOfSpans(spans)
            else:
                pass

            return s

    def readdir(self, path, fh):

        full_path = self._full_path(path)

        dirents = ['.', '..','q'] + self.exposedFileList
        if os.path.isdir(full_path):
            dirents.extend(os.listdir(full_path))

        for r in dirents:
            yield r

    def readlink(self, path):
        pathname = os.readlink(self._full_path(path))
        if pathname.startswith("/"):
            # Path name is absolute, sanitize it.
            return os.path.relpath(pathname, self.root)
        else:
            return pathname

    def mknod(self, path, mode, dev):
        return os.mknod(self._full_path(path), mode, dev)

    def rmdir(self, path):
        full_path = self._full_path(path)
        return os.rmdir(full_path)

    def mkdir(self, path, mode):
        return os.mkdir(self._full_path(path), mode)

    def statfs(self, path):

        full_path = self._full_path(path)
        stv = os.statvfs(full_path)
        
        s = dict((key, getattr(stv, key)) for key in ('f_bavail', 'f_bfree',
            'f_blocks', 'f_bsize', 'f_favail', 'f_ffree', 'f_files', 'f_flag',
            'f_frsize', 'f_namemax'))

        return s

    def unlink(self, path):
        return os.unlink(self._full_path(path))

    def symlink(self, name, target):
        return os.symlink(target, self._full_path(name))

    def rename(self, old, new):
        return os.rename(self._full_path(old), self._full_path(new))

    def link(self, target, name):
        return os.link(self._full_path(name), self._full_path(target))

    def utimens(self, path, times=None):
        return os.utime(self._full_path(path), times)

    # File methods
    # ============

    def open(self, path, flags):

        if path in self.exposedPathList:
            return 0
        else:
            full_path = self._full_path(path)
            fh  = os.open(full_path, flags)
        
            length      = os.stat(full_path).st_size
            dataSource  = FileDataSource(fh,full_path, 0,length)
            if path in list(self.handles.keys()):

                #
                # Already been opened, lets close the old handle but
                # keep the spans.
                #
                fhOld,spans        = self.handles[path]
            else:

                #
                # New file open, lets make spans for the whole file.
                #
                spans       = [(0,length, dataSource)]

            self.handles[path]    = (fh, spans)

            #
            # Generate the line index
            #
            #LineIndex.GenerateLineIndex(full_path)

            return 0


    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)


    def read(self, path, length, offset, fh):

        if path in self.exposedPathList:

            return self.exposedFile[path].read(path,length,offset,fh)

        else:

            fn,spans    = self.handles[path]

            origin      = spans[0][0]

            data    = GetData(spans, offset+origin, offset+length+origin)
            if len(data) > length:
                data    = data[:length]

            return data

    def write(self, path, buf, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.write(fh, buf)

    def truncate(self, path, length, fh=None):
        full_path = self._full_path(path)
        with open(full_path, 'r+') as f:
            f.truncate(length)

    def flush(self, path, fh):
        return 0

    def release(self, path, fh):

        if path in self.exposedPathList:

            return 1

        else:

            fn,spans    = self.handles[path]
            os.close(fn)
            return 1

    def fsync(self, path, fdatasync, fh):
        return self.flush(path, fh)

    def SetHandles(self, handles):
        self.SynchronousPut( (1,handles) )
        self.SynchronousPut( (3,fileName) )

    def GetHandles(self):
        handles = self.SynchronousPut( (2,None) )
        return handles


    def RunFUSE(self, fs,mountPoint):
        # nolocalcaches
        FUSE(fs, mountPoint, nothreads=True, foreground=True, attr_timeout=0, entry_timeout=0, negative_timeout=0, direct_io=True)



def StartFUSEThread(fs,mountPoint):

    fs.RunFUSE(fs, mountPoint)


def FUSEThread(fs, mountPoint):
    fs.RunFUSE(fs,mountPoint)



def SetHandles(handles):
    fs.SynchronousPut( (1,handles) )

def GetHandles():
    handles = fs.SynchronousPut( (2,None) )
    return handles










class TestFUSE(unittest.TestCase):


    def ResetState(self):
        fs.SetHandles({})

    def test_one(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            spans   = []
            text    = f.read()
            f.close()
            
            self.assertEqual(text, b'abcdefghijklmnopqrstuvwxyz\n' )


    def test_two(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (10,14, ds1) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            f.seek(0,os.SEEK_END)
            length  = f.tell()

            f.seek(0,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(length, 31)



    def test_three(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            f.seek(0, os.SEEK_END)
            length  = f.tell()
            f.close()

            self.assertEqual(length, 27)



    def test_four(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (10,14, ds1) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            length  = os.path.getsize('tmp/SmallTestFile')

            f.seek(0,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'abcdefghijABCDklmnopqrstuvwxyz\n')
            self.assertEqual(length, 31)



    def test_five(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (10,14, ds1) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            f.seek(0,os.SEEK_END)
            length  = f.tell()

            f.close()

            self.assertEqual(length, 31)



    def test_six(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (0,4, ds1) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            length  = os.path.getsize('tmp/SmallTestFile')

            f.seek(0,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'ABCDabcdefghijklmnopqrstuvwxyz\n')
            self.assertEqual(length, 31)




    def test_seven(self):

        self.ResetState()

        with open('tmp/EmptyFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/EmptyFile']
            spans   = InsertSpan(spans, (0,4, ds1) )
            handles['/EmptyFile']    = (fh,spans)
            fs.SetHandles(handles)

            handles = fs.GetHandles()
            fh,spans= handles['/EmptyFile']
            spans   = InsertSpan(spans, (4,8, ds1) )
            handles['/EmptyFile']    = (fh,spans)
            fs.SetHandles(handles)

            length  = os.path.getsize('tmp/EmptyFile')

            f.seek(0,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'ABCDABCD')
            self.assertEqual(length, 8)




    def test_eight(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            length1  = os.path.getsize('tmp/MediumSizeFile')

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (33133,33138, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            length2  = os.path.getsize('tmp/MediumSizeFile')

            f.seek(33112,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'<End Of original fileABCDE>\n')
            self.assertEqual(length1, 33135)
            self.assertEqual(length2, 33140)




    def test_nine(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            length1  = os.path.getsize('tmp/MediumSizeFile')

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = AddSpan(spans, (33130,33133, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            length2  = os.path.getsize('tmp/MediumSizeFile')

            f.seek(33112,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'<End Of original fABC>\n')
            self.assertEqual(length1, 33135)
            self.assertEqual(length2, 33135)




    def test_ten(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            length1  = os.path.getsize('tmp/MediumSizeFile')

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = AddSpan(spans, (33130,33133, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (100,110, ds2) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            length2  = os.path.getsize('tmp/MediumSizeFile')

            f.seek(33122,os.SEEK_SET)
            data1    = f.read()

            f.seek(95,os.SEEK_SET)
            data2    = f.read(20)

            f.close()

            self.assertEqual(data1, b'<End Of original fABC>\n')
            self.assertEqual(data2, b'orbi ABCDEFGHIJet vi')
            self.assertEqual(length1, 33135)
            self.assertEqual(length2, 33145)




    def test_eleven(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            length1  = os.path.getsize('tmp/MediumSizeFile')

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = AddSpan(spans, (33130,33133, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'0123456789'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (33131,33135, ds2) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            length2  = os.path.getsize('tmp/MediumSizeFile')

            f.seek(33125,os.SEEK_SET)
            data1    = f.read()

            f.close()

            self.assertEqual(data1, b'nal fA0123BC>\n')
            self.assertEqual(length1, 33135)
            self.assertEqual(length2, 33139)



    def test_twelve(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            length1  = os.path.getsize('tmp/MediumSizeFile')

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = AddSpan(spans, (33130,33133, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'0123456789'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (33131,33141, ds2) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            length2  = os.path.getsize('tmp/MediumSizeFile')

            f.seek(33125,os.SEEK_SET)
            data1    = f.read()

            f.close()

            self.assertEqual(data1, b'nal fA0123456789BC>\n')
            self.assertEqual(length1, 33135)
            self.assertEqual(length2, 33145)





    def test_thirteen(self):

        self.ResetState()

        with open('tmp/SmallTestFile','rb') as f:

            text1   = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (0,4, ds1) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'0123456789'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/SmallTestFile']
            spans   = InsertSpan(spans, (31,35, ds2) )
            handles['/SmallTestFile']    = (fh,spans)
            fs.SetHandles(handles)

            length  = os.path.getsize('tmp/SmallTestFile')

            f.seek(0,os.SEEK_SET)
            data    = f.read()

            f.close()

            self.assertEqual(data, b'ABCDabcdefghijklmnopqrstuvwxyz\n0123')
            self.assertEqual(length, 35)





    def test_fourteen(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            for i in range(15):
                text1   = b'[a]'
                ds1     = StringDataSource(text1, 0,len(text1))
                handles = fs.GetHandles()
                fh,spans= handles['/MediumSizeFile']
                spans   = InsertSpan(spans, (0,3, ds1) )
                handles['/MediumSizeFile']    = (fh,spans)
                fs.SetHandles(handles)

            f.seek(0,os.SEEK_SET)
            data    = f.read(50)

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']

            f.close()

            self.assertEqual(data, b'[a][a][a][a][a][a][a][a][a][a][a][a][a][a][a]Lorem')
            #self.assertEqual(length, 35)




    def test_fifteen(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            text1   = b'a'
            ds1     = StringDataSource(text1, 0,len(text1))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (0,1, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'a'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (1,2, ds1) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            f.seek(0,os.SEEK_SET)
            data    = f.read(50)

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']

            f.close()

            #self.assertEqual(data, b'[a][a][a][a][a][a][a][a][a][a][a][a][a][a][a]Lorem')
            #self.assertEqual(length, 35)






    def test_sixteen(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = RemoveData(spans, 0,1 )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            f.seek(0,os.SEEK_SET)
            data    = f.read(10)

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']

            f.close()

            self.assertEqual(data, b'orem ipsum')
            #self.assertEqual(length, 10)




    def test_seventeen(self):

        self.ResetState()

        with open('tmp/MediumSizeFile','rb') as f:

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = RemoveData(spans, 0,1 )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'a'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (1,2, ds2) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            text2   = b'a'
            ds2     = StringDataSource(text2, 0,len(text2))
            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']
            spans   = InsertSpan(spans, (1,2, ds2) )
            handles['/MediumSizeFile']    = (fh,spans)
            fs.SetHandles(handles)

            f.seek(0,os.SEEK_SET)
            data    = f.read(10)

            handles = fs.GetHandles()
            fh,spans= handles['/MediumSizeFile']

            f.close()

            self.assertEqual(data, b'aaorem ips')








def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])



def RunEDFS():

    #
    # Make the mount point if its not already there.
    #
    dirName = './tmp'
    try:
        os.mkdir(dirName)
    except OSError:
        pass

    global fs
    fs = Passthrough('./TestFiles')

    #
    # Make sure we unmount on exit.
    #
    signal.signal(signal.SIGINT, signal.default_int_handler)
    atexit.register(ExitFunction)

    #
    # Create the fs object and the thread to run it.
    #
    t = multiprocessing.Process(target=StartFUSEThread, args=(fs,'./tmp') )
    t.daemon    = True;
    t.start()

    #
    # Wait until FUSE is up and running.
    #
    dirContents = []
    while dirContents == []:
        try:
            dirContents = os.listdir('./tmp')
        except OSError:
            dirContents = []
            time.sleep(0.1)
    


if __name__ == '__main__':

    RunEDFS()

    #
    # Run the tests.
    #
    try:
        unittest.main()

    except KeyboardInterrupt:
        sys.exit(-1)




