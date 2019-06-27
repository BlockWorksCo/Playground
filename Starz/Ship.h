//
//  Ship.h
//  Starz
//
//  Created by Steve Tickle on 26/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>
#import "SpaceObject.h"

@class ShipTemplate;

@interface Ship :  SpaceObject  
{
}

@property (nonatomic, retain) NSNumber * Damage;
@property (nonatomic, retain) NSManagedObject * ShipToFleet;
@property (nonatomic, retain) NSManagedObject * ShipToPlayer;
@property (nonatomic, retain) ShipTemplate * ShipToShipTemplate;

@end



