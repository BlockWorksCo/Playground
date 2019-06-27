//
//  TechnologyItemChooserViewController.h
//  Starz
//
//  Created by Steve Tickle on 21/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TechnologyItemChooserViewController : UIViewController
{
    SEL     newChoiceSelector;
    id      newChoiceSelectorObject;
}


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil selector:(SEL)selectorParam object:(id)objectParam;


@end
