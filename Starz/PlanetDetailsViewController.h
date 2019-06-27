//
//  PlanetDetailsViewController.h
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Model.h"
#import "Planet.h"


@interface PlanetDetailsViewController : UITableViewController
{
    Planet*             thePlanet;
}

@property (retain) Planet*      thePlanet;

@end
