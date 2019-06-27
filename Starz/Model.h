//
//  Model.h
//  Starz
//
//  Created by Steve Tickle on 09/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "Game.h"
#import "Planet.h"
#import "ProductionQueueItem.h"
#import "Hull.h"
#import "LogEntry.h"
#import "TechFixing.h"
#import "TechnologyItem.h"
#import "CoreDataUser.h"



@interface Model : CoreDataUser
{
    
}




+(Model *)instance;

@end
