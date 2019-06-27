//
//  UniverseModel.h
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "CoreDataUser.h"
#import "Planet.h"
#import "Game.h"
#import "ProductionQueueItem.h"

@interface UniverseModel : CoreDataUser
{
    float       theUniverseDensity;
    float       theUniverseSize;

}

@property (assign) float    theUniverseDensity;
@property (assign) float    theUniverseSize;



+ (UniverseModel *)instance;
-(void)loadDefaults;

-(NSArray*)allPlanets;
-(void)addPlanet:(Game*)game XPosition:(int)x YPosition:(int)y Name:(NSString*)name;
- (Planet*)planetClosestToPoint:(CGPoint)point;
-(ProductionQueueItem*)addProductionQueueItemToPlanet:(Planet*)planet;


@end
