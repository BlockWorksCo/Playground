


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "Stream.h"


void streamSetUint8(Stream* stream, uint8_t value)                                               
{                                                                                                  
    uint8_t*    streamBytes = (uint8_t*)*stream;                                                   
    streamBytes[0]  = value;                                                                       
    *stream      = (Stream)(streamBytes+1);                                                    
}                                                                                                  
                                                                                                   
void streamSetUint8Array(Stream* stream, uint8_t* values, uint32_t numberOfElements)             
{                                                                                                  
    for(uint32_t i=0; i<numberOfElements; i++)                                                     
    {                                                                                              
        streamSetUint8( stream, values[i] );                                                         
    }                                                                                              
}                                                                                                  




void streamGetUint8(Stream* stream, uint8_t* value)                    
{                                                                        
    uint8_t*    streamBytes = (uint8_t*)*stream;                         
                                                                         
    *value  = streamBytes[0];                                            
    *stream  = (Stream)(streamBytes+1);                              
}                                                                        
                                                                         

void streamGetUint8Array(Stream* stream, uint8_t* values, uint32_t numberOfElements)              
{                                                                                                   
    for(uint32_t i=0; i<numberOfElements; i++)                                                      
    {                                                                                               
        streamGetUint8( stream, &values[i] );                                                         
    }                                                                                               
}                                                                                                   

