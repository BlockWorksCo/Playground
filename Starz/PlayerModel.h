//
//  PlayerModel.h
//  Starz
//
//  Created by Steve Tickle on 04/09/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "CoreDataUser.h"
#import "ShipTemplate.h"
#import "Hull.h"
#import "TechFixing.h"
#import "TechnologyItem.h"

@interface PlayerModel : CoreDataUser
{

}


+ (PlayerModel *)instance;
-(void)loadDefaults;



//
//
//
-(TechFixing*)addTechFixingForItem:(TechnologyItem*)item toShipTemplate:(ShipTemplate*)template atPosition:(CGPoint)point;
-(NSArray*)techFixingsForShipTemplate:(ShipTemplate*)template atPosition:(CGPoint)point;
-(NSArray*)allTechFixingsForShipTemplate:(ShipTemplate*)template;
-(ShipTemplate*)getShipTemplateForName:(NSString*)name AndHull:(Hull*)hull;
-(ShipTemplate*)addShipTemplateWithName:(NSString*)name andHull:(Hull*)hull;
-(NSArray*)allShipTemplatesForName:(NSString*)name;
-(NSArray*)allShipTemplatesWithHullType:(int)type;
-(NSArray*)allProductionItems;





@end
