



import select
import socket
import DLMSPlayground
import DLMS
import time
import xmltodict
import re
import binascii






class Meter:
    """
    """
    currentMessage  = ''
    inMessage       = False


    def __init__(self):
        """
        """
        self.counter    = 0

    
    def SetTransport(self, transport):
        """
        """
        self.transport  = transport


    def CreateResponse_0000010000FF_02(self):
        """
        """
        template    = \
        """
        <GetResponse>
          <GetResponseNormal>
            <InvokeIdAndPriority Value="81" />
            <Result>
              <Data>
                <OctetString Value="%s" />
              </Data>
            </Result>
          </GetResponseNormal>
        </GetResponse>
        """
        value   = 'xxxx%d'%self.counter
        self.counter    = self.counter+1
        xml = template%binascii.hexlify(value)
        print(xml)
        d   = xmltodict.parse(xml)
        return d



    def CreateResponse_0000600104FF_02(self):
        """
        """
        template    = \
        """
        <GetResponse>
          <GetResponseNormal>
            <InvokeIdAndPriority Value="81" />
            <Result>
              <Data>
                <OctetString Value="%s" />
              </Data>
            </Result>
          </GetResponseNormal>
        </GetResponse>
        """
        value   = '0000%d'%self.counter
        self.counter    = self.counter+1
        xml = template%binascii.hexlify(value)
        print(xml)
        d   = xmltodict.parse(xml)
        return d


    def GetRequest(self, message):
        """
        """
        ic          = message['GetRequest']['GetRequestNormal']['AttributeDescriptor']['ClassId']['@Value']
        code        = message['GetRequest']['GetRequestNormal']['AttributeDescriptor']['InstanceId']['@Value']
        attribute   = message['GetRequest']['GetRequestNormal']['AttributeDescriptor']['AttributeId']['@Value']

        print('--- GetRequest for code:%s ic:%s attribute:%s---'%(code,ic,attribute))

        try:
            handlerName = 'CreateResponse_%s_%s'%(code,attribute)
            return getattr(self,handlerName)()
        except:
            template    = \
            """
            <GetResponse>
              <GetResponseNormal>
                <InvokeIdAndPriority Value="81"/>
                <Result>
                  <DataAccessError Value="ReadWriteDenied"/>
                </Result>
              </GetResponseNormal>
            </GetResponse>
            """
            xml = template
            d   = xmltodict.parse(xml)
            return d
   

    def SetRequest(self, message):
        """
        """
        ic          = message['SetRequest']['SetRequestNormal']['AttributeDescriptor']['ClassId']['@Value']
        code        = message['SetRequest']['SetRequestNormal']['AttributeDescriptor']['InstanceId']['@Value']
        attribute   = message['SetRequest']['SetRequestNormal']['AttributeDescriptor']['AttributeId']['@Value']

        print('--- SetRequest for code:%s ic:%s attribute:%s---'%(code,ic,attribute))

        return message
   

    def AssociationRequest(self, message):
        """
        """
        template  = \
        """
        <AssociationResponse>
          <ApplicationContextName Value="LN" />
          <AssociationResult Value="1" />
          <ResultSourceDiagnostic>
            <ACSEServiceUser Value="13" />
          </ResultSourceDiagnostic>
          <InitiateResponse>
            <NegotiatedDlmsVersionNumber Value="6" />
            <NegotiatedConformance>
              <ConformanceBit Name="Get" />
              <ConformanceBit Name="BlockTransferWithGetOrRead" />
            </NegotiatedConformance>
            <NegotiatedMaxPduSize Value="180" />
            <VaaName Value="7" />
          </InitiateResponse>
        </AssociationResponse>
        """
        xml = template
        d   = xmltodict.parse(xml)
        return d


        

    def ProcessHDLC(self, hdlcHex):
        """
        """
        print('[%s]'%hdlcHex)

        xml = DLMS.HDLCToXML(hdlcHex)
        print(xml)

        d   = xmltodict.parse(xml)
        #print(d)

        #
        # Identify the message type, then call the method
        # of the same name on this object.
        #
        messageType = d.keys()[0]
        responseDict = getattr(self, messageType)(d)

        #
        # Now package the response into transport format (HDLC).
        #
        responseHex = DLMS.DictToHDLC(responseDict)

        return responseHex


    def ByteReceived(self, byte):
        """
        """
        if self.inMessage == True:
            self.currentMessage  = self.currentMessage + '%02x'%ord(byte)
            if ord(byte) == 0x7e:
                self.inMessage   = False
                response    = self.ProcessHDLC(self.currentMessage)
                self.transport.OutputBytes(binascii.unhexlify(response))
        else:
            if ord(byte) == 0x7e:
                self.inMessage       = True
                self.currentMessage  = '7e'







class HDLCTransport:
    """
    """

    def __init__(self):
        """
        """
        pass




class TCPTransportHandler:
    """
    """

    def __init__(self, upperLevel):
        """
        """
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = ('localhost', 9999)
        sock.bind(server_address)
        sock.listen(1)

        upperLevel.SetTransport(self)
        while True:
            self.connection, client_address = sock.accept()

            count   = 0
            while True:
                r, w, e = select.select((self.connection,), (), (), 1.0)
                if r:
                    data = self.connection.recv(1024)
                    numberOfBytes   = len(data)

                    if numberOfBytes == 0:
                        self.connection.close()
                        break

                    elif numberOfBytes > 0:
                        for byte in data:
                            upperLevel.ByteReceived(byte)


    def OutputBytes(self, data):
        """
        """
        print('<%s>'%binascii.hexlify(data))
        self.connection.send(data)




if __name__ == "__main__":

    meter           = Meter()
    hdlcTransport   = HDLCTransport()
    tcpTransport    = TCPTransportHandler(meter)



