//
//  Model.m
//  Starz
//
//  Created by Steve Tickle on 09/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Model.h"
#import "Planet.h"
#import "ProductionQueueItem.h"
#import "ProductionItem.h"
#import "TechnologyItem.h"
#import "ShipTemplate.h"
#import "TechFixing.h"
#import "GameModel.h"

@implementation Model


//
//
//




//
// Maintain the singleton.
//
+(Model*)instance
{
    static Model*   instance    = nil;
    
    @synchronized(self)
    {
        if (instance == nil)
        {
            instance = [[Model alloc] init];
            //[instance openDatabase:@"StarzModel.sqlite"];
            
            //
            //
            //
            //NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"asteroids" ofType:@"txt"];
            //NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"countries" ofType:@"txt"];
            //NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"alien_races" ofType:@"txt"];
            
            //[instance setTheNameGenerator:[[NameGenerator alloc] initWithFile:fileName]];
        }
        
        return instance;
    }
    
    return NULL;
}




//
//
//
-(void)setTheUniverseSize:(float)size
{
    if([[GameModel instance] theCurrentGame] != nil)
    {
        [[[GameModel instance] theCurrentGame] setUniverseSideLength:[NSNumber numberWithFloat:size]];
    }
    else
    {
        NSLog(@"No current game");
    }
}

-(float)theUniverseSize
{
    if([[GameModel instance] theCurrentGame] != nil)
    {
        float   size    = [[[[GameModel instance] theCurrentGame] universeSideLength] floatValue];
        return size;
    }
    else
    {
        NSLog(@"No current game");
        return 0.0;
    }
}






/*
- (void)writeToPlist
{
    NSString*               filePath    = [[NSBundle mainBundle] pathForResource:@"DefaultProductionItems" ofType:@"plist"];
    NSMutableArray*         array       = [[NSMutableArray alloc] init];
    NSMutableDictionary*    row;
    //NSMutableDictionary*    row         = [[NSMutableDictionary alloc] initWithContentsOfFile:filePath];
    
    //
    //
    //
    row         = [[NSMutableDictionary alloc] init];
    [row setValue:@"Ship One" forKey:@"Name"];
    [row setValue:@"keypath1" forKey:@"KeyPath"];
    [array addObject:row];
    
    row         = [[NSMutableDictionary alloc] init];
    [row setValue:@"Ship Two" forKey:@"Name"];
    [row setValue:@"keypath2" forKey:@"KeyPath"];
    [array addObject:row];
    
    row         = [[NSMutableDictionary alloc] init];
    [row setValue:@"Ship Three" forKey:@"Name"];
    [row setValue:@"keypath3" forKey:@"KeyPath"];
    [array addObject:row];
    
    //
    //
    //
    [array writeToFile:filePath atomically:YES];
}
 */












@end
