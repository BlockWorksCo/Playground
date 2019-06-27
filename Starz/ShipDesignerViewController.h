//
//  ShipDesignerViewController.h
//  Starz
//
//  Created by Steve Tickle on 17/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Hull.h"
#import "TechnologyItem.h"


@interface ShipDesignerViewController : UIViewController <UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate>
{
    NSArray*                theHulls;
    Hull*                   theHull;
    NSString*               theHullShapeMask;
    
    
    IBOutlet UITextField*   theHullNameTextField;
    IBOutlet UITextField*   theShipNameTextField;
    IBOutlet UIPickerView*  theHullPickerView;
    
    IBOutlet UITextField*   theGermaniumTextField;
    IBOutlet UITextField*   theIroniumTextField;
    IBOutlet UITextField*   theBoraniumTextField;
}

@property (retain) NSArray*         theHulls;
@property (retain) Hull*            theHull;
@property (retain) NSString*        theHullShapeMask;


-(IBAction)hullNameTouched:(id)sender;
-(void)hullChosen:(Hull*)newHull;
-(void)createShipTemplateWithName:(NSString*)name andHull:(Hull*)hull;
-(void)techItemChosen:(TechnologyItem*)item atPosition:(CGPoint)position;


@end
