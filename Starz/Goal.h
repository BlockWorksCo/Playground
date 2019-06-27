//
//  Goal.h
//  Starz
//
//  Created by Steve Tickle on 08/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Condition.h"



//
//
//
typedef struct 
{
    id      effect;
    double  strength;
    
} Effect;


//
//
//
@interface Goal : NSObject
{
    NSMutableArray*     resultConditions;
}


+(NSArray*)effects;
-(void)elaborate:(NSMutableArray*)goalList;

@end
