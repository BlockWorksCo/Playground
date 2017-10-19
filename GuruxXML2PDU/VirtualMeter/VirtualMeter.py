



import HDLC
import TCP
import DLMSPlayground
import DLMS
import xmltodict
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


        

    def ProcessHDLC(self, pduHex):
        """
        """
        #print('\n----------------request (hex) ---------\n%s'%pduHex)
        d  = DLMS.PDUToDict(pduHex)

        #
        # Identify the message type, then call the method
        # of the same name on this object.
        #
        print('\n----------------request ---------\n%s'%d)
        messageType = d.keys()[0]
        responseDict = getattr(self, messageType)(d)
        print('\n----------------response ---------\n%s'%responseDict)
        print(responseDict)

        #
        # Now package the response into transport format (HDLC).
        #
        responseHex = DLMS.DictToPDU(responseDict)
        #print('responseHex=[%s]'%responseHex)
        response    = binascii.unhexlify(responseHex)
        #print('[%s]'%responseHex)

        #print('\n----------------response (hex)---------\n%s'%responseHex)
        return responseHex


    def PDUReceived(self, pduHex):
        """
        """
        responseHex    = self.ProcessHDLC(pduHex)
        self.lowerLevel.OutputBytes(responseHex) 

        return responseHex


    def Link(self, upper, lower):
        """
        """
        self.upperLevel = upper
        self.lowerLevel = lower





if __name__ == "__main__":

    meter   = Meter()
    hdlc    = HDLC.HDLC()
    tcp     = TCP.TCPTransport()

    meter.Link(None,hdlc)
    hdlc.Link(meter,tcp)
    tcp.Link(hdlc,None)

    tcp.Run()


