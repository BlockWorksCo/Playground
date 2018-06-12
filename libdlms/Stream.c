


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "Stream.h"


void axdrSetUint8(AXDRStream* stream, uint8_t value)                                               
{                                                                                                  
    uint8_t*    streamBytes = (uint8_t*)*stream;                                                   
    streamBytes[0]  = value;                                                                       
    *stream      = (AXDRStream)(streamBytes+1);                                                    
}                                                                                                  
                                                                                                   
void axdrSetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements)             
{                                                                                                  
    for(uint32_t i=0; i<numberOfElements; i++)                                                     
    {                                                                                              
        axdrSetUint8( stream, values[i] );                                                         
    }                                                                                              
}                                                                                                  




void axdrGetUint8(AXDRStream* stream, uint8_t* value)                    
{                                                                        
    uint8_t*    streamBytes = (uint8_t*)*stream;                         
                                                                         
    *value  = streamBytes[0];                                            
    *stream  = (AXDRStream)(streamBytes+1);                              
}                                                                        
                                                                         

void axdrGetUint8Array(AXDRStream* stream, uint8_t* values, uint32_t numberOfElements)              
{                                                                                                   
    for(uint32_t i=0; i<numberOfElements; i++)                                                      
    {                                                                                               
        axdrGetUint8( stream, &values[i] );                                                         
    }                                                                                               
}                                                                                                   

