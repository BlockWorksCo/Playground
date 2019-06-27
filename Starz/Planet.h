//
//  Planet.h
//  Starz
//
//  Created by Steve Tickle on 14/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>
#import "SpaceObject.h"

@class ProductionQueueItem;

@interface Planet :  SpaceObject  
{
}

@property (nonatomic, retain) NSNumber * MineCount;
@property (nonatomic, retain) NSString * StarbaseType;
@property (nonatomic, retain) NSNumber * FactoryCount;
@property (nonatomic, retain) NSString * ScannerType;
@property (nonatomic, retain) NSNumber * Population;
@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSManagedObject * PlanetToPlayer;
@property (nonatomic, retain) NSManagedObject * PlanetToGame;
@property (nonatomic, retain) NSSet* PlanetToProductionQueueItem;

@end


@interface Planet (CoreDataGeneratedAccessors)
- (void)addPlanetToProductionQueueItemObject:(ProductionQueueItem *)value;
- (void)removePlanetToProductionQueueItemObject:(ProductionQueueItem *)value;
- (void)addPlanetToProductionQueueItem:(NSSet *)value;
- (void)removePlanetToProductionQueueItem:(NSSet *)value;

@end

