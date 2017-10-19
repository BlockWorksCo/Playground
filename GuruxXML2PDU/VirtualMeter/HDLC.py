

import binascii
import DLMS


class HDLC:
    """
    """

    def __init__(self):
        """
        """
        self.inMessage  = False


    def Link(self, upper, lower):
        """
        """
        self.upperLevel = upper
        self.lowerLevel = lower

    def ByteReceived(self, byte):
        """
        """
        if self.inMessage == True:
            self.currentMessage  = self.currentMessage + '%02x'%ord(byte)
            if ord(byte) == 0x7e:
                self.inMessage   = False
                pdu     = DLMS.HDLCToPDU(self.currentMessage)
                pduHex  = binascii.hexlify(pdu)

                self.upperLevel.PDUReceived(pduHex)
        else:
            if ord(byte) == 0x7e:
                self.inMessage       = True
                self.currentMessage  = '7e'


    def OutputBytes(self, pduHex):
        """
        """
        #print('OutputBytes [%s]'%pduHex)
        hdlc     = DLMS.PDUToHDLC(pduHex)
        #print('hdlc=[%s]'%hdlc)
        self.lowerLevel.OutputBytes(binascii.unhexlify(hdlc))



