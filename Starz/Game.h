//
//  Game.h
//  Starz
//
//  Created by Steve Tickle on 26/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>

@class LogEntry;
@class Planet;

@interface Game :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * Name;
@property (nonatomic, retain) NSNumber * Size;
@property (nonatomic, retain) NSString * backgroundImage;
@property (nonatomic, retain) NSNumber * EndYear;
@property (nonatomic, retain) NSNumber * NumberOfPlayers;
@property (nonatomic, retain) NSNumber * universeSideLength;
@property (nonatomic, retain) NSNumber * CurrentYear;
@property (nonatomic, retain) NSSet* GameToPlayers;
@property (nonatomic, retain) NSSet* GameToLogEntry;
@property (nonatomic, retain) NSSet* GameToPlanets;

@end


@interface Game (CoreDataGeneratedAccessors)
- (void)addGameToPlayersObject:(NSManagedObject *)value;
- (void)removeGameToPlayersObject:(NSManagedObject *)value;
- (void)addGameToPlayers:(NSSet *)value;
- (void)removeGameToPlayers:(NSSet *)value;

- (void)addGameToLogEntryObject:(LogEntry *)value;
- (void)removeGameToLogEntryObject:(LogEntry *)value;
- (void)addGameToLogEntry:(NSSet *)value;
- (void)removeGameToLogEntry:(NSSet *)value;

- (void)addGameToPlanetsObject:(Planet *)value;
- (void)removeGameToPlanetsObject:(Planet *)value;
- (void)addGameToPlanets:(NSSet *)value;
- (void)removeGameToPlanets:(NSSet *)value;

@end

