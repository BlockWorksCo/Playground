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
import Queue
from multiprocessing.managers import BaseManager, NamespaceProxy

from fuse import FUSE, FuseOSError, Operations

from Span import *
import unittest



class Passthrough(Operations, multiprocessing.managers.BaseProxy):


    def __init__(self, root):
        self.root           = root
        self.handles        = {}

        #
        #
        #
        self.requestQ    = multiprocessing.Queue()
        self.responseQ   = multiprocessing.Queue()


    # Helpers
    # =======

    def SynchronousPut(self, rq):
        self.requestQ.put( rq )
        #print('** triggering **')
        try:
            os.statvfs('tmp/q')
        except OSError:
            pass
        #print('** waiting for response **')
        response    = self.responseQ.get()
        #print('** response is %s**'%(response))

        return response

    def ProcessQ(self):
        try:
            time.sleep(0.2)
            rq,data   = self.requestQ.get_nowait()

        
            if rq == 1:
                #print('setting %s'%(data))
                self.handles    = data
                rsp             = True

            elif rq == 2:
                #print('rq 2')
                rsp             = self.handles
                #print('getting %s'%(rsp))
          
            else:
                #print('** bad request [%s] **'%(rq))
                sys.exit(-1)
                rsp             = None

            self.responseQ.put(rsp)
            #print('put response on responseQ %s'%rsp)

        except Queue.Empty:

            #print('trigger with no data')
            pass



    def _full_path(self, partial):
        if partial.startswith("/"):
            partial = partial[1:]
        path = os.path.join(self.root, partial)
        return path

    BLOCK_SIZE  = 1024*4

    def LengthOfSpans(self, spans):
        lastSpan        = spans[-1]
        start,end,t     = lastSpan

        return end


    # Filesystem methods
    # ==================

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
            #print('Trigger')
            self.ProcessQ()
            return {'st_mode':33204, 'st_ino':2, 'st_dev':62, 'st_nlink':1, 'st_uid':1000, 'st_gid':1000, 'st_size':27, 'st_atime':int(time.time()), 'st_mtime':int(time.time()), 'st_ctime':1511394708}
        else:
            
            #print('getattr(%s,%s)'%(path,str(fh)))
            full_path = self._full_path(path)
            st = os.lstat(full_path)
            s= dict((key, getattr(st, key)) for key in ('st_atime', 'st_ctime',
                         'st_gid', 'st_mode', 'st_mtime', 'st_nlink', 'st_size', 'st_uid'))

            if path in self.handles.keys():
                #print('--- in handles ---')
                fh,spans        = self.handles[path]
                #print(spans)
                s['st_size']    = self.LengthOfSpans(spans)
                #print(s)
            else:
                #print('--- not in handles ---')
                pass

            return s

    def readdir(self, path, fh):

        full_path = self._full_path(path)
        #print('** readdir %s **'%(full_path))

        dirents = ['.', '..','q']
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
        #print('statfs')

        full_path = self._full_path(path)
        stv = os.statvfs(full_path)
        
        s=dict((key, getattr(stv, key)) for key in ('f_bavail', 'f_bfree',
            'f_blocks', 'f_bsize', 'f_favail', 'f_ffree', 'f_files', 'f_flag',
            'f_frsize', 'f_namemax'))

        #print('** statfs on %s **'%path)

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

        #print('open of %s'%path)
        full_path = self._full_path(path)
        fh  = os.open(full_path, flags)
    
        length      = os.stat(full_path).st_size
        dataSource  = FileDataSource(fh, 0,length)
        if path in self.handles.keys():

            #
            # Already been opened, lets close the old handle but
            # keep the spans.
            #
            fhOld,spans        = self.handles[path]
            #os.close(fhOld)
        else:

            #
            # New file open, lets make spans for the whole file.
            #
            spans       = [(0,length, dataSource)]

        self.handles[path]    = (fh, spans)

        return 0


    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)


    def read(self, path, length, offset, fh):

        fn,spans    = self.handles[path]

        #print(self.handles)
        if length > self.BLOCK_SIZE:
            length  = self.BLOCK_SIZE

        full_path   = self._full_path(path)
        fileLength  = self.LengthOfSpans(spans)
        #print('file length = %d'%fileLength)

        if offset+length > fileLength:
            length  = fileLength - offset

        if length <= 0:
            #print('EOF @ %d'%(offset))
            return None

        else:

            #print('read %s from %d of %d bytes'%(path,offset,length))
            #print(fh)
            #print( self.handles[path] )

            #os.lseek(fh, offset, os.SEEK_SET)
            #data    = os.read(fh, length)
            #print('offset=%d'%(offset))
            #print('1) length=%d'%(length))
            data    = GetData(spans, offset, offset+length)
            #print(data)

            return data

    def write(self, path, buf, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.write(fh, buf)

    def truncate(self, path, length, fh=None):
        full_path = self._full_path(path)
        with open(full_path, 'r+') as f:
            f.truncate(length)

    def flush(self, path, fh):
        #print('*** flush ****')
        #return os.fsync(fh)
        return 0

    def release(self, path, fh):
        #print('** release %s **'%path)
        fn,spans    = self.handles[path]
        #print('close')
        os.close(fn)
        self.handles.pop(path)
        return 1
        #pass

    def fsync(self, path, fdatasync, fh):
        return self.flush(path, fh)

    def SetHandles(self, handles):
        #print('** SetHandles [%s] **'%(multiprocessing.current_process().name))
        #print(handles)
        self.SynchronousPut( (1,handles) )

    def GetHandles(self):
        #print('** GetHandles **')
        handles = self.SynchronousPut( (2,None) )
        #print('** GetHandles are %s **'%handles)
        return handles


    def RunFUSE(self, fs,mountPoint):
        # nolocalcaches
        FUSE(fs, mountPoint, nothreads=True, foreground=True, attr_timeout=0, entry_timeout=0, negative_timeout=0, direct_io=True)



def StartFUSEThread(fs,mountPoint):

    fs.RunFUSE(fs, mountPoint)


def FUSEThread(fs, mountPoint):
    fs.RunFUSE(fs,mountPoint)



class MyManager(BaseManager): pass

class TestProxy(NamespaceProxy):

    _exposed_ = ('__getattribute__', '__setattr__', '__delattr__', 'SetHandles','GetHandles','StartFUSEThread')

    def SetHandles(self, handles):
        callmethod = object.__getattribute__(self, '_callmethod')
        return callmethod('SetHandles', (handles,))

    def GetHandles(self):
        callmethod = object.__getattribute__(self, '_callmethod')
        return callmethod('GetHandles')

    def StartFUSEThread(self,fs,mountPoint):
        callmethod = object.__getattribute__(fs, '_callmethod')
        return callmethod('StartFUSEThread',(fs,mountPoint) )


MyManager.register('Passthrough', Passthrough, TestProxy)






class TestSpans(unittest.TestCase):

    def test_one(self):

        spans   = []
        f       = open('tmp/SmallTestFile')
        text    = f.read()
        f.close()
        
        self.assertEqual(text, 'abcdefghijklmnopqrstuvwxyz\n' )


    def test_two(self):

        f      = open('tmp/SmallTestFile','r')

        text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
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

        f      = open('tmp/SmallTestFile')

        f.seek(0, os.SEEK_END)
        length  = f.tell()
        f.close()

        self.assertEqual(length, 27)



    def test_four(self):

        f      = open('tmp/SmallTestFile','r')

        text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        ds1     = StringDataSource(text1, 0,len(text1))
        handles = fs.GetHandles()
        fh,spans= handles['/SmallTestFile']
        spans   = InsertSpan(spans, (10,14, ds1) )
        handles['/SmallTestFile']    = (fh,spans)
        fs.SetHandles(handles)

        length  = os.path.getsize('tmp/SmallTestFile')

        f.seek(0,os.SEEK_SET)
        data    = f.read()
        #print(data)

        f.close()

        self.assertEqual(data, 'abcdefghijABCDklmnopqrstuvwxyz\n')
        self.assertEqual(length, 31)



    def test_five(self):

        f      = open('tmp/SmallTestFile','r')

        text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
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

        f      = open('tmp/SmallTestFile','r')

        text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        ds1     = StringDataSource(text1, 0,len(text1))
        handles = fs.GetHandles()
        fh,spans= handles['/SmallTestFile']
        spans   = InsertSpan(spans, (10,14, ds1) )
        handles['/SmallTestFile']    = (fh,spans)
        fs.SetHandles(handles)

        handles = fs.GetHandles()

        f.seek(0,os.SEEK_END)
        length  = f.tell()

        f.close()

        self.assertEqual(length, 31)









def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])
    #pass


