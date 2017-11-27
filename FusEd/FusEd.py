#!/usr/bin/env python


import time
import glob
import sys
import signal
import atexit
import subprocess
import threading
import os
import errno

from fuse import FUSE, FuseOSError, Operations

from Span import *
import unittest



class Passthrough(Operations):


    def __init__(self, root):
        self.root           = root
        self.spansForFile   = {}
        self.fnMap          = {}

    # Helpers
    # =======

    def _full_path(self, partial):
        if partial.startswith("/"):
            partial = partial[1:]
        path = os.path.join(self.root, partial)
        return path

    BLOCK_SIZE  = 1024*4
    openFiles   = {}

    def AddBlockListForFile(self, fh):
        """
        The overlay block list contains blocks of data that overlay the on-disk data.
        """
        self.openFiles[fh]   = {} 

    def AddBlockForFile(self, fh, blockIndex, data):
        """
        Add an overlay block for the file at the specified location.
        """
        self.openFiles[fh][blockIndex]  = data


    def GetBlockForFile(self, fh, blockIndex):
        """
        """
        if blockIndex in self.openFiles[fh]:
            return self.openFiles[fh]
        else:
            os.lseek(fh, blockIndex*self.BLOCK_SIZE, os.SEEK_SET)
            return os.read(fh, self.BLOCK_SIZE)
             

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
        print('getattr(%s,%s)'%(path,str(fh)))
        full_path = self._full_path(path)
        st = os.lstat(full_path)
        s= dict((key, getattr(st, key)) for key in ('st_atime', 'st_ctime',
                     'st_gid', 'st_mode', 'st_mtime', 'st_nlink', 'st_size', 'st_uid'))

        print(self.fnMap.keys())
        if path in self.fnMap.keys():
            fh  = self.fnMap[path]
            spans   = self.spansForFile[fh]
            lastSpan    = spans[-1]
            start,end,t       = lastSpan
            s['st_size'] = end
            print(s)
        else:
            print('not in map')

        return s

    def readdir(self, path, fh):
        full_path = self._full_path(path)

        dirents = ['.', '..']
        if os.path.isdir(full_path):
            dirents.extend(os.listdir(full_path))
        for r in dirents:
            yield r,

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
        
        s=dict((key, getattr(stv, key)) for key in ('f_bavail', 'f_bfree',
            'f_blocks', 'f_bsize', 'f_favail', 'f_ffree', 'f_files', 'f_flag',
            'f_frsize', 'f_namemax'))
        print('statfs on %s'%path)

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

        print('open of %s'%path)
        full_path = self._full_path(path)
        fh  = os.open(full_path, flags)
        self.fnMap[path]    = fh
        print(self.fnMap.keys())
    
        length                  = os.stat(full_path).st_size
        dataSource              = FileDataSource(fh, 0,length)
        self.spansForFile[fh]   = [(0,length, dataSource)]

        #
        #
        #
        #text1    = 'Hello World'
        #ds1 = StringDataSource(text1, 0,len(text1))
        #self.spansForFile[fh]   = InsertSpan(self.spansForFile[fh], (10,10+len(text1), ds1) )

        #text2    = 'Hello Mars'
        #ds2 = StringDataSource(text2, 0,len(text2))
        #self.spansForFile[fh]   = InsertSpan(self.spansForFile[fh], (100,100+len(text2), ds2) )

        #text3    = 'Scooby Dooby Doo.'
        #ds3 = StringDataSource(text3, 0,len(text3))
        #self.spansForFile[fh]   = InsertSpan(self.spansForFile[fh], (2000,2000+len(text3), ds3) )

        return fh

    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)

    def read(self, path, length, offset, fh):

        #print('\noffset=%d'%offset)
        if length > self.BLOCK_SIZE:
            length  = self.BLOCK_SIZE

        full_path = self._full_path(path)
        fileLength      = os.stat(full_path).st_size
        #print('filelength = %d'%fileLength)
        if offset+length > fileLength:
            length  = fileLength - offset

        if length <= 0:
            #print('EOF @ %d'%(offset))
            return None
        else:

            #print('read %s from %d of %d bytes'%(path,offset,length))

            #os.lseek(fh, offset, os.SEEK_SET)
            #data    = os.read(fh, length)
            data    = GetData(self.spansForFile[fh], offset, offset+length)

            return data

    def write(self, path, buf, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.write(fh, buf)

    def truncate(self, path, length, fh=None):
        full_path = self._full_path(path)
        with open(full_path, 'r+') as f:
            f.truncate(length)

    def flush(self, path, fh):
        return os.fsync(fh)

    def release(self, path, fh):
        return os.close(fh)

    def fsync(self, path, fdatasync, fh):
        return self.flush(path, fh)


def FUSEThread(fs,mountPoint):
    FUSE(fs, mountPoint, nothreads=False, foreground=True)







class TestSpans(unittest.TestCase):

    def _test_one(self):

        spans   = []
        text    = open('tmp/SmallTestFile').read()
        
        self.assertEqual(text[:26], 'abcdefghijklmnopqrstuvwxyz' )


    def _test_two(self):

        fh      = open('tmp/SmallTestFile')
        fn      = fs.fnMap['/SmallTestFile']
        
        text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        ds1     = StringDataSource(text1, 0,len(text1))
        fs.spansForFile[fn]   = InsertSpan(fs.spansForFile[fn], (10,14, ds1) )

        fh.seek(0, os.SEEK_SET)
        data    = fh.read(30)

        #print(fs.spansForFile[fn][2][2].rangeStart)
        #print(fs.spansForFile[fn][2][2].rangeEnd)
        #print(fs.spansForFile[fn][2][0])
        #print(fs.spansForFile[fn][2][1])

        self.assertEqual(data, 'abcdefghijABCDklmnopqrstuvwxyz' )


    def _test_three(self):

        fh      = open('tmp/SmallTestFile')
        fn      = fs.fnMap['/SmallTestFile']

        #length  = os.path.getsize('tmp/SmallTestFile')
        length  = os.lseek(fn, 0, os.SEEK_END)

        self.assertEqual(length, 27)



    def test_four(self):

        print('test four')
        fh      = os.open('tmp/SmallTestFile', os.O_RDONLY)
        #fh      = open('tmp/SmallTestFile','r')
        print('test four end')
        #fn      = fs.fnMap['/SmallTestFile']

        #text1   = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        #ds1     = StringDataSource(text1, 0,len(text1))
        #fs.spansForFile[fn]   = InsertSpan(fs.spansForFile[fn], (10,14, ds1) )

        #length  = os.path.getsize('tmp/SmallTestFile')
        #length  = fh.seek(0, os.SEEK_END)
        #length  = os.lseek(fn, 0, os.SEEK_END)
        os.close(fh)

        #self.assertEqual(length, 31)









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
    # Make sure we unmount on exit.
    #
    signal.signal(signal.SIGINT, signal.default_int_handler)
    atexit.register(ExitFunction)

    #
    # Create the fs object and the thread to run it.
    #
    global fs
    fs  = Passthrough('./TestFiles')
    t = threading.Thread(target=FUSEThread, args=(fs,'./tmp'))
    t.daemon    = True;
    t.start()

    #
    # Run the tests.
    #
    try:
        time.sleep(1.0)
        #print(glob.glob('tmp/*'))
        unittest.main()

    except KeyboardInterrupt:
        sys.exit(-1)




