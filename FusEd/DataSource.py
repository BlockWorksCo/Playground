


import unittest


class StringDataSource:

    def __init__(self, text, rangeStart,rangeEnd):
        self.text       = text
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        readStart   = self.rangeStart+offset
        readEnd     = min(readStart+numberOfBytes, self.rangeEnd)

        #print('readStart=%d'%readStart)
        #print('readEnd=%d'%readEnd)
        #print('readData=%s'%self.text[readStart:readEnd])

        data        = self.text[readStart:readEnd]

        return data
        



class Tests(unittest.TestCase):

    def test_one(self):

        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))
        data        = dataSource.Read(10,5)
        self.assertEqual(data, 'klmno' )







if __name__ == '__main__':

    unittest.main()
    



