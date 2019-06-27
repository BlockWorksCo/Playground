//
//  Planner.h
//  Starz
//
//  Created by Steve Tickle on 08/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Goal.h"
#import "Motivation.h"
#import "Task.h"


@interface Planner : NSObject
{
}

-(void)elaborateGoals:(NSMutableArray*)goals;

@end
