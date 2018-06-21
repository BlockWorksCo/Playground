


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define DRIFT_RATE      (0.01)


typedef struct
{
    uint32_t    serverTime; // truth.
    uint32_t    deviceTime; // drifting time on device.
    uint32_t    token;
    uint32_t    value;

} Sample;



uint32_t DriftTime( uint32_t serverTime, uint32_t timeSinceSync )
{
    uint32_t    driftedTime = serverTime + (uint32_t)(DRIFT_RATE * (float)timeSinceSync );
}


void GenerateClockDriftData( Sample* samples, uint32_t numberOfSamples )
{
    uint32_t    lastTokenReceived   = rand();

    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        samples[i].token         = lastTokenReceived;
        samples[i].serverTime    = i;
        samples[i].deviceTime    = DriftTime( samples[i].serverTime, i );

        if( samples[i].serverTime % 3600 == 0 )
        {
            lastTokenReceived   = rand();
        }
    }
}




void RecoverSamples( Sample* samples, uint32_t numberOfSamples )
{
    uint32_t    lastTokenIndex  = 0;
    uint32_t    lastToken       = samples[lastTokenIndex].token;

    for(uint32_t i=1; i<numberOfSamples; i++)
    {
        if( samples[i].token != lastToken )
        {
            float   deviceDelta = samples[i].deviceTime - samples[lastTokenIndex].deviceTime; 
            float   serverDelta = samples[i].serverTime - samples[lastTokenIndex].serverTime; 
            float   driftRate   = deviceDelta / serverDelta;
            printf("- %08d %08d %08x %f\n", samples[i].serverTime, samples[i].deviceTime, samples[i].token, driftRate );

            lastTokenIndex   = i;
            lastToken   = samples[lastTokenIndex].token;
        }
    }
}


int main()
{
    Sample  samples[24*3600];
    const uint32_t  numberOfSamples = sizeof(samples)/sizeof(samples[0]);

    GenerateClockDriftData( &samples[0], numberOfSamples );
    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        printf("%08d %08d %08x\n", samples[i].serverTime, samples[i].deviceTime, samples[i].token );
    }

    RecoverSamples( &samples[0], numberOfSamples );

    return 0;
}



