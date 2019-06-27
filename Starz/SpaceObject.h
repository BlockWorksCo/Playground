//
//  SpaceObject.h
//  Starz
//
//  Created by Steve Tickle on 14/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface SpaceObject :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * XPosition;
@property (nonatomic, retain) NSNumber * Diameter;
@property (nonatomic, retain) NSNumber * YPosition;

@end



