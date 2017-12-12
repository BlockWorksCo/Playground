#!/usr/bin/python3



import sys





def GenerateLineIndex(inputName, outputName):

    with open(inputName,'rb') as inF, open(outputName,'wb') as outF:

        outF.write(b'%08x\n'%(0))
        for line in inF:
            offset  = inF.tell()
            outF.write(b'%08x\n'%(offset))



if __name__ == '__main__':

    fileName        = sys.argv[1]
    outputFileName  = sys.argv[2]

    GenerateLineIndex( fileName, outputFileName )

