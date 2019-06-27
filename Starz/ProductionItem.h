//
//  ProductionItem.h
//  Starz
//
//  Created by Steve Tickle on 16/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class ProductionQueueItem;

@interface ProductionItem :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * KeyPath;
@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSSet* ProductionItemToProductionQueueItem;

@end


@interface ProductionItem (CoreDataGeneratedAccessors)
- (void)addProductionItemToProductionQueueItemObject:(ProductionQueueItem *)value;
- (void)removeProductionItemToProductionQueueItemObject:(ProductionQueueItem *)value;
- (void)addProductionItemToProductionQueueItem:(NSSet *)value;
- (void)removeProductionItemToProductionQueueItem:(NSSet *)value;

@end

