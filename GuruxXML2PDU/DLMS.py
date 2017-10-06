




import binascii
import sys
import subprocess
import xmltodict


def ReadPDU(position, hdlc):
    """
    """
    pdu = hdlc[position:-3]
    position    = position+len(pdu)

    return (pdu,position)



def ReadCS(position, hdlc):
    """
    """
    hi = ord(hdlc[position])
    position    = position+1

    lo = ord(hdlc[position])
    position    = position+1

    cs = (hi<<8) | lo

    return (cs,position)



def ReadLLC(position, hdlc):
    """
    """
    remoteLSAP = ord(hdlc[position])
    position    = position+1

    localLSAP   = ord(hdlc[position])
    position    = position+1

    llcQuality  = ord(hdlc[position])
    position    = position+1

    return (remoteLSAP,position)



def ReadFrameFormat(position, hdlc):
    """
    Note: this can be a two byte field if segmentation occurs.
    """
    formatHi = ord(hdlc[position])
    position    = position+1

    formatLo = ord(hdlc[position])
    position    = position+1

    format  = (formatHi<<8) | formatLo

    frameType   = format&0xf000
    frameLength = format&0x7ff

    return (frameType,frameLength,position)



def ReadControlField(position, hdlc):
    """
    """
    control = ord(hdlc[position])
    position    = position+1

    return (control,position)



def ReadAddress(position, hdlc):
    """
    """
    address = ord(hdlc[position])
    position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    if address&0x01 == 0:
        address     = (address << 8) | ord(hdlc[position])
        position    = position+1

    return (address,position)





class FCS16(object):

    fcstab = [
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78,
    ]

    _PPP_INITIAL_FCS16 = 0xffff
    _PPP_GOOD_FCS16 = 0xf0b8

    def __init__(self):
        pass

    @classmethod
    def pppfcs16(self, fcs, packet, formated=False):
        for ch in packet:
            fcs = (fcs >> 8) ^ self.fcstab[(fcs ^ ord(ch)) & 0xff]
            print('%02x = %04x'%(ord(ch),0xffff-fcs))

        if formated is True:
            fcs = fcs ^ 0xffff
            fcs = chr(fcs & 0x00ff), chr((fcs >> 8) & 0x00ff)

        return fcs

    @classmethod
    def fcsFormat(self, fcs):
        fcs = fcs ^ 0xffff
        fcs = chr(fcs & 0x00ff), chr((fcs >> 8) & 0x00ff)
        fcs = ''.join(map(lambda c: c, fcs))

        return fcs




def crc16(data):
    """
    Needs to be inverted and byte-swapped from raw FCS-16 calculation.
    """
    crc = 0xffff - FCS16.pppfcs16( FCS16._PPP_INITIAL_FCS16, binascii.unhexlify(data) )
    hi  = (crc&0xff00)>>8
    lo  = crc&0xff
    crc = (lo<<8)|hi
    return crc

def WritePDU(hdlc, pdu):
    """
    """
    return hdlc+pdu



def WriteHCS(hdlc,hcsPos,cs):
    """
    """
    hdlc = hdlc[0:hcsPos] + ('%04x'%cs) + hdlc[hcsPos+4:]
    return hdlc


def WriteFakeHCS(hdlc,hcsPos,cs):
    """
    """
    return hdlc+('%04x'%cs)


def WriteFCS(hdlc,cs):
    """
    """
    return hdlc+('%04x'%cs)



def WriteLLC(hdlc, llc):
    """
    """
    return hdlc+('%06x'%llc)



def WriteFrameFormat(hdlc, frameType):
    """
    """
    frameLength = 0x47
    format  = (frameType<<8) | (frameLength&0x7ff)

    return hdlc+('%04x'%format)



def WriteFrameLength(hdlc, frameLength):
    """
    11-bit field
    """

    hdlc = hdlc[0:4] + ('%02x'%frameLength) + hdlc[6:]

    return hdlc



def WriteControlField(hdlc, control):
    """
    """
    return hdlc+('%02x'%control)



def WriteAddress(hdlc, address):
    """
    """
    if address <= 0xff:
        address = '%02x'%(address|0x01)

    if address <= 0xffff:
        address = '%04x'%(address|0x01)

    if address <= 0xffffff:
        address = '%08x'%(address|0x01)

    return hdlc+address



def PDUToHDLC(pduHex, controlField=0x10):
    """
    7e a01c 00020023 41 32 ca4d e6e600 c0018100010000600100ff0200 8c6d 7e <-- good
    7e a01c 00020023 41 10 974b e6e600 C0018100010000600101FF0200 030f 7e
    7e a01c 00020023 41 32 8749 e6e600 c0018100010000600100ff0200 b813 7e
    """
    frameType   = 0xa0
    length      = 0x00
    srcAddress  = 0x41
    dstAddress  = 0x00020023
    LLC         = 0xe6e600

    hdlc   = '7e'
    hdlc   = WriteFrameFormat(hdlc,frameType)
    hdlc   = WriteAddress(hdlc,dstAddress)
    hdlc   = WriteAddress(hdlc,srcAddress)
    hdlc   = WriteControlField(hdlc, controlField)
    hcsPos   = len(hdlc)
    hdlc   = WriteFakeHCS(hdlc, hcsPos, 0xffff )
    print()
    hdlc   = WriteLLC(hdlc, LLC)
    hdlc   = WritePDU(hdlc, pduHex)
    hdlc   = WriteFrameLength(hdlc, (len(hdlc[2:])+4)/2 )
    hdlc   = WriteHCS(hdlc, hcsPos, crc16(hdlc[2:hcsPos]) )
    hdlc   = WriteFCS(hdlc, crc16(hdlc[2:]) )
    hdlc   = hdlc+'7e'

    return hdlc


def HDLCToDict(hdlcHex):
    """
    """
    hdlc    = binascii.unhexlify(hdlcHex)

    if ord(hdlc[0]) != 0x7e or ord(hdlc[-1]) != 0x7e:
        print('no HDLC framing detected.')
    else:
        position                    = 1
        frameType,length,position   = ReadFrameFormat(position, hdlc)
        dstAddress,position         = ReadAddress(position, hdlc)
        srcAddress,position         = ReadAddress(position, hdlc)
        controlField,position       = ReadControlField(position, hdlc)
        HCS,position                = ReadCS(position, hdlc)
        LLC,position                = ReadLLC(position, hdlc)
        pdu,position                = ReadPDU(position, hdlc)
        FCS,position                = ReadCS(position, hdlc)

        p   = subprocess.Popen(['java','-jar','pdu2xml.jar',binascii.hexlify(pdu)], stdout=subprocess.PIPE)
        output,errorOutput  = p.communicate()

        d   = xmltodict.parse(output)
    
        return d


def DictToHDLC(d):
    """
    """
    xml = xmltodict.unparse(d)
    open('temp.xml','w+').write(xml)

    p   = subprocess.Popen(['java','-jar','xml2pdu.jar','temp.xml'], stdout=subprocess.PIPE)
    pdu,errorOutput  = p.communicate()

    hdlc    = PDUToHDLC(pdu.replace('\n',''))

    return hdlc


# d=DLMS.HDLCToDict('7ea01c000200234176ea49e6e600c00181000301000c0700ff020014fa7e')
# DLMS.DictToHDLC(d)






if __name__ == '__main__':
    print(PDUToHDLC('c0018100010000600100ff0200', controlField=0x32))
    print('7ea01c000200234132ca4de6e600c0018100010000600100ff02008c6d7e')

