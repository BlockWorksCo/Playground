

import glob
import os
import stat
import sys
import filecmp
import shutil


directoryA       = '/home/ubuntu/Playground/RootA'
directoryB       = '/home/ubuntu/Playground/RootB'
stagingDirectory = '/home/ubuntu/Playground/StagingArea'

directoryA       = sys.argv[1]
directoryB       = sys.argv[2]
stagingDirectory = sys.argv[3]
patchScript      = open(stagingDirectory+'/Patch.sh','w+')
patchScript.write('#!/bin/sh\n')
patchScript.write('set -xe\n')

#
# Obtain the set of files that exist in both directories.
#
filesInA = [file[len(directoryA)+1:] for file in glob.iglob(directoryA+'/**',recursive=True) if os.path.isfile(file)==True]
filesInB = [file[len(directoryB)+1:] for file in glob.iglob(directoryB+'/**',recursive=True) if os.path.isfile(file)==True]
filesToCompare  = list(set(filesInA).union(filesInB))


#
# Compare the files
#
matches, mismatches, errors = filecmp.cmpfiles(directoryA, directoryB, filesToCompare)

#
# Ignore the matches
# 'tar' the mismatches and error-files that exist in B.
# Create an 'rm' set for those files that exist in A and not B.
#
filesToRemove   = list(set(filesInA).intersection(errors))
filesToAdd      = list(set(filesInB).intersection(errors))
filesToCopy     = mismatches + filesToAdd

#
# Copy the filesToCopy to the StagingArea.
#
for file in filesToCopy:
    sourcePath = directoryB+'/'+file
    destPath   = stagingDirectory+'/'+file

    if os.path.isfile(sourcePath) == True and os.path.isdir(sourcePath) == False and os.path.islink(sourcePath) == False:
        path    = os.path.dirname(destPath)
        try:
            os.makedirs(path)
        except FileExistsError:
            pass
        shutil.copy2(sourcePath, destPath)
        patchScript.write('mkdir -p $1/%s \n'%(os.path.dirname(file)) )
        patchScript.write('cp -p -f ./%s $1/%s\n'%(file,file) )

    if os.path.islink(sourcePath) == True:
        target  = os.readlink(sourcePath)
        path    = os.path.dirname(destPath)[len(stagingDirectory)+1:]
        patchScript.write('ln -s $1/%s $1/%s\n'%(path+'/'+target,file) )

#
# Add the remove lines to the script
#
for file in filesToRemove:
    patchScript.write('rm -f $1/%s\n'%(file) )



