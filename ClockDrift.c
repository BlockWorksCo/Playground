


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>


//
// Algorithm assumes linear drift between token periods. Tokens are psuedo-random numbers
// sent to the device on the return from the previous set of sample data being transferred
// to the server.
// Both the server and device can authenticate the other by checking the token against the
// expected value.
//
#define TOKEN_PERIOD            (900)

typedef struct
{
    uint32_t    serverTime; // truth.
    uint32_t    deviceTime; // drifting time on device.
    uint32_t    token;
    uint32_t    value;
    uint32_t    correctedDeviceTime;
    float       driftRate;          // real drift rate used in generating data.
    float       derivedDriftRate;   // calculated during correction.

} Sample;




// https://helloacm.com
float BilinearInterpolation(float q11, float q12, float q21, float q22, 
                            float x1, float x2, 
                            float y1, float y2, 
                            float x, float y) 
{
    float x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

uint32_t DriftTime( float driftRate, uint32_t serverTime, uint32_t timeSinceSync )
{
    uint32_t    driftedTime = serverTime + (uint32_t)(driftRate * (float)timeSinceSync );
}


void GenerateClockDriftData( Sample* samples, uint32_t numberOfSamples )
{
    uint32_t    lastTokenReceived   = rand();

    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        samples[i].driftRate     = sin( ((float)i)/7200.0 ) / 10000.0;
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
        samples[i].derivedDriftRate     = driftRate;
    }
}


int main()
{
    static Sample  samples[7*24*3600];
    const uint32_t  numberOfSamples = sizeof(samples)/sizeof(samples[0]);
    uint32_t        totalError  = 0;
    int32_t         maxError    = -1;
    int32_t         minError    = 1;

    GenerateClockDriftData( &samples[0], numberOfSamples );
    RecoverSamples( &samples[0], numberOfSamples );

    for(uint32_t i=0; i<numberOfSamples; i++)
    {
        int32_t    diff    = samples[i].correctedDeviceTime - samples[i].serverTime;
        totalError  += diff;
        if( diff >= maxError )
        {
            maxError    = diff;
        }
        if( diff <= minError )
        {
            minError    = diff;
        }
        printf("%08d %08d %08d %1.8f %d %f\n", samples[i].serverTime, samples[i].deviceTime, samples[i].correctedDeviceTime, samples[i].driftRate, diff, samples[i].derivedDriftRate-1.0 );
    }
    printf("Average error: %f\n", (float)totalError / (float)numberOfSamples);
    printf("error range: %d -> %d\n", minError, maxError );

    return 0;
}



