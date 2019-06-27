//
//  CoreDataUser.h
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface CoreDataUser : NSObject <NSFetchedResultsControllerDelegate>
{
	NSManagedObjectContext*         managedObjectContext;
    NSManagedObjectModel*           managedObjectModel;
    NSPersistentStoreCoordinator*   persistentStoreCoordinator;    
}

@property (retain) NSManagedObjectContext*       managedObjectContext;
@property (retain) NSManagedObjectModel*         managedObjectModel;
@property (retain) NSPersistentStoreCoordinator* persistentStoreCoordinator;



-(void)openDatabase:(NSString*)fileName;
-(void)openDatabaseWithModel:(NSManagedObjectModel*)pManagedObjectModel StoreCoordinator:(NSPersistentStoreCoordinator*)pStoreCoordinator ObjectContext:(NSManagedObjectContext*)pContext;

-(NSArray*)fetchEntitiesOfName:(NSString*)entityName withPredicate:(NSString*)predicateText andSortField:(NSString*)sortField;
-(NSManagedObject*)addEntityOfClass:(NSString*)className;
-(void)deleteObject:(NSManagedObject*)object;

- (NSString *)applicationDocumentsDirectory;
-(void)save;


@end
