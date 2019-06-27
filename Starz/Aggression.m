//
//  Aggression.m
//  Starz
//
//  Created by Steve Tickle on 13/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Aggression.h"


@implementation Aggression

//
//
//
+(NSArray*)effects
{
    static NSArray*     stringList  = nil;
    
    if(stringList == nil)
    {
        stringList  = [[NSArray alloc] initWithObjects:@"Aggression"];
    }
    
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
