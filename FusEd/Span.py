


import unittest




def SplitAtPosition(spans, position):

    newSpans    = []
    for j in range(len(spans)):
        tS,tE,t   = spans[j]

        if tS<=position and tE>position:
            newSpans.append( (tS,position,t) )
            newSpans.append( (position,tE,t) )

        else:
            newSpans.append( (tS,tE,t) )

    return newSpans


def RemoveSpansCoveredBy(spans, s,e):
    
    newSpans    = []
    for j in range(len(spans)):
        tS,tE,t   = spans[j]

        if tS<s or tE>e:
            newSpans.append( (tS,tE,t) )
    
    return newSpans



def OrderedInsert(spans, span):
    
    if spans == []:
        newSpans    = [span]

    else:
        newSpans    = []
        s,e,t0      = span
        for j in range(len(spans)):
            tS,tE,t1   = spans[j]

            newSpans.append( (tS,tE,t1) )

            if tE == s:
                newSpans.append(span)

    return newSpans



def AddSpan(spans, span):

    newSpans    = []

    s,e,t = span
    spans   = SplitAtPosition(spans, s)
    spans   = SplitAtPosition(spans, e)
    spans   = RemoveSpansCoveredBy(spans, s,e)
    spans   = OrderedInsert( spans, span )

    return spans


def ReduceSpans(spans):

    for tS,tE,t in spans:
        if tS == tE:
            spans.remove( (tS,tE,t) )

    return spans





class TestSpans(unittest.TestCase):

    def test_one(self):

        spans=[(0,50,'A'),(50,100,'A')]
        spans   = AddSpan( spans, (10,20,'B') )
        self.assertEqual(spans, [(0, 10, 'A'), (10, 20, 'B'), (20, 50, 'A'), (50, 100, 'A')] )


    def test_two(self):

        spans=[]
        spans   = AddSpan( spans, (10,20,'B') )
        print(spans)
        self.assertEqual(spans, [(10, 20, 'B')] )







if __name__ == '__main__':

    unittest.main()
    



