//
//  DecreaseMineProduction.m
//  Starz
//
//  Created by Steve Tickle on 13/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "DecreaseMineProduction.h"


@implementation DecreaseMineProduction

//
//
//
+(NSArray*)effects
{
    static NSArray*     stringList  = nil;
    
    if(stringList == nil)
    {
        stringList  = [[NSArray alloc] initWithObjects:@"MineProductionDown"];
    }
    
    return nil;
}


@end
