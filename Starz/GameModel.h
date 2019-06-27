//
//  GameModel.h
//  Starz
//
//  Created by Steve Tickle on 06/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "CoreDataUser.h"
#import "Game.h"
#import "ProductionItem.h"
#import "LogEntry.h"
#import "NameGenerator.h"
#import "AIPlayer.h"
#import "HumanPlayer.h"



@interface GameModel : CoreDataUser
{
    NSString*                       theGameName;
    Game*                           theCurrentGame;  
    NameGenerator*                  theNameGenerator;
    
    HumanPlayer*                    theHumanPlayer;
    AIPlayer*                       theAIPlayer;

}


@property (retain) NSString*        theGameName;
@property (retain) Game*            theCurrentGame;
@property (retain) NameGenerator*   theNameGenerator;
@property (assign) HumanPlayer*     theHumanPlayer;
@property (assign) AIPlayer*        theAIPlayer;


+ (GameModel *)instance;
-(void)loadDefaults;

-(NSArray*)allHulls;
-(NSArray*)allTechnologyItems;
-(NSArray*)allGames;

-(LogEntry*)addLogEntry:(NSString*)text ForYear:(int)year ToGame:(Game*)game;
-(void)addLogEntry:(NSString*)text;

- (BOOL)isSetup;
- (Game*)gameForName:(NSString*)name;

-(LogEntry*)addLogEntry:(NSString*)text ForYear:(int)year ToGame:(Game*)game;
-(NSArray*)allLogEntriesForGame:(Game*)game;
-(void)addLogEntry:(NSString*)text;

-(int)numberOfGames;
-(void)setupGame;
-(Game*)addNewGame;
-(void)removeGame:(Game*)game;

-(NSArray*)getGameList;

-(NSString*)gamePath;
-(void)createOrOpenGame:(NSString*)name;




@end
