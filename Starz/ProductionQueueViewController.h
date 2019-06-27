//
//  ProductionQueueViewController.h
//  Starz
//
//  Created by Steve Tickle on 13/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Planet.h"
#import "Model.h"
#import "ProductionItem.h"


@interface ProductionQueueViewController : UITableViewController 
{
    Planet*             thePlanet;
    NSString*           chosenItem;
    IBOutlet UIToolbar* toolbar;

}

@property (retain) Planet*      thePlanet;
@property (retain) NSString*    chosenItem;
@property (retain) UIToolbar*   toolbar;


-(void)setChosenItem:(NSString*)name;
-(void)newItemChosen:(ProductionItem*)item;

@end
