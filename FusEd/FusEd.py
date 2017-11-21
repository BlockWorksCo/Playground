#!/usr/bin/env python


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
        full_path = self._full_path(path)
        st = os.lstat(full_path)
        return dict((key, getattr(st, key)) for key in ('st_atime', 'st_ctime',
                     'st_gid', 'st_mode', 'st_mtime', 'st_nlink', 'st_size', 'st_uid'))

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
        return dict((key, getattr(stv, key)) for key in ('f_bavail', 'f_bfree',
            'f_blocks', 'f_bsize', 'f_favail', 'f_ffree', 'f_files', 'f_flag',
            'f_frsize', 'f_namemax'))

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
        full_path = self._full_path(path)
        fh  = os.open(full_path, flags)
    
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

        text3    = 'Scooby Dooby Doo.'
        ds3 = StringDataSource(text3, 0,len(text3))
        self.spansForFile[fh]   = InsertSpan(self.spansForFile[fh], (2000,2000+len(text3), ds3) )

        return fh

    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)

    def read(self, path, length, offset, fh):

        print('\noffset=%d'%offset)
        if length > self.BLOCK_SIZE:
            length  = self.BLOCK_SIZE

        full_path = self._full_path(path)
        fileLength      = os.stat(full_path).st_size
        if offset+length > fileLength:
            length  = fileLength - offset

        if length <= 0:
            print('EOF @ %d'%(offset))
            return None
        else:

            print('read %s from %d of %d bytes'%(path,offset,length))

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


def FUSEThread(root):
    FUSE(Passthrough(root), './tmp', nothreads=True, foreground=True)



class TestSpans(unittest.TestCase):

    def test_one(self):

        spans   = []
        self.assertEqual(spans, [] )




def ExitFunction():
    subprocess.Popen(['fusermount','-uz','tmp'])


if __name__ == '__main__':

    dirName = './tmp'
    try:
        os.mkdir(dirName)
    except OSError:
        pass

    signal.signal(signal.SIGINT, signal.default_int_handler)
    atexit.register(ExitFunction)

    t = threading.Thread(target=FUSEThread, args=(dirName,))
    t.daemon    = True;
    t.start()

    try:
        unittest.main()

    except KeyboardInterrupt:
        sys.exit(-1)




