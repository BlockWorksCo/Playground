//
//  ShipDesignerView.h
//  Starz
//
//  Created by Steve Tickle on 18/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ShipDesignerView : UIView
{
    CGPoint     touchedPoint;
    NSString*   ShapeMask;
    CGPoint     touchedItem;
    
    NSString*   shipTemplateName;
}

@property (assign) CGPoint      touchedPoint;
@property (retain) NSString*    ShapeMask;
@property (retain) NSString*    shipTemplateName;

@end
