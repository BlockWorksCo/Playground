//
//  Player.m
//  Starz
//
//  Created by Steve Tickle on 29/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Player.h"

//
//
//
BOOL    theChangesAreNestedFlag = NO;

@implementation Player

@synthesize theMineBuildRate;
@synthesize theFactoryBuildRate;
@synthesize theResearchRate;
@synthesize theExtraShipsFlag;


//
//
//
-(id)init
{
    self    = [super init];
    
    //
    //
    //
    theMineBuildRate    = 0.333;
    theFactoryBuildRate = 0.333;
    theResearchRate     = 0.333;
    
    theExtraShipsFlag   = NO;
    
    return self;
}


//
//
//
-(void)setTheMineBuildRate:(float)rate
{
    if(theChangesAreNestedFlag == YES)
    {
        return;
    }
    
    theChangesAreNestedFlag = YES;
    
    //
    //
    //
    theMineBuildRate            = rate;
    
    float       ratioOfOtherTwo     = (theFactoryBuildRate/theResearchRate);
    float       remainder           = 1.0 - rate;
    
    //
    //
    //
    [self willChangeValueForKey:@"theResearchRate"];
    [self willChangeValueForKey:@"theFactoryBuildRate"];

    if(isnan(ratioOfOtherTwo))
    {
        theMineBuildRate            = 0.333;
        theFactoryBuildRate         = 0.333;
        theResearchRate             = 0.333;
    }
    else
    {
        //
        //
        //
        theFactoryBuildRate     = (remainder * ratioOfOtherTwo) / (ratioOfOtherTwo + 1.0);
        theResearchRate         = remainder / (ratioOfOtherTwo + 1.0);
    }
    
    //
    //
    //
    [self didChangeValueForKey:@"theResearchRate"];
    [self didChangeValueForKey:@"theFactoryBuildRate"];
        
    theChangesAreNestedFlag = NO;
}

//
//
//
-(void)setTheFactoryBuildRate:(float)rate
{
    if(theChangesAreNestedFlag == YES)
    {
        return;
    }
    
    theChangesAreNestedFlag = YES;
    
    //
    //
    //
    theFactoryBuildRate             = rate;
    
    float       ratioOfOtherTwo     = (theMineBuildRate/theResearchRate);
    float       remainder           = 1.0 - rate;
    
    //
    //
    //
    [self willChangeValueForKey:@"theResearchRate"];
    [self willChangeValueForKey:@"theMineBuildRate"];

    if(isnan(ratioOfOtherTwo))
    {
        theMineBuildRate            = 0.333;
        theFactoryBuildRate         = 0.333;
        theResearchRate             = 0.333;
    }
    else
    {
        //
        //
        //
        theMineBuildRate     = (remainder * ratioOfOtherTwo) / (ratioOfOtherTwo + 1.0);
        theResearchRate         = remainder / (ratioOfOtherTwo + 1.0);
    }
    
    //
    //
    //
    [self didChangeValueForKey:@"theResearchRate"];
    [self didChangeValueForKey:@"theMineBuildRate"];
        
    theChangesAreNestedFlag = NO;
}

//
//
//
-(void)setTheResearchRate:(float)rate
{
    if(theChangesAreNestedFlag == YES)
    {
        return;
    }
    
    theChangesAreNestedFlag = YES;
    
    //
    //
    //
    theResearchRate            = rate;
    
    float       ratioOfOtherTwo     = (theFactoryBuildRate/theMineBuildRate);
    float       remainder           = 1.0 - rate;
    
    //
    //
    //
    [self willChangeValueForKey:@"theMineBuildRate"];
    [self willChangeValueForKey:@"theFactoryBuildRate"];

    if(isnan(ratioOfOtherTwo))
    {
        theMineBuildRate            = 0.333;
        theFactoryBuildRate         = 0.333;
        theResearchRate             = 0.333;
    }
    else
    {
        //
        //
        //
        theFactoryBuildRate     = (remainder * ratioOfOtherTwo) / (ratioOfOtherTwo + 1.0);
        theMineBuildRate         = remainder / (ratioOfOtherTwo + 1.0);
    }
    
    //
    //
    //
    [self didChangeValueForKey:@"theMineBuildRate"];
    [self didChangeValueForKey:@"theFactoryBuildRate"];
    
    theChangesAreNestedFlag = NO;
}

@end
