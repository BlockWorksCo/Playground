//
//  GameSetupItemHelpView.m
//  Starz
//
//  Created by Steve Tickle on 26/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "GameSetupItemHelpViewController.h"


@implementation GameSetupItemHelpViewController


@synthesize textView;
@synthesize helpText;


/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

//
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
//
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //
    //
    //
    [self setTitle:@"Help"];
    [textView setText:helpText];
    
    //
    //
    //
    //[[self view] set
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
