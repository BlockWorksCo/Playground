//
//  MapViewController.h
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Model.h"
#import "MapScrollView.h"
#import <QuartzCore/CoreAnimation.h>


@interface MapViewController : UIViewController <UIScrollViewDelegate>
{
    
    IBOutlet MapScrollView*     theMapScrollView;
    IBOutlet UIToolbar*         theToolbar;
    
    CAShapeLayer*               theShapeLayer;
    UIView*                     thePlanetNamesView;
    
    IBOutlet UIImageView*       theMapBackgroundImageView;

}

@property (readonly) CAShapeLayer*  theShapeLayer;
@property (retain) UIImageView*     theMapBackgroundImageView;

- (id)getNavigationController;
-(void)togglePlanetNames;


@end
