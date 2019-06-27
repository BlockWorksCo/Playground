//
//  TechnologyItem.h
//  Starz
//
//  Created by Steve Tickle on 19/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface TechnologyItem :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSNumber * Boranium;
@property (nonatomic, retain) NSNumber * Ironium;
@property (nonatomic, retain) NSNumber * Germanium;

@end



