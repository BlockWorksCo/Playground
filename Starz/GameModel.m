//
//  GameModel.m
//  Starz
//
//  Created by Steve Tickle on 06/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "Model.h"
#import "GameModel.h"
#import "PlayerModel.h"
#import "UniverseModel.h"


@implementation GameModel


@synthesize theGameName;
@synthesize theCurrentGame;
@synthesize theNameGenerator;
@synthesize theHumanPlayer;
@synthesize theAIPlayer;



//
// Maintain the singleton.
//
+ (GameModel *)instance
{
    static GameModel*   instance    = nil;
    
    @synchronized(self)
    {
        if (instance == nil)
        {
            instance = [[GameModel alloc] init];
            //[instance openDatabase:@"GameModel.sqlite"];
            //[instance openDatabaseWithModel:[[Model instance] managedObjectModel] StoreCoordinator:[[Model instance] persistentStoreCoordinator] ObjectContext:[[Model instance] managedObjectContext]];
            
            //
            //
            //
            //[instance setTheGameName:@"<None>"];
            [instance setTheHumanPlayer:[[[HumanPlayer alloc] init] retain]];
            [instance setTheAIPlayer:[[[AIPlayer alloc] init] retain]];
            
            //
            //
            //
            [instance getGameList];

            
            //
            //
            //
            //NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"asteroids" ofType:@"txt"];
            //NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"countries" ofType:@"txt"];
            NSString*       fileName        = [[NSBundle mainBundle] pathForResource: @"alien_races" ofType:@"txt"];
            
            [instance setTheNameGenerator:[[NameGenerator alloc] initWithFile:fileName]];
        }
        
        return instance;
    }
    
    return NULL;
}







//
//
//

//
//
//
-(void)loadDefaults
{
    NSString*   filePath;
    NSArray*    items;
    
    
    //
    // Load the TechnologyItems records into the DB from the plist.
    //
    items       = [self allTechnologyItems];
    if([items count] == 0)
    {
        filePath    = [[NSBundle mainBundle] pathForResource:@"DefaultTechnologyItems" ofType:@"plist"];
        items       = [[NSArray alloc] initWithContentsOfFile:filePath];
        
        for(int i=0; i<[items count]; i++)
        {
            NSString*   name        = [[items objectAtIndex:i] valueForKey:@"Name"];
            NSNumber*   ger         = [[items objectAtIndex:i] valueForKey:@"Germanium"];
            NSNumber*   iro         = [[items objectAtIndex:i] valueForKey:@"Ironium"];
            NSNumber*   bor         = [[items objectAtIndex:i] valueForKey:@"Boranium"];
            
            NSLog(@"name=%@ bor=%@ iro=%@ ger=%@",name,bor,iro,ger);
            TechnologyItem*     newItem   = [NSEntityDescription insertNewObjectForEntityForName:@"TechnologyItem" inManagedObjectContext:managedObjectContext];
            [newItem setName:name];
            [newItem setGermanium:ger];
            [newItem setIronium:iro];
            [newItem setBoranium:bor];
        }
    }
    
    
    
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
            NSNumber*   type        = [[items objectAtIndex:i] valueForKey:@"Type"];
            
            NSLog(@"name=%@ shapeMask=%@",name,shapeMask);
            Hull*     newItem   = [NSEntityDescription insertNewObjectForEntityForName:@"Hull" inManagedObjectContext:managedObjectContext];
            [newItem setName:name];
            [newItem setShapeMask:shapeMask];
            [newItem setType:type];
        }
    }
    
}





