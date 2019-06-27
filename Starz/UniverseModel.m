//
//  UniverseModel.m
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "UniverseModel.h"
#import "GameModel.h"
#import "Model.h"


@implementation UniverseModel

@synthesize theUniverseDensity;
@synthesize theUniverseSize;

//
// Maintain the singleton.
//
+ (UniverseModel *)instance
{
    static UniverseModel*   instance    = nil;
    
    @synchronized(self)
    {
        if (instance == nil)
        {
            instance = [[UniverseModel alloc] init];
            //[instance openDatabase:@"UniverseModel.sqlite"];            
            [instance openDatabaseWithModel:[[Model instance] managedObjectModel] StoreCoordinator:[[Model instance] persistentStoreCoordinator] ObjectContext:[[Model instance] managedObjectContext]];

            
            //
            //
            //
            [instance setTheUniverseSize:0.5];
            [instance setTheUniverseDensity:0.5];
            
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
#if 0
    NSString*   filePath;
    NSArray*    items;
    
    //
    // Load the Hull records into the DB from the plist.
    //
    items   = [self allHulls];
    if([items count] == 0)
    {
        filePath    = [[NSBundle mainBundle] pathForResource:@"DefaultHulls" ofType:@"plist"];
        items       = [[NSArray alloc] initWithContentsOfFile:filePath];
        
        for(int i=0; i<[items count]; i++)
        {
            NSString*   name        = [[items objectAtIndex:i] valueForKey:@"Name"];
            NSString*   shapeMask   = [[items objectAtIndex:i] valueForKey:@"ShapeMask"];
            
            NSLog(@"name=%@ shapeMask=%@",name,shapeMask);
            Hull*     newItem   = [NSEntityDescription insertNewObjectForEntityForName:@"Hull" inManagedObjectContext:managedObjectContext];
            [newItem setName:name];
            [newItem setShapeMask:shapeMask];
        }
    }
#endif
    
}




//
//
//
-(NSArray*)allPlanets
{
    NSArray*    items   = [self fetchEntitiesOfName:@"Planet" withPredicate:nil andSortField:@"Name"];
    
    return items;
}







//
//
//
-(void)addPlanet:(Game*)game XPosition:(int)x YPosition:(int)y Name:(NSString*)name
{
    int                 diameter    = (rand() % 50)+25;
    Planet*             newPlanet   = [NSEntityDescription insertNewObjectForEntityForName:@"Planet" inManagedObjectContext:managedObjectContext];
    
    [newPlanet  setValue:[NSNumber numberWithInteger:10]            forKey:@"MineCount"];
    [newPlanet  setValue:[NSNumber numberWithInteger:12]            forKey:@"FactoryCount"];
    [newPlanet  setValue:@"<No Scanner>"                            forKey:@"ScannerType"];
    [newPlanet  setValue:@"<No Starbase>"                           forKey:@"StarbaseType"];
    
    [newPlanet  setValue:[NSNumber numberWithInteger:x]             forKey:@"XPosition"];
    [newPlanet  setValue:[NSNumber numberWithInteger:y]             forKey:@"YPosition"];
    [newPlanet  setValue:[NSNumber numberWithInteger:diameter]      forKey:@"Diameter"];
    [newPlanet  setValue:name                                       forKey:@"Name"];
    [newPlanet  setValue:[NSNumber numberWithInteger:53]            forKey:@"Population"];
    [newPlanet  setValue:game  forKey:@"PlanetToGame"];
    
    //
    //
    //
    //[self addProductionQueueItemToPlanet:newPlanet];
}



//
//
//
-(float)universeSideLength
{
    return [[[[GameModel instance] theCurrentGame] universeSideLength] floatValue];
    //return (int)(theUniverseSize * 4096);
}


//
//
//
- (Planet*)planetClosestToPoint:(CGPoint)point
{
    int         xPosition       = point.x;
    int         yPosition       = point.y;
    NSArray*    planets         = [self allPlanets];
    Planet*     closestPlanet   = nil;
    int         closestDistance = INT_MAX;
    
    //
    //
    //
    for(int i=0; i<[planets count]; i++)
    {
        Planet* planet      = [planets objectAtIndex:i];        
        int     x           = [[planet valueForKey:@"XPosition"] intValue];
        int     y           = [[planet valueForKey:@"YPosition"] intValue];
        int     dx          = x - xPosition;
        int     dy          = y - yPosition;
        int     dx2         = dx*dx;
        int     dy2         = dy*dy;
        int     distanceToPoint = dx2+dy2;
        
        //
        //
        //
        if(distanceToPoint < closestDistance)
        {
            closestPlanet   = planet;
            closestDistance = distanceToPoint;
        }
    }    
    
    return closestPlanet;
    
}




//
//
//
-(ProductionQueueItem*)addProductionQueueItemToPlanet:(Planet*)planet
{
    ProductionQueueItem*    productionQueueItem = [NSEntityDescription insertNewObjectForEntityForName:@"ProductionQueueItem" inManagedObjectContext:managedObjectContext];
    
    [productionQueueItem setName:@"<Not set>"];
    [productionQueueItem setPriority:[NSNumber numberWithInteger:0]];
    
    [productionQueueItem setProductionQueueItemToPlanet:planet];
    [planet addPlanetToProductionQueueItemObject:productionQueueItem];    
    
    return productionQueueItem;
}






@end
