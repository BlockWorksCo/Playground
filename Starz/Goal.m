//
//  Goal.m
//  Starz
//
//  Created by Steve Tickle on 08/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Goal.h"


@implementation Goal




//
//
//
+(NSArray*)effects
{
    return nil;
}

//
//
//
-(void)elaborate:(NSMutableArray*)goalList
{
    Goal*   newGoal = [[Goal alloc] init];
    
    [goalList addObject:newGoal];
}


@end
