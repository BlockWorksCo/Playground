




def SpanContainingPoint(spans, point):

    for start,end in spans:
        if start <= point and end >= point:
            return (start,end)

    return None


def AddSpan(spans, start,end):

    sS,sE   = SpanContainingPoint(spans, start)
    eS,eE   = SpanContainingPoint(spans, end)

    startContainerIndex = spans.index( (sS,sE) )
    endContainerIndex   = spans.index( (eS,eE) )

    if startContainerIndex == endContainerIndex:
        spans.remove( (sS,sE) )
        spans.insert(startContainerIndex, (end,eE))
        spans.insert(startContainerIndex, (start,end))
        spans.insert(startContainerIndex, (sS,start))



    return spans


def RationaliseSpans(spans):

    for span in spans:
        start,end   = span

        if start == end:
            spans.remove(span)


if __name__ == '__main__':

    spans   = [(0,100)]
    AddSpan(spans, 10,20)
    AddSpan(spans, 0,5)
    RationaliseSpans(spans)

    print(spans)
    

