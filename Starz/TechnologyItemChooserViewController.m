//
//  TechnologyItemChooserViewController.m
//  Starz
//
//  Created by Steve Tickle on 21/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "TechnologyItemChooserViewController.h"
#import "TechnologyItem.h"
#import "Model.h"
#import "StarzAppDelegate.h"
#import "GameModel.h"

@implementation TechnologyItemChooserViewController

//
//
//
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil selector:(SEL)selectorParam object:(id)objectParam
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        //
        //
        //
        [self setTitle:@"Tech Chooser"];
        newChoiceSelector           = selectorParam;
        newChoiceSelectorObject     = objectParam;
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
    return 1;
}





//
// Customize the number of rows in the table view.
//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{    
    //
    // return the number sections (always 1).
    //
    NSArray*    itemsArray  = [[GameModel instance] allTechnologyItems];
    
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
    NSArray*        itemsArray  = [[GameModel instance] allTechnologyItems];
    TechnologyItem* item        = [itemsArray objectAtIndex:indexPath.row];
	cell.textLabel.text         = [item Name];
	
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
    if(newChoiceSelectorObject != nil)
    {
        if([newChoiceSelectorObject respondsToSelector:newChoiceSelector] == YES)
        {
            NSArray*    itemsArray  = [[GameModel instance] allTechnologyItems];
            Hull*       item        = [itemsArray objectAtIndex:indexPath.row];
            
            [newChoiceSelectorObject performSelector:newChoiceSelector withObject:item];
        }
    }
    
    //
    // Dismiss this view.
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController popViewControllerAnimated:YES];    
}







@end
