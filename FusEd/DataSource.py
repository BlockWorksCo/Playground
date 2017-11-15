


import unittest


class StringDataSource:

    def __init__(self, text, rangeStart,rangeEnd):
        self.text       = text
        self.rangeStart = rangeStart
        self.rangeEnd   = rangeEnd


    def Read(self, offset, numberOfBytes):

        data    = self.text[self.rangeStart+offset:self.rangeStart+offset+numberOfBytes]

        return data
        



class Tests(unittest.TestCase):

    def test_one(self):

        text        = 'abcdefghijklmnopqrstuvwxyz'
        dataSource  = StringDataSource(text, 0,len(text))
        data        = dataSource.Read(10,5)
        self.assertEqual(data, 'klmno' )







if __name__ == '__main__':

    unittest.main()
    



