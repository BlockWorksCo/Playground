//
//  ShipTemplate.h
//  Starz
//
//  Created by Steve Tickle on 27/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Hull;
@class Ship;
@class TechFixing;

@interface ShipTemplate :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) Hull * ShipTemplateToHull;
@property (nonatomic, retain) NSSet* ShipTemplateToShip;
@property (nonatomic, retain) NSSet* ShipTemplateToTechFixing;

@end


@interface ShipTemplate (CoreDataGeneratedAccessors)
- (void)addShipTemplateToShipObject:(Ship *)value;
- (void)removeShipTemplateToShipObject:(Ship *)value;
- (void)addShipTemplateToShip:(NSSet *)value;
- (void)removeShipTemplateToShip:(NSSet *)value;

- (void)addShipTemplateToTechFixingObject:(TechFixing *)value;
- (void)removeShipTemplateToTechFixingObject:(TechFixing *)value;
- (void)addShipTemplateToTechFixing:(NSSet *)value;
- (void)removeShipTemplateToTechFixing:(NSSet *)value;

@end