//
// Called to add a new game to the DB.
//
- (Game*)addNewGame
{    
    Game*   newGame   = (Game*)[self addEntityOfClass:@"Game"];
    
    // 
    // Set up all the required items.
    //

    //
    // Create a string from the current data and time.
    //
    NSDate*                 date                = [NSDate date];
    NSDateFormatter*        dateFormatter       = [[[NSDateFormatter alloc] init] autorelease];
    NSString*               dateString;
    
    //
    // Set the date style.
    //
    [dateFormatter setDateStyle:NSDateFormatterLongStyle];
    [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
    dateString          = [dateFormatter stringFromDate:date];
    
    //
	// Set the name of the new Game object to the date/time string.
    //
    [newGame setName:dateString];
    [newGame setEndYear:[NSNumber numberWithInteger:2400]];
    [newGame setNumberOfPlayers:[NSNumber numberWithInteger:2]];
    [newGame setSize:[NSNumber numberWithInteger:2400]];
    [newGame setCurrentYear:[NSNumber numberWithInteger:2400]];
    [newGame setUniverseSideLength:[NSNumber numberWithFloat:2048]];
    [self save];
    
    
    //
    // Load the defaults if they arent already there.
    //
    [self loadDefaults];
    
    //
    //
    //
    [self addLogEntry:@"New Game created" ForYear:2100 ToGame:newGame];
    
    //
    //
    //
    [self save];    
    
    return newGame;
}









//
// Return all the Hulls in the Db.
//
-(NSArray*)allHulls
{
    NSArray*    items   = [self fetchEntitiesOfName:@"Hull" withPredicate:nil andSortField:@"Name"];
    
    return items;
    
}



//
// Add a log entry to the given game.
//
-(LogEntry*)addLogEntry:(NSString*)text ForYear:(int)year ToGame:(Game*)game
{
    LogEntry*   entry   = (LogEntry*)[self addEntityOfClass:@"LogEntry"];

    [entry setText:text];
    [entry setYear:[NSNumber numberWithInt:year]];
    
    [game addGameToLogEntryObject:entry];
    
    return entry;
}


//
// Helper function to log an entry for the current game year.
//
-(void)addLogEntry:(NSString*)text
{
    [self addLogEntry:text ForYear:[[theCurrentGame CurrentYear] intValue] ToGame:theCurrentGame];
}




//
//
//
- (BOOL)isSetup
{
    NSArray*    planetArray = [[UniverseModel instance] allPlanets];
    
    if(planetArray != 0)
    {
        if([planetArray count] > 0)
        {
            return YES;
        }
    }
    
    return NO;
}


//
//
//
- (void)setTheGameName:(NSString*)name
{
    theGameName         = name;
    theCurrentGame      = [self gameForName:name];
}



//
//
//
- (Game*)gameForName:(NSString*)name
{
    NSString*   query   = [NSString stringWithFormat:@"Name=\"%@\"",name];
    NSArray*    items   = [self fetchEntitiesOfName:@"Game" withPredicate:query andSortField:@"Name"];
    Game*       game    = [items objectAtIndex:0];
    
    return game;    
}






//
//
//
-(NSArray*)allLogEntriesForGame:(Game*)game
{
    NSString*   query   = [NSString stringWithFormat:@"LogEntryToGame == %@",[[GameModel instance] theCurrentGame]];
    NSArray*    items   = [self fetchEntitiesOfName:@"LogEntry" withPredicate:query andSortField:@"Year"];
    
    return items;

}



//
// Return all the TechnologyItems in the Db.
//
-(NSArray*)allTechnologyItems
{
    NSArray*    items   = [self fetchEntitiesOfName:@"TechnologyItem" withPredicate:nil andSortField:@"Name"];
    
    return items;
}



//
// Return all the Games in the Db.
//
-(NSArray*)allGames
{
    NSArray*    items   = [self fetchEntitiesOfName:@"Game" withPredicate:nil andSortField:@"Name"];
    
    return items;
}





//
//
//
- (int)numberOfGames
{
    return [[self allGames] count];
}




//
//
//
- (void)setupGame
{
    int     planetCount = 0;
    int     sideLength  = ([[UniverseModel instance] theUniverseSize] * 4096.0) - 200;
    float   density     = [[UniverseModel instance] theUniverseDensity];
    
    //
    // If the game has been setup, go to the map, otherwise to the setup view.
    //
    if([[GameModel instance] isSetup] == NO)
    {
        //
        // Create the planets.
        //
        for(int i=0; i<sideLength; i+=100)
        {
            for(int j=0; j<sideLength; j+=100)
            {
                float       chance      = ((float)(rand()%1000))/1000.0;
                
                if(chance < density)
                {
                    NSString*   planetName  = [theNameGenerator nextName];    
                    int         x           = 100+(((int)i) + ((rand()%25)-10));
                    int         y           = 100+(((int)j) + ((rand()%25)-10));
                    
                    [[UniverseModel instance] addPlanet:[[GameModel instance] theCurrentGame]  XPosition:(int)x YPosition:(int)y Name:planetName];
                    planetCount++;
                }
            }
        }
        
        //
        // Load the defaults
        //
        [self loadDefaults];
        [[PlayerModel instance] loadDefaults];
        [[UniverseModel instance] loadDefaults];
        
        [self save];
    }
}






//
//
//
-(void)removeGame:(Game*)game
{
    [self deleteObject:game];
#if 0
    //
    // Delete the managed object for the given index path
    //
    NSManagedObjectContext*     context     = [fetchedResultsController managedObjectContext];
    [context deleteObject:[fetchedResultsController objectAtIndexPath:indexPath]];
    
    //
    // Save the context.
    //
    NSError *error;
    if (![context save:&error])
    {
        // Handle the error...
    }
#endif
}





//
//
//
-(NSString*)gamePath
{
    NSString*   path    = [NSString stringWithFormat:@"StarzGames/"];
    //NSString*   path    = [NSString stringWithFormat:@"%@/Documents/StarzGames",NSHomeDirectory()];
    
    return path;
}


//
//
//
-(void)createOrOpenGame:(NSString*)name
{
    NSString*   fileName    = [NSString stringWithFormat:@"%@%@.sqlite",[self gamePath],name];
    
    //
    // Open the database with the given name.
    //
    [[GameModel instance] openDatabase:fileName];
    [[PlayerModel instance] openDatabaseWithModel:[self managedObjectModel] StoreCoordinator:[self persistentStoreCoordinator] ObjectContext:[self managedObjectContext]];
    [[UniverseModel instance] openDatabaseWithModel:[self managedObjectModel] StoreCoordinator:[self persistentStoreCoordinator] ObjectContext:[self managedObjectContext]];

    [[GameModel instance] save];

}


//
//
//
-(NSArray*)getGameList
{
    //NSURL*  storeUrl    = [NSURL fileURLWithPath: [[self applicationDocumentsDirectory] stringByAppendingPathComponent:[self gamePath]]];
    
    //
    // Attempt to create the game directory, if it fails, it must already exist.
    //
    NSString*   fullGamePath    = [NSString stringWithFormat:@"%@/%@",[self applicationDocumentsDirectory],[self gamePath]];
    [[NSFileManager defaultManager] createDirectoryAtPath:fullGamePath attributes:nil];
    
    //
    // Read the contents of the game directory.
    //
    NSDirectoryEnumerator*  enumerator  = [[NSFileManager defaultManager] enumeratorAtPath:fullGamePath];
    NSMutableArray*         gameList    = [[NSMutableArray alloc] init];
    NSString*               fileName;
    
    //
    // Enumerate all the files in the directory and pick out the games.
    //
    while((fileName = [enumerator nextObject]) != 0)
    {
        if([[fileName pathExtension] isEqualToString:@"sqlite"] != NO)
        {
            NSString*    nameAndExtension  = [fileName lastPathComponent];
            NSString*   name    = [[nameAndExtension componentsSeparatedByString:@"."] objectAtIndex:0];
            [gameList addObject:name];
        }
    }
    NSLog(@"%@",gameList);
    
    return gameList;
}








@end
