//
//  IncreaseResearch.m
//  Starz
//
//  Created by Steve Tickle on 13/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "IncreaseResearch.h"


@implementation IncreaseResearch

//
//
//
+(NSArray*)effects
{
    static NSArray*     stringList  = nil;
    
    if(stringList == nil)
    {
        stringList  = [[NSArray alloc] initWithObjects:@"ResearchUp"];
    }
    
    return nil;
}



@end
