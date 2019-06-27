//
//  PlayerModel.m
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "PlayerModel.h"
#import "GameModel.h"
#import "Model.h"


@implementation PlayerModel


//
// Maintain the singleton.
//
+ (PlayerModel *)instance
{
    static PlayerModel*   instance  = nil;
    
    @synchronized(self)
    {
        if (instance == nil)
        {
            instance = [[PlayerModel alloc] init];
            //[instance openDatabase:@"PlayerModel.sqlite"];
            [instance openDatabaseWithModel:[[Model instance] managedObjectModel] StoreCoordinator:[[Model instance] persistentStoreCoordinator] ObjectContext:[[Model instance] managedObjectContext]];

        }
        
        return instance;
    }
    
    return NULL;
}







//
//
//
-(void)loadDefaults
{
    NSString*   filePath;
    NSArray*    items;
    
    //
    // Load the production items into the DB from the plist.
    //
    items   = [[PlayerModel instance] allProductionItems];
    if([items count] == 0)
    {
        filePath    = [[NSBundle mainBundle] pathForResource:@"DefaultProductionItems" ofType:@"plist"];
        items       = [[NSArray alloc] initWithContentsOfFile:filePath];
        
        for(int i=0; i<[items count]; i++)
        {
            NSString*   name    = [[items objectAtIndex:i] valueForKey:@"Name"];
            NSString*   keypath = [[items objectAtIndex:i] valueForKey:@"KeyPath"];
            
            NSLog(@"name=%@ keyPath=%@",name,keypath);
            ProductionItem*     newItem   = [NSEntityDescription insertNewObjectForEntityForName:@"ProductionItem" inManagedObjectContext:managedObjectContext];
            [newItem setName:name];
            [newItem setKeyPath:keypath];
        }
    }
    
    
}






//
// Return all the ShipTemplates in the Db.
//
-(NSArray*)allShipTemplatesWithHullType:(int)type
{
    NSString*   query   = [NSString stringWithFormat:@"ShipTemplateToHull.Type == %d", type];
    NSArray*    items   = [self fetchEntitiesOfName:@"ShipTemplate" withPredicate:query andSortField:@"Name"];
    
    return items;
}




//
// Return all the ShipTemplates in the Db.
//
-(NSArray*)allShipTemplatesForName:(NSString*)name
{
    NSString*   query   = [NSString stringWithFormat:@"Name == \"%@\"", name];
    NSArray*    items   = [self fetchEntitiesOfName:@"ShipTemplate" withPredicate:query andSortField:@"Name"];
    
    return items;
}






//
// Add a ShipTemplate object with the given name and hull.
//
-(ShipTemplate*)addShipTemplateWithName:(NSString*)name andHull:(Hull*)hull
{
    ShipTemplate*   entry   = (ShipTemplate*)[self addEntityOfClass:@"ShipTemplate"];
    
    // 
    // Set up all the required items.
    //
    [entry setName:name];
    [entry setShipTemplateToHull:hull];
    
    return entry;
}




//
// Return the existing ShipTemplate for the given name or create a new one with the
// given name and Hull.
//
-(ShipTemplate*)getShipTemplateForName:(NSString*)name AndHull:(Hull*)hull
{
    NSArray*        itemArray   = [self allShipTemplatesForName:name];
    ShipTemplate*   item        = nil;
    
    //
    // If there is an item in the array, return the first one, otherwise create a new one.
    //
    if([itemArray count] > 0)
    {
        item    = (ShipTemplate*)[itemArray objectAtIndex:0];
    }
    else
    {
        item    = [self addShipTemplateWithName:name andHull:hull];
    }
    
    return item;
}







// 
// Return all the TechFixings for a particular ShipTemplate
//
-(NSArray*)allTechFixingsForShipTemplate:(ShipTemplate*)template
{    
    NSString*   query   = [NSString stringWithFormat:@"TechFixingToShipTemplate.Name == \"%@\"",[template Name]];
    NSArray*    items   = [self fetchEntitiesOfName:@"TechFixing" withPredicate:query andSortField:@"yPosition"];
    
    return items;
}



//
// Get the techFixing on the ShipTemplate with the given name at the given point.
//
-(NSArray*)techFixingsForShipTemplate:(ShipTemplate*)template atPosition:(CGPoint)point
{    
    NSString*   query   = [NSString stringWithFormat:@"(TechFixingToShipTemplate.Name == \"%@\") AND (xPosition == %f) AND (yPosition == %f)", [template Name], point.x, point.y];
    NSArray*    items   = [self fetchEntitiesOfName:@"TechFixing" withPredicate:query andSortField:@"yPosition"];
    
    return items;    
}



//
// Add a TechFixing with the given TechItem to the Db.
//
-(TechFixing*)addTechFixingForItem:(TechnologyItem*)item toShipTemplate:(ShipTemplate*)template atPosition:(CGPoint)point
{
    TechFixing*     newObject       = (TechFixing*)[self addEntityOfClass:@"TechFixing"];
    
    // 
    // Set up all the required items.
    //
    [newObject setXPosition:[NSNumber numberWithInt:point.x]];
    [newObject setYPosition:[NSNumber numberWithInt:point.y]];
    [newObject setTechFixingToShipTemplate:template];
    [newObject setTechFixingToTechnologyItem:item];
    
    return newObject;
}



//
// Return all the ProductionItems in the Db.
//
-(NSArray*)allProductionItems
{
    NSArray*    items   = [self fetchEntitiesOfName:@"ProductionItem" withPredicate:nil andSortField:@"Name"];
    
    return items;    
}











@end
