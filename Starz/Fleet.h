//
//  Fleet.h
//  Starz
//
//  Created by Steve Tickle on 26/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>
#import "SpaceObject.h"

@class Ship;

@interface Fleet :  SpaceObject  
{
}

@property (nonatomic, retain) NSSet* FleetToShip;

@end


@interface Fleet (CoreDataGeneratedAccessors)
- (void)addFleetToShipObject:(Ship *)value;
- (void)removeFleetToShipObject:(Ship *)value;
- (void)addFleetToShip:(NSSet *)value;
- (void)removeFleetToShip:(NSSet *)value;

@end

