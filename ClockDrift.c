


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>


#define TOKEN_PERIOD            (1800)

typedef struct
{
    uint32_t    serverTime; // truth.
    uint32_t    deviceTime; // drifting time on device.
    uint32_t    token;
    uint32_t    value;
    uint32_t    correctedDeviceTime;
    float       driftRate;

} Sample;



uint32_t DriftTime( float driftRate, uint32_t serverTime, uint32_t timeSinceSync )
{
    uint32_t    driftedTime = serverTime + (uint32_t)(driftRate * (float)timeSinceSync );
}


void GenerateClockDriftData( Sample* samples, uint32_t numberOfSamples )
{
    uint32_t    lastTokenReceived   = rand();

    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        samples[i].driftRate     = sin( ((float)i)/7200.0 ) / 1000.0;
        samples[i].token         = lastTokenReceived;
        samples[i].serverTime    = i;
        samples[i].deviceTime    = DriftTime( samples[i].driftRate, samples[i].serverTime, i );

        if( samples[i].serverTime % TOKEN_PERIOD == 0 )
        {
            lastTokenReceived   = rand();
        }
    }
}




void RecoverSamples( Sample* samples, uint32_t numberOfSamples )
{
    uint32_t    lastTokenIndex  = 0;
    uint32_t    lastToken       = samples[lastTokenIndex].token;
    uint32_t    serverTimeAtTokenChange = samples[lastTokenIndex].serverTime;
    uint32_t    deviceTimeAtTokenChange = samples[lastTokenIndex].deviceTime;
    float       driftRate       = 0.0;

    for(uint32_t i=1; i<numberOfSamples; i++)
    {
        if( samples[i].token != lastToken )
        {
            float       deviceDelta = samples[i].deviceTime - samples[lastTokenIndex].deviceTime;
            float       serverDelta = samples[i].serverTime - samples[lastTokenIndex].serverTime;
            driftRate   = (deviceDelta / serverDelta);
            printf("- %08d %08d %08x %f\n", samples[i].serverTime, samples[i].deviceTime, samples[i].token, driftRate );

            lastTokenIndex   = i;
            lastToken   = samples[lastTokenIndex].token;
            serverTimeAtTokenChange = samples[lastTokenIndex].serverTime;
            deviceTimeAtTokenChange = samples[lastTokenIndex].deviceTime;
        }

        uint32_t    deviceTimeSinceLastTokenChange    = samples[i].deviceTime - deviceTimeAtTokenChange;
        uint32_t    correctedDeviceTime = serverTimeAtTokenChange + (uint32_t)((float)deviceTimeSinceLastTokenChange / driftRate) + 1;
        samples[i].correctedDeviceTime  = correctedDeviceTime;
    }
}


int main()
{
    Sample  samples[24*3600];
    const uint32_t  numberOfSamples = sizeof(samples)/sizeof(samples[0]);

    GenerateClockDriftData( &samples[0], numberOfSamples );
    RecoverSamples( &samples[0], numberOfSamples );

    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        uint32_t    diff    = samples[i].correctedDeviceTime - samples[i].serverTime;
        printf("%08d %08d (%08d) %1.5f %d\n", samples[i].serverTime, samples[i].deviceTime, samples[i].correctedDeviceTime, samples[i].driftRate, diff );
    }

    return 0;
}



