


import unittest



def SpanContainingPoint(spans, point):

    for start,end in spans:
        if start <= point and end >= point:
            return (start,end)

    return None,None


def AddSpan(spans, start,end):

    sS,sE   = SpanContainingPoint(spans, start)
    eS,eE   = SpanContainingPoint(spans, end)

    try:
        startContainerIndex = spans.index( (sS,sE) )
    except ValueError:
        startContainerIndex = -1

    try:
        endContainerIndex   = spans.index( (eS,eE) )
    except ValueError:
        endContainerIndex   = -1

    #print('start='+str(startContainerIndex))
    #print('end='+str(endContainerIndex))

    if startContainerIndex == endContainerIndex:
        spans.remove( (sS,sE) )
        spans.insert(startContainerIndex, (end,eE))
        spans.insert(startContainerIndex, (start,end))
        spans.insert(startContainerIndex, (sS,start))

    if startContainerIndex != endContainerIndex and endContainerIndex == -1:
        
        spans.append( (start,end) )


    if startContainerIndex != endContainerIndex and startContainerIndex == -1:
        
        spans.insert(0, (start,end) )



    return spans


def RationaliseSpans(spans):

    for span in spans:
        start,end   = span

        if start == end:
            spans.remove(span)




class TestSpans(unittest.TestCase):

    def test_one(self):
        spans   = [(0,100)]
        AddSpan(spans, 10,20)
        AddSpan(spans, 0,5)
        RationaliseSpans(spans)

        self.assertEqual(spans, [(0, 5), (5, 10), (10, 20), (20, 100)] )


    def test_two(self):
        spans   = [(0,100)]
        AddSpan(spans, 100,120)
        RationaliseSpans(spans)

        self.assertEqual(spans, [(0, 100), (100, 120)] )



    def test_three(self):
        spans   = [(0,100)]
        AddSpan(spans, -10,0)
        RationaliseSpans(spans)

        self.assertEqual(spans, [(-10, 0), (0, 100)] )






if __name__ == '__main__':

    unittest.main()
    

