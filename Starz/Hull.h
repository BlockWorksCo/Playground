//
//  Hull.h
//  Starz
//
//  Created by Steve Tickle on 19/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class ShipTemplate;

@interface Hull :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * ShapeMask;
@property (nonatomic, retain) NSNumber * Type;
@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSSet* HullToShipTemplate;

@end


@interface Hull (CoreDataGeneratedAccessors)
- (void)addHullToShipTemplateObject:(ShipTemplate *)value;
- (void)removeHullToShipTemplateObject:(ShipTemplate *)value;
- (void)addHullToShipTemplate:(NSSet *)value;
- (void)removeHullToShipTemplate:(NSSet *)value;

@end

