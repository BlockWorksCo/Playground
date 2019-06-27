//
//  StarzAppDelegate.h
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright ZooKoo 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface StarzAppDelegate : NSObject
{
    UIWindow*                   window;
    UINavigationController*     navigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow*                window;
@property (nonatomic, retain) IBOutlet UINavigationController*  navigationController;

@end

