


import unittest
from Span import *




def AddOverlay(overlays, offset,data):

    overlays    = AddSpan(overlays, (offset,offset+len(data),data) )

    return overlays



def GetData(base,overlays, start,numberOfBytes):

    end = start+numberOfBytes

    numberOfBytesCopied = 0
    position            = start
    data                = ''
    while numberOfBytesCopied < numberOfBytes:
        span    = SpanAtPoint(overlays, position)
        if span == None:
            spanStart   = 0
            spanEnd     = len(base)
            spanData    = base
        else:
            spanStart,spanEnd,spanData  = span

        print('spanStart=%d'%spanStart)
        print('spanEnd=%d'%spanEnd)
        print('spanData=%s'%spanData)

        numberOfBytesToCopy = min(numberOfBytes,spanEnd-spanStart)
        data                += spanData[position-spanStart:position-spanStart+numberOfBytesToCopy]
        numberOfBytesCopied += numberOfBytesToCopy
        position            += numberOfBytesToCopy

    return data
    




class TestSpans(unittest.TestCase):

    def test_one(self):

        overlays    = []
        overlays    = AddOverlay(overlays, 10, '01234')
        self.assertEqual(overlays, [(10, 15, '01234')] )


    def test_two(self):

        overlays    = []
        overlays    = AddOverlay(overlays, 10, '01234')
        base        = 'abcdefghijklmnopqrstuvwxyz'
        result      = GetData(base,overlays, 0,26)
        self.assertEqual(result, 'abcdefghij01234pqrstuvwxyz' )








if __name__ == '__main__':

    unittest.main()
    



