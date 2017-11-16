"""

          ----- -------- --------     ---------

          +++++ ++       ++++++++++++ +++++++++


"""



import unittest
from DataSource import *



def SpanAtPoint(spans, position):

    for tS,tE,t in spans:
        if tS<=position and tE>position:
            return (tS,tE,t)

    return None


def SplitAtPosition(spans, position):

    newSpans    = []
    for tS,tE,t in spans:

        if tS<=position and tE>position:
            newSpans.append( (tS,position,t) )
            newSpans.append( (position,tE,t) )

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
            spans.remove( (tS,tE,t) )

    return spans





def AddSpan(spans, span):

    newSpans    = []

    s,e,t = span
    spans   = SplitAtPosition(spans, s)
    spans   = SplitAtPosition(spans, e)
    spans   = RemoveSpansCoveredBy(spans, s,e)
    spans   = ReduceSpans(spans)
    spans   = OrderedInsert( spans, span )

    return spans



def RemoveSpan(spans, span):

    spans.remove(span)

    return spans




def GetData(spans, rangeStart,rangeEnd):

    numberOfBytes       = rangeEnd-rangeStart
    numberOfBytesCopied = 0
    position            = rangeStart
    data                = ''
    while numberOfBytesCopied < numberOfBytes:
        spanStart,spanEnd,spanData  = SpanAtPoint(spans, position)

        #print('spanStart=%d'%spanStart)
        #print('spanEnd=%d'%spanEnd)
        #print('spanData=%s'%str(spanData))

        numberOfBytesToCopy = min(numberOfBytes,spanEnd-spanStart)
        data                += spanData.Read(0,numberOfBytesToCopy)
        numberOfBytesCopied += numberOfBytesToCopy
        position            += numberOfBytesToCopy

    return data
    







class TestSpans(unittest.TestCase):

    def test_one(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = AddSpan( spans, (10,20,'B') )
        self.assertEqual(spans, [(0, 10, 'A'), (10, 20, 'B'), (20, 50, 'A'), (50, 100, 'A')] )


    def test_two(self):

        spans=[]
        spans   = AddSpan( spans, (10,20,'B') )
        self.assertEqual(spans, [(10, 20, 'B')] )



    def test_three(self):

        spans=[(10,20,'B')]
        spans   = AddSpan( spans, (5,10,'C') )
        self.assertEqual(spans, [(5,10,'C'),(10, 20, 'B')] )



    def test_four(self):

        spans=[(10,20,'A'),(20,50,'A')]
        spans   = AddSpan( spans, (15,30,'B') )
        self.assertEqual(spans, [(10,15,'A'),(15, 30, 'B'),(30,50,'A')] )



    def test_five(self):

        spans=[(10,20,'A'),(20,50,'A'),(50,100,'C')]
        spans   = AddSpan( spans, (15,80,'B') )
        self.assertEqual(spans, [(10,15,'A'),(15, 80, 'B'),(80,100,'C')] )



    def test_six(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (100,120,'C') )
        self.assertEqual(spans, [(10,50,'A'),(50,100,'B'),(100,120,'C')] )


    def test_seven(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (0,10,'C') )
        self.assertEqual(spans, [(0,10,'C'),(10,50,'A'),(50,100,'B')] )


    def test_eight(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (0,20,'C') )
        self.assertEqual(spans, [(0,20,'C'),(20,50,'A'),(50,100,'B')] )



    def test_nine(self):

        spans=[(10,50,'A'),(50,100,'B')]
        spans   = AddSpan( spans, (80,120,'C') )
        self.assertEqual(spans, [(10,50,'A'),(50,80,'B'),(80,120,'C')] )


    def test_ten(self):

        spans=[(50,100,'A')]
        spans   = RemoveSpan( spans, (50,100,'A') )
        self.assertEqual(spans, [] )


    def test_eleven(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = RemoveSpan( spans, (50,100,'A') )
        self.assertEqual(spans, [(0,50,'A')] )


    def test_eleven(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = RemoveSpan( spans, (0,50,'A') )
        self.assertEqual(spans, [(50,100,'A')] )


    def test_twelve(self):

        spans=[(0,50,'A'),(50,100,'A')]
        span    = SpanAtPoint(spans, 10)
        self.assertEqual(span, (0,50,'A') )


    def test_thirteen(self):

        spans=[(0,50,'A'),(50,100,'B')]
        span    = SpanAtPoint(spans, 55)
        self.assertEqual(span, (50,100,'B') )


    def test_fourteen(self):

        spans=[(0,50,'A'),(50,100,'B')]
        span    = SpanAtPoint(spans, 101)
        self.assertEqual(span, None )


    def test_fifteen(self):

        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))

        spans   = [ (0,10,dataSource.SubDataSource(0,10)) , (10,20,dataSource.SubDataSource(10,20)) ]
        result  = GetData( spans, 0,20 )
        self.assertEqual(result, 'abcdefghijklmnopqrst' )







if __name__ == '__main__':

    unittest.main()
    



