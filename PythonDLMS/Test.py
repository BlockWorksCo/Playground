

import unittest
from EncodeDLMS import *
from EncodeAXDR import *
from EncodeHDLC import *
from DecodeDLMS import *
from DecodeAXDR import *
from DecodeHDLC import *
import xmltodict


class DecodeDLMSTests(unittest.TestCase):

    def tstOne(self):
        input   = 'c00181000100002a0000ff0200'
        result  = DecodeDLMS(binascii.unhexlify(input))
        print('[%s]'%result)
        self.assertEqual('foo'.upper(), 'FOO')


    def testTwo(self):
        input   = 'C40181000101020A02020FFE162102020FFE162102020FFE162102020FFF162302020FFF162302020FFF162302020FFE161E02020FFE162002020FFE162002020FFE161F'
        result  = DecodeDLMS(binascii.unhexlify(input))
        print('[%s]'%result)
        self.assertEqual('foo'.upper(), 'FOO')


class DecodeHDLCTests(unittest.TestCase):

    def testOne(self):
        input   = "7ea0190341540abbe6e600c00181000100002a0000ff0200174d7e"
        (frameType,dstAddress,srcAddress,controlField,LLC,pdu)  = DecodeHDLC(input) 
        self.assertEqual(frameType, 0xa000)
        self.assertEqual(dstAddress, 0x03)
        self.assertEqual(srcAddress, 0x41)
        self.assertEqual(controlField, 0x54)
        self.assertEqual(LLC, 'e6e600')
        self.assertEqual(binascii.hexlify(pdu), 'c00181000100002a0000ff0200')

    def testTwo(self):
        input   = "7ea02003419328bc8180140502080006020800070400000007080400000007b3c67e"
        (frameType,dstAddress,srcAddress,controlField,LLC,pdu)  = DecodeHDLC(input) 
        self.assertEqual(frameType, 0xa000)
        self.assertEqual(dstAddress, 0x03)
        self.assertEqual(srcAddress, 0x41)
        self.assertEqual(controlField, 0x93)
        self.assertEqual(LLC, '818014')
        self.assertEqual(binascii.hexlify(pdu), '0502080006020800070400000007080400000007')



if __name__ == '__main__':
    unittest.main()





