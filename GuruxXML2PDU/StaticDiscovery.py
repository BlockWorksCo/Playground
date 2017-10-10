



import DLMSPlayground
import DLMS
import time



OBISList    = \
[
   {'code':[ 0,0,1,0,0,255 ], 'ic':3 },
   {'code':[ 1,0,0,1,2,255 ], 'ic':3 },
   {'code':[ 1,0,31,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,51,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,71,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,32,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,52,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,72,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,4,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,15,255 ], 'ic':3 },
   {'code':[ 1,0,81,7,26,255 ], 'ic':3 },
   {'code':[ 1,0,13,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,33,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,53,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,73,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,21,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,22,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,41,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,42,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,61,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,62,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,23,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,24,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,43,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,44,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,63,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,64,7,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,1,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,2,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,3,255 ], 'ic':3 },
   {'code':[ 1,0,1,6,4,255 ], 'ic':3 },
   {'code':[ 1,0,2,6,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,6,1,255 ], 'ic':3 },
   {'code':[ 1,0,2,6,2,255 ], 'ic':3 },
   {'code':[ 1,0,2,6,3,255 ], 'ic':3 },
   {'code':[ 1,0,2,6,4,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,1,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,2,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,1,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,2,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,1,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,2,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,3,255 ], 'ic':3 },
   {'code':[ 1,0,3,6,4,255 ], 'ic':3 },
   {'code':[ 1,0,4,6,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,6,1,255 ], 'ic':3 },
   {'code':[ 1,0,4,6,2,255 ], 'ic':3 },
   {'code':[ 1,0,4,6,3,255 ], 'ic':3 },
   {'code':[ 1,0,4,6,4,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,3,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,1,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,2,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,3,255 ], 'ic':3 },
   {'code':[ 1,0,4,8,4,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,3,2,4,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,0,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,1,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,2,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,3,255 ], 'ic':3 },
   {'code':[ 1,0,4,2,3,255 ], 'ic':3 },
   {'code':[ 0,0,1,0,0,255 ], 'ic':3 },
]




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

    for obisInfo in OBISList:
        a   = obisInfo['code'][0]
        b   = obisInfo['code'][1]
        c   = obisInfo['code'][2]
        d   = obisInfo['code'][3]
        e   = obisInfo['code'][4]
        f   = obisInfo['code'][5]
        ic  = obisInfo['ic']

        hexCode = '%02x%02x%02x%02x%02x%02x'%(a,b,c,d,e,f)
        print('------- getting %s --------'%hexCode)

        rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,2)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(1.0)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )
     
        rq    = DLMSPlayground.CreateGetRequest(ic,hexCode,2)
        print(rq)
        DLMSPlayground.SendHDLCToMeter(p, rq )
        time.sleep(1.0)
        rsp    = DLMSPlayground.GetResponseFromMeter(p)
        print(rsp)
        print( DLMS.HDLCToDict(rsp) )
     



if __name__ == '__main__':
    StaticDiscovery()

