//
//  ProductionItemChooserViewController.h
//  Starz
//
//  Created by Steve Tickle on 15/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ProductionQueueViewController.h"


@interface ProductionItemChooserViewController : UITableViewController
{
    ProductionQueueViewController*  productionQueueViewController;
    SEL                             choiceSelector;
    id                              choiceSelectorObject;
}

@property (retain) ProductionQueueViewController*   productionQueueViewController;
@property (readonly) SEL                            choiceSelector;
@property (readonly) id                             choiceSelectorObject;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil;
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil selector:(SEL)pSelector object:(id)pObject;
-(void)showDetailView;
-(void)update;



@end
