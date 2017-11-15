


import unittest
from Span import *




def AddOverlay(overlays, offset,data):

    overlays    = AddSpan(overlays, (offset,offset+len(data),data) )

    return overlays



def GetData(overlays, offset):

    return data
    

class TestSpans(unittest.TestCase):

    def test_one(self):

        overlays    = []
        overlays    = AddOverlay(overlays, 10, '01234')
        self.assertEqual(overlays, [(10, 15, '01234')] )








if __name__ == '__main__':

    unittest.main()
    



