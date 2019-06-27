//
//  Motivation.m
//  Starz
//
//  Created by Steve Tickle on 12/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Motivation.h"


@implementation Motivation


//
//
//
-(NSMutableArray*)generateGoals
{
    NSMutableArray*     goalList    = [[NSMutableArray alloc] init];
    Goal*               newGoal;
    
    //
    // Generate some Goals.
    //
    newGoal = [[Goal alloc] init];
    [goalList addObject:newGoal];
    
    newGoal = [[Goal alloc] init];
    [goalList addObject:newGoal];
    
    newGoal = [[Goal alloc] init];
    [goalList addObject:newGoal];
    
    return goalList;    
}


@end
