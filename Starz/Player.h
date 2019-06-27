//
//  Player.h
//  Starz
//
//  Created by Steve Tickle on 29/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Player : NSObject
{
    float       theMineBuildRate;
    float       theFactoryBuildRate;
    float       theResearchRate;
    BOOL        theExtraShipsFlag;
}

@property (assign) float        theMineBuildRate;
@property (assign) float        theFactoryBuildRate;
@property (assign) float        theResearchRate;
@property (assign) BOOL         theExtraShipsFlag;

@end
