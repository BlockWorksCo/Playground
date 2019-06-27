//
//  AIPlayer.h
//  Starz
//
//  Created by Steve Tickle on 29/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Player.h"
#import "AIEngine.h"

@interface AIPlayer : Player
{
    float               theIntelligenceLevel;
    AIEngine*           aiEngine;
    NSMutableArray*     goalClassLibrary;
}

@property (assign) float        theIntelligenceLevel;

@end
