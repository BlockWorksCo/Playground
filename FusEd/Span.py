#!/usr/bin/env python3
"""

          ----- -------- --------     ---------

          +++++ ++       ++++++++++++ +++++++++


"""



import unittest
from DataSource import *
import logging


logger         = logging.getLogger('Spans')

def SpanAtPoint(spans, position):

    for tS,tE,t in spans:
        if tS<=position and tE>position:
            return (tS,tE,t)

    return None


def SplitAtPosition(spans, position):

    newSpans    = []
    for tS,tE,t in spans:

        if tS<=position and tE>=position:
            if type(t).__name__ == 'str':
                tA    = t
                tB    = t
            else:
                tA    = t.SubDataSource(0, position-tS)
                tB    = t.SubDataSource(position-tS, tE-tS)
            newSpans.append( (tS,position,tA) )
            newSpans.append( (position,tE,tB) )

        else:
            newSpans.append( (tS,tE,t) )

    return newSpans


def RemoveSpansCoveredBy(spans, s,e):
    
    newSpans    = []
    for tS,tE,t in spans:

        if tS<s or tE>e:
            newSpans.append( (tS,tE,t) )
    
    return newSpans



def OrderedInsert(spans, span):
    
    if spans == []:
        newSpans    = [span]

    else:
        newSpans    = []
        s,e,t0      = span
        inserted    = False
        for tS,tE,t1 in spans:

            if e == tS and inserted == False:
                newSpans.append(span)
                inserted    = True

            newSpans.append( (tS,tE,t1) )

            if tE == s and inserted == False:
                newSpans.append(span)
                inserted    = True

    return newSpans


def ReduceSpans(spans):

    for tS,tE,t in spans:
        if tS == tE:

            #
            # Zero-width span, remove it.
            #
            spans.remove( (tS,tE,t) )

    numberOfMods    = 1
    while numberOfMods > 0:
        
        newSpans        = []
        numberOfSpans   = len(spans)
        numberOfMods    = 0

        for i in range(numberOfSpans):

            tS1,tE1,t1 = spans[i]

            if i<numberOfSpans-1:
                tS2,tE2,t2 = spans[i+1]
                
                if tE1 == tS2 and type(t1) == type(t2) and i<numberOfSpans-1 and t1.CanCombineWith(t2) == True:

                    #
                    # Adjoining spans with the same type of data Source
                    #
                    if type(t1) == str:
                        newSpans.append( (tS1,tE2, t1+t2) )
                    else:
                        p1              = t1.SubDataSource(0,tE1-tS1)
                        p2              = t2.SubDataSource(0,tE2-tS2)
                        combinedSpan    = p1.CombineWith(p2)
                        newSpans.append( (tS1,tE2, combinedSpan) )
                    #
                    # We've modified the list, copy the rest of the list and start again...
                    #
                    numberOfMods    += 1
                    newSpans        = newSpans + spans[i+2:]
                    break

                else:
                    newSpans.append( (tS1,tE1,t1) )

            else:
                newSpans.append( (tS1,tE1,t1) )

        spans   = newSpans
                    

    return spans



def RemoveGaps(spans):

    for i in range(len(spans)-1):
        
        s0,e0,t0    = spans[i]
        s1,e1,t1    = spans[i+1]

        if e0 != s1:
            width   = e1-s1
            s1      = e0
            e1      = s1+width

        spans[i+1]  = (s1,e1,t1)

    return spans





def AddSpan(spans, span, reduceFlag=True):

    newSpans    = []

    s,e,t = span
    spans   = SplitAtPosition(spans, s)
    spans   = SplitAtPosition(spans, e)
    spans   = RemoveSpansCoveredBy(spans, s,e)
    if reduceFlag == False:
        spans   = ReduceSpans(spans)
    spans   = OrderedInsert( spans, span )
    spans   = RemoveGaps(spans)

    return spans



def InsertSpan(spans, span):

    s,e,t = span
    spans   = SplitAtPosition(spans, s)

    if spans == []:
        newSpans   = [span]

    else:
        newSpans    = []
        s,e,t0      = span
        inserted    = False
        offset      = 0
        for tS,tE,t1 in spans:

            if s == tS and inserted == False:
                newSpans.append(span)
                offset      = e-s
                inserted    = True

            newSpans.append( (tS+offset,tE+offset,t1) )

    newSpans    = ReduceSpans(newSpans)

    return newSpans



def RemoveSpan(spans, span):

    spans.remove(span)
    spans   = RemoveGaps(spans)
    spans   = ReduceSpans(spans)

    return spans




