//
//  AIEngine.m
//  Starz
//
//  Created by Steve Tickle on 12/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "AIEngine.h"


@implementation AIEngine

@synthesize currentGoals;


//
//
//
-(id)init
{
    self    = [super init];
    if(self != nil)
    {
        currentGoals    = [[NSMutableArray alloc] init];  
        planner         = [[Planner alloc] init];
    }
    
    return self;
}



//
// Add the given Motivation to the list.
//
-(void)addMotivation:(Motivation*)motivation
{
    [motivations addObject:motivation];
}


//
// Main loop, generate sub-goals from motivations, generate objects from the class library
//
-(void)generateTasks:(NSArray*)goalClassLibrary
{
    //
    // Clear out the previous Goals.
    //
    [currentGoals removeAllObjects];
    
    //
    // Generate the top level Goals from the Motivations.
    //
    for(int i=0; i<[motivations count]; i++)
    {
        Motivation*     motivation  = [motivations objectAtIndex:i];
        NSMutableArray* newGoals    = [motivation generateGoals];
        [currentGoals addObjectsFromArray:newGoals];
    }
    
    //
    // Generate sub-Goals from Goals until:
    // - Tasks have been generated.
    // - Quiescense is acheived.
    // - Timeout.
    //
    [planner elaborateGoals:currentGoals];
}




@end
