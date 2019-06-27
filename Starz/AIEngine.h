//
//  AIEngine.h
//  Starz
//
//  Created by Steve Tickle on 12/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Planner.h"
#import "Goal.h"
#import "Motivation.h"
#import "Task.h"


@interface AIEngine : NSObject
{
    NSMutableArray*     currentGoals;
    NSMutableArray*     motivations;
    Planner*            planner;
}

@property (retain) NSMutableArray*  currentGoals;


-(id)init;
-(void)addMotivation:(Motivation*)motivation;
-(void)generateTasks:(NSArray*)goalClassLibrary;

@end
