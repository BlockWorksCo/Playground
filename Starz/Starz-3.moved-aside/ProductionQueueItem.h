//
//  ProductionQueueItem.h
//  Starz
//
//  Created by Steve Tickle on 14/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Planet;

@interface ProductionQueueItem :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * Priority;
@property (nonatomic, retain) Planet * ProductionQueueItemToPlanet;

@end



