//
//  Planner.m
//  Starz
//
//  Created by Steve Tickle on 08/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Planner.h"


@implementation Planner




//
//
//
-(void)elaborateGoals:(NSMutableArray*)goals
{
    for(int i=0; i<[goals count]; i++)
    {
        Goal*   goal    = [goals objectAtIndex:i];
        
        [goal elaborate:goals];
    }
}



@end
