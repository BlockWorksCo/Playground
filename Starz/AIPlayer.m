//
//  AIPlayer.m
//  Starz
//
//  Created by Steve Tickle on 29/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "AIPlayer.h"

#import "Expansion.h"
#import "Aggression.h"
#import "GrowPopulation.h"
#import "DecreaseResearch.h"
#import "IncreaseResearch.h"
#import "IncreaseMineProduction.h"
#import "DecreaseMineProduction.h"
#import "IncreaseFactoryProduction.h"
#import "DecreaseFactoryProduction.h"

@implementation AIPlayer

@synthesize theIntelligenceLevel;



//
//
//
-(id)init
{
    self    = [super init];
    if(self != nil)
    {
        //
        //
        //
        theIntelligenceLevel    = 0.5;
        
        //
        // Create the AIEngine, the Motivations and add them to the AIEngine.
        //
        aiEngine        = [[AIEngine alloc] init];
        
        Aggression*         m1  = [[Aggression alloc] init];
        Expansion*          m2  = [[Expansion alloc] init];
        GrowPopulation*     m3  = [[GrowPopulation alloc] init];
        
        [aiEngine addMotivation:m1];
        [aiEngine addMotivation:m2];
        [aiEngine addMotivation:m3];
        
        //
        //
        //
        goalClassLibrary    = [[NSMutableArray alloc] init];
        [goalClassLibrary addObject:[DecreaseResearch class]];
        [goalClassLibrary addObject:[IncreaseResearch class]];
        [goalClassLibrary addObject:[IncreaseMineProduction class]];
        [goalClassLibrary addObject:[DecreaseMineProduction class]];
        [goalClassLibrary addObject:[IncreaseFactoryProduction class]];
        [goalClassLibrary addObject:[DecreaseFactoryProduction class]];
    }
    
    return self;
}


//
//
//
-(void)play
{
    [aiEngine generateTasks:goalClassLibrary];
}




@end
