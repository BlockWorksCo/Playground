//
//  LogViewController.m
//  Starz
//
//  Created by Steve Tickle on 23/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "LogViewController.h"
#import "GameModel.h"
#import "Model.h"


@implementation LogViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
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
    [self setTitle:@"Log"];
    
    //
    //
    //
    UITextView* view        = (UITextView*)[self view];
    NSString*   newText     = [[NSString alloc] init];
    NSArray*    entryList   = [[GameModel instance] allLogEntriesForGame:[[GameModel instance] theCurrentGame]];
    for(int i=0; i<[entryList count]; i++)
    {
        LogEntry*   entry   = [entryList objectAtIndex:i];
                               
        NSLog(@"%@: %@",[entry Year],[entry Text]);
        newText = [newText stringByAppendingFormat:@"\n%@: %@",[entry Year],[entry Text]];
    }

    [view setText:newText];
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
