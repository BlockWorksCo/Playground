



import DLMSPlayground
import DLMS
import time




def StaticDiscovery():
    """
    """
    p=DLMSPlayground.OpenPortToMeter('/dev/ttyUSB2')

    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateDISC() )
    time.sleep(1.0)
    print( DLMSPlayground.GetResponseFromMeter(p) )

    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateSNRM() )
    time.sleep(1.0)
    print( DLMSPlayground.GetResponseFromMeter(p) )

    DLMSPlayground.SendHDLCToMeter(p, DLMSPlayground.CreateAARQ('LN', 'Low', '3132333435363738') )
    time.sleep(1.0)
    print( DLMS.HDLCToDict(DLMSPlayground.GetResponseFromMeter(p)) )

    rq    = DLMSPlayground.CreateGetRequest(1,'0000600100FF',2)
    print(rq)
    DLMSPlayground.SendHDLCToMeter(p, rq )
    time.sleep(1.0)
    rsp    = DLMSPlayground.GetResponseFromMeter(p)
    print(rsp)
    print( DLMS.HDLCToDict(rsp) )
 



if __name__ == '__main__':
    StaticDiscovery()

