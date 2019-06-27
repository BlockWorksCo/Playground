//
//  TechFixing.h
//  Starz
//
//  Created by Steve Tickle on 02/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class ShipTemplate;
@class TechnologyItem;

@interface TechFixing :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * xPosition;
@property (nonatomic, retain) NSNumber * yPosition;
@property (nonatomic, retain) ShipTemplate * TechFixingToShipTemplate;
@property (nonatomic, retain) TechnologyItem * TechFixingToTechnologyItem;

@end



