//
//  TechnologyItem.h
//  Starz
//
//  Created by Steve Tickle on 02/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class TechFixing;

@interface TechnologyItem :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSNumber * Boranium;
@property (nonatomic, retain) NSNumber * Ironium;
@property (nonatomic, retain) NSNumber * Germanium;
@property (nonatomic, retain) NSSet* TechnologyItemToTechFixing;

@end


@interface TechnologyItem (CoreDataGeneratedAccessors)
- (void)addTechnologyItemToTechFixingObject:(TechFixing *)value;
- (void)removeTechnologyItemToTechFixingObject:(TechFixing *)value;
- (void)addTechnologyItemToTechFixing:(NSSet *)value;
- (void)removeTechnologyItemToTechFixing:(NSSet *)value;

@end

