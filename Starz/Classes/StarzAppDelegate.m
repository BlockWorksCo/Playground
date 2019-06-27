//
//  StarzAppDelegate.m
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright ZooKoo 2009. All rights reserved.
//

#import "StarzAppDelegate.h"
#import "RootViewController.h"
#import "Model.h"
#import "PlayerModel.h"
#import "GameModel.h"

@implementation StarzAppDelegate

@synthesize window;
@synthesize navigationController;


//
//
//
- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	// Configure and show the window
	[window addSubview:[navigationController view]];
	[window makeKeyAndVisible];
}


//
//
//
- (void)applicationWillTerminate:(UIApplication *)application 
{
    //
    // Save the data
    //
    [[PlayerModel instance] save];
    [[GameModel instance] save];
}


//
//
//
- (void)dealloc 
{
	[navigationController release];
	[window release];
	[super dealloc];
}





@end
