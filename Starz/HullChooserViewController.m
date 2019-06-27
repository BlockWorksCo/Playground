//
//  HullChooserViewController.m
//  Starz
//
//  Created by Steve Tickle on 21/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "HullChooserViewController.h"
#import "StarzAppDelegate.h"
#import "Model.h"
#import "GameModel.h"



@implementation HullChooserViewController


//
// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
//
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil selector:(SEL)selectorParam object:(id)objectParam
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        //
        //
        //
        [self setTitle:@"Hull Chooser"];
        hullChosenSelector          = selectorParam;
        hullChosenSelectorObject    = objectParam;
    }
    return self;
}

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

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











//
//
//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    //
    // return the number sections (always 1).
    //
    return 1;
}





//
// Customize the number of rows in the table view.
//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSArray*    itemsArray  = [[GameModel instance] allHulls];
    
    return [itemsArray count];
}


//
// Customize the appearance of table view cells.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString*        CellIdentifier  = @"CellIdentifier";
    UITableViewCell*        cell            = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    //
    //
    //
    if (cell == nil) 
    {
        cell    = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
        //[cell setAccessoryType:UITableViewCellAccessoryDetailDisclosureButton];
    }
    
    //
	// Configure the cell.
    //
    NSArray*    itemsArray  = [[GameModel instance] allHulls];
    Hull*       item        = [itemsArray objectAtIndex:indexPath.row];
	cell.textLabel.text     = [item Name];
	
    return cell;
}

//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
    //
    // Call the callback.
    //
    if(hullChosenSelectorObject != nil)
    {
        if([hullChosenSelectorObject respondsToSelector:hullChosenSelector] == YES)
        {
            NSArray*    itemsArray  = [[GameModel instance] allHulls];
            Hull*       item        = [itemsArray objectAtIndex:indexPath.row];
            
            [hullChosenSelectorObject performSelector:hullChosenSelector withObject:item];
        }
    }
    
    //
    // Dismiss this view.
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController popViewControllerAnimated:YES];
    
}






@end