if __name__ == '__main__':

    #
    # Make the mount point if its not already there.
    #
    dirName = './tmp'
    try:
        os.mkdir(dirName)
    except OSError:
        pass

    #
    #
    #
    #manager = MyManager()
    #manager.start()

    global fs
    #fs = manager.Passthrough('./TestFiles')
    fs = Passthrough('./TestFiles')

    #
    # Make sure we unmount on exit.
    #
    signal.signal(signal.SIGINT, signal.default_int_handler)
    atexit.register(ExitFunction)

    #
    # Create the fs object and the thread to run it.
    #
    #fs.SetHandles({})
    #fs.StartFUSEThread(fs,'./tmp')
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
            #print('waiting')
            time.sleep(0.1)
    
    #fh  = open('./tmp/SmallTestFile')

    #text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    #ds1     = StringDataSource(text1, 0,len(text1))
    #handles = fs.GetHandles()
    #fh,spans= handles['/SmallTestFile']
    #spans   = InsertSpan(spans, (10,14, ds1) )
    #handles['/SmallTestFile']    = (fh,spans)
    #fs.SetHandles(handles)

    #while True:
    #    time.sleep(1)
    #    print('** main [%s] **'%(multiprocessing.current_process().name))

    #
    # Run the tests.
    #
    try:
        unittest.main()
        print('unittest done')

    except KeyboardInterrupt:
        sys.exit(-1)




