//
//  MapScrollView.h
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIKit/UIScrollView.h"
#import "Model.h"
#import "MapView.h"


@interface MapScrollView : UIScrollView
{
    
    int             theSize;
    IBOutlet id     theMapViewController;
}

- (void)viewDidLoad;

- (IBAction)TurnButtonPressed:(id)sender;
- (IBAction)ShipsButtonPressed:(id)sender;
- (IBAction)OrdersButtonPressed:(id)sender;
- (IBAction)StatisticsButtonPressed:(id)sender;
- (IBAction)SetupButtonPressed:(id)sender;

@end
