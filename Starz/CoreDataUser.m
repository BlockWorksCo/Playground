//
//  CoreDataUser.m
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "CoreDataUser.h"


@implementation CoreDataUser

@synthesize managedObjectContext;
@synthesize managedObjectModel;
@synthesize persistentStoreCoordinator;


//
// Returns the path to the application's documents directory.
//
- (NSString *)applicationDocumentsDirectory 
{
    NSArray*    paths       = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString*   basePath    = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
    
    return basePath;
}



//
// Open the database
//
-(void)openDatabaseWithModel:(NSManagedObjectModel*)pManagedObjectModel StoreCoordinator:(NSPersistentStoreCoordinator*)pStoreCoordinator ObjectContext:(NSManagedObjectContext*)pContext
{
    //
    // Use the supplied objects to access the Db instead of opening another file ourselves.
    //
    managedObjectModel          = pManagedObjectModel;
    persistentStoreCoordinator  = pStoreCoordinator;
    managedObjectContext        = pContext;
}    

//
// Open the database.
//
-(void)openDatabase:(NSString*)fileName
{
	NSError*    error;

    //
    // Create the ManagedObjectModel
    //
    managedObjectModel = [[NSManagedObjectModel mergedModelFromBundles:nil] retain];    
    
    // 
    // Create the PersistentStoreCoordinator
    //
    NSURL*  storeUrl    = [NSURL fileURLWithPath: [[self applicationDocumentsDirectory] stringByAppendingPathComponent:fileName]];
	
    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:managedObjectModel];
    if ([persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:nil error:&error] == 0)
    {
        NSLog([error description]);
    }    
    
    //
    // Create the managedObjectContext
    //
    if(persistentStoreCoordinator != nil)
    {
        managedObjectContext = [[NSManagedObjectContext alloc] init];
        [managedObjectContext setPersistentStoreCoordinator:persistentStoreCoordinator];
    }
}




//
// Get the techFixing on the ShipTemplate with the given name at the given point.
//
-(NSArray*)fetchEntitiesOfName:(NSString*)entityName withPredicate:(NSString*)predicateText andSortField:(NSString*)sortField
{    
    NSArray*    itemArray     = 0;
    
    //
	// Create the fetch request for the entity.
    //
	NSFetchRequest*     fetchRequest        = [[NSFetchRequest alloc] init];
	NSSortDescriptor*   sortDescriptor      = [[NSSortDescriptor alloc] initWithKey:sortField ascending:YES];
	NSArray*            sortDescriptors     = [[NSArray alloc] initWithObjects:sortDescriptor, nil];	
	[fetchRequest setSortDescriptors:sortDescriptors];
    
    //
    // Lets add a predicate to perform our search.
    //
    if(predicateText != nil)
    {
        NSPredicate*      predicate           = [NSPredicate predicateWithFormat:predicateText];
        [fetchRequest setPredicate:predicate];
    }
    
    //
	// Get an object of the correct type and give it to the fetchRequest.
    //
	NSEntityDescription*     entity          = [NSEntityDescription entityForName:entityName inManagedObjectContext:managedObjectContext];
	[fetchRequest setEntity:entity];
	
    //
    // Create a controller to do the fetching.
    //
	NSFetchedResultsController* controller   = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest managedObjectContext:managedObjectContext sectionNameKeyPath:nil cacheName:@"Root"];
    
    //
    // Perform the fetch.
    //
	NSError *error;
	if ([controller performFetch:&error] == 0)
    {
        NSLog(@"Problems performing the fetch: %@", [error description]);
	}
    
    //
    // Get the NSArray of data from the controller.
    //
    NSArray*    sections    = [controller sections];
    if([sections count] > 0)
    {
        id<NSFetchedResultsSectionInfo> section = [sections objectAtIndex:0];
        
        if([section numberOfObjects] > 0)
        {
            itemArray   = [section objects];
        }
    }
    
    //
    // Clean up.
    //
	[controller release];
	[fetchRequest release];
    
    return itemArray;
    
}




//
//
//
-(NSManagedObject*)addEntityOfClass:(NSString*)className
{
    NSManagedObject*    newObject = [NSEntityDescription insertNewObjectForEntityForName:className inManagedObjectContext:managedObjectContext];
    
    //return (NSClassFromString(className) *)entry;
    return newObject;
}


//
//
//
-(void)deleteObject:(NSManagedObject*)object
{
    //
    // Delete the managed object for the given index path
    //
    [managedObjectContext deleteObject:object];
    
    //
    // Save the context.
    //
    NSError*    error;
    if ([managedObjectContext save:&error] == 0)
    {
        NSLog(@"Problems in deleteObject: %@",[error description]);
    }
}



//
// Performs the save action for the application, which is to send the save:
// message to the application's managed object context.
//
-(void)save
{
    NSError*    error;
    
    //
    //
    //
    if ([managedObjectContext save:&error] == NO) 
    {
		NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
		exit(-1);  // Fail
    }
}







@end