def GetData(spans, rangeStart,rangeEnd):

    numberOfBytes       = rangeEnd-rangeStart
    numberOfBytesCopied = 0
    position            = rangeStart
    data                = b''
    while numberOfBytesCopied < numberOfBytes:
        span                        = SpanAtPoint(spans, position)
        if span != None:
            spanStart,spanEnd,spanData  = span

            numberOfBytesToCopy = min(numberOfBytes,spanEnd-position)
            data                += spanData.Read(position-spanStart,numberOfBytesToCopy)
            numberOfBytesCopied += numberOfBytesToCopy
            position            += numberOfBytesToCopy

        else:
            return data

    return data
    



def RemoveData(spans, rangeStart,rangeEnd):

    logger.debug('spans 1: %s'%(str(spans)))
    span    = (rangeStart,rangeEnd,None)
    spans   = AddSpan(spans, span, reduceFlag=False)
    logger.debug('spans 2: %s'%(str(spans)))
    spans   = RemoveSpan(spans, span)
    logger.debug('spans 3: %s'%(str(spans)))

    return spans





class TestSpans(unittest.TestCase):

    def _test_one(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = AddSpan( spans, (10,20,'B') )
        self.assertEqual(spans, [(0, 10, 'A'), (10, 20, 'B'), (20, 50, 'A'), (50, 100, 'A')] )


    def _test_two(self):

        spans=[]
        spans   = AddSpan( spans, (10,20,'B') )
        self.assertEqual(spans, [(10, 20, 'B')] )



    def _test_three(self):

        spans=[(10,20,'B')]
        spans   = AddSpan( spans, (5,10,'C') )
        self.assertEqual(spans, [(5,10,'C'),(10, 20, 'B')] )



    def _test_four(self):

        spans=[(10,20,'A'),(20,50,'A')]
        spans   = AddSpan( spans, (15,30,'B') )
        self.assertEqual(spans, [(10,15,'A'),(15, 30, 'B'),(30,50,'A')] )



    def _test_five(self):

        spans=[(10,20,'A'),(20,50,'A'),(50,100,'C')]
        spans   = AddSpan( spans, (15,80,'B') )
        self.assertEqual(spans, [(10,15,'A'),(15, 80, 'B'),(80,100,'C')] )



    def _test_six(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (100,120,'C') )
        self.assertEqual(spans, [(10,50,'A'),(50,100,'B'),(100,120,'C')] )


    def _test_seven(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (0,10,'C') )
        self.assertEqual(spans, [(0,10,'C'),(10,50,'A'),(50,100,'B')] )


    def _test_eight(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (0,20,'C') )
        self.assertEqual(spans, [(0,20,'C'),(20,50,'A'),(50,100,'B')] )



    def _test_nine(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (80,120,'C') )
        self.assertEqual(spans, [(10,50,'A'),(50,80,'B'),(80,120,'C')] )


    def _test_ten(self):

        spans=[(50,100,'A')]
        spans   = RemoveSpan( spans, (50,100,'A') )
        self.assertEqual(spans, [] )


    def _test_eleven(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = RemoveSpan( spans, (50,100,'A') )
        self.assertEqual(spans, [(0,50,'A')] )


    def _test_eleven(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = RemoveSpan( spans, (0,50,'A') )
        self.assertEqual(spans, [(50,100,'A')] )


    def _test_twelve(self):

        spans=[(0,50,'A'),(50,100,'A')]
        span    = SpanAtPoint(spans, 10)
        self.assertEqual(span, (0,50,'A') )


    def _test_thirteen(self):

        spans=[(0,50,'A'),(50,100,'B')]
        span    = SpanAtPoint(spans, 55)
        self.assertEqual(span, (50,100,'B') )


    def _test_fourteen(self):

        spans=[(0,50,'A'),(50,100,'B')]
        span    = SpanAtPoint(spans, 101)
        self.assertEqual(span, None )


    def test_fifteen(self):

        text        = b'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))

        spans   = [ (0,10,dataSource.SubDataSource(0,10)) , (10,20,dataSource.SubDataSource(10,20)) ]
        result  = GetData( spans, 0,20 )
        self.assertEqual(result, b'abcdefghijklmnopqrst' )


    def test_sixteen(self):

        text        = b'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))

        spans   = [ (0,10,dataSource.SubDataSource(0,10)) , (10,20,dataSource.SubDataSource(12,22)) ]
        result  = GetData( spans, 0,20 )
        self.assertEqual(result, b'abcdefghijmnopqrstuv' )


    def test_seventeen(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,10,dataSource1.SubDataSource(0,10)), (10,15,dataSource2.SubDataSource(0,10)) , (15,20,dataSource1.SubDataSource(10,20)) ]
        result  = GetData( spans, 0,20 )
        self.assertEqual(result, b'abcdefghijABCDEklmno' )



    def test_eighteen(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        spans   = [ (0,6,dataSource1.SubDataSource(0,10)), (6,10,dataSource1.SubDataSource(11,20)) ]
        result  = GetData( spans, 0,10 )
        self.assertEqual(result, b'abcdeflmno' )



    def test_nineteen(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,26,dataSource1.SubDataSource(0,26)) ]
        spans   = AddSpan(spans, (6,10,dataSource2.SubDataSource(10,17)) )

        result  = GetData( spans, 0,26 )
        self.assertEqual(result, b'abcdefKLMNklmnopqrstuvwxyz' )



    def test_twenty(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,26,dataSource1.SubDataSource(0,26)) ]
        spans   = AddSpan(spans, (10,20,dataSource2.SubDataSource(10,26)) )

        result  = GetData( spans, 0,26 )
        self.assertEqual(result, b'abcdefghijKLMNOPQRSTuvwxyz' )



    def test_twentyone(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        spans   = [ (0,26,dataSource1.SubDataSource(0,26)) ]
        spans   = RemoveData(spans, 10,15 )

        result  = GetData( spans, 0,15 )
        self.assertEqual(result, b'abcdefghijpqrst' )


    def test_twentytwo(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,13,dataSource1.SubDataSource(0,13)), (13,26,dataSource2.SubDataSource(13,26)) ]
        spans   = RemoveData(spans, 10,20 )

        result  = GetData( spans, 0,16 )
        self.assertEqual(result, b'abcdefghijUVWXYZ' )


    def test_twentythree(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,26,dataSource1.SubDataSource(0,26)) ]
        spans   = InsertSpan(spans, (10,20,dataSource2) )

        result  = GetData( spans, 0,36)
        self.assertEqual(result, b'abcdefghijABCDEFGHIJklmnopqrstuvwxyz' )



    def _test_twentyfour(self):

        spans=[(0,50,'A'),(50,100,'B')]
        spans   = InsertSpan(spans, (10,20,'C') )
        self.assertEqual(spans, [(0,10,'A'),(10,20,'C'),(20,60,'A'),(60,110,'B')] )



    def _test_twentyfive(self):

        spans=[]
        spans   = InsertSpan(spans, (10,20,'C') )
        self.assertEqual(spans, [(10,20,'C')] )



    def test_twentysix(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,len(text2))

        spans   = [ (0,26,dataSource1.SubDataSource(0,26)) ]
        spans   = InsertSpan(spans, (26,30,dataSource2) )

        result  = GetData( spans, 0,30)
        self.assertEqual(result, b'abcdefghijklmnopqrstuvwxyzABCD' )




    def test_twentyseven(self):

        text1       = b'*'*32768
        dataSource1 = StringDataSource(text1, 0,0+len(text1))

        text2       = b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        dataSource2 = StringDataSource(text2, 0,0+len(text2))

        spans   = [ (0,32768,dataSource1.SubDataSource(0,32768)) ]
        spans   = InsertSpan(spans, (32768,32778,dataSource2) )

        result  = GetData( spans, 32765,32778)
        self.assertEqual(result, b'***ABCDEFGHIJ' )




    def test_twentyeight(self):

        text1       = b'*'*32768
        dataSource1 = StringDataSource(text1, 0,0+len(text1))

        text2       = b'a'
        dataSource2 = StringDataSource(text2, 0,0+len(text2))

        text3       = b'a'
        dataSource3 = StringDataSource(text3, 0,0+len(text2))

        spans   = [ (0,32768,dataSource1.SubDataSource(0,32768)) ]
        spans   = InsertSpan(spans, (0,1,dataSource2) )
        spans   = InsertSpan(spans, (1,2,dataSource3) )

        result  = GetData( spans, 0,10)
        self.assertEqual(result, b'aa********' )




    def test_thirty(self):

        text1       = b'abcdefghijklmnopqrstuvwxyz'
        dataSource1 = StringDataSource(text1, 0,0+len(text1))

        spans   = [ (0,len(text1),dataSource1.SubDataSource(0,len(text1))) ]
        spans   = RemoveData(spans, 0,1 )

        result  = GetData( spans, 1,10)
        self.assertEqual(result, b'bcdefghij' )







if __name__ == '__main__':

    unittest.main()
    



