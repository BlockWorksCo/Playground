//
//  ProductionItemChooserViewController.m
//  Starz
//
//  Created by Steve Tickle on 15/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "ProductionItemChooserViewController.h"
#import "PlayerModel.h"
#import "StarzAppDelegate.h"
#import "ProductionQueueViewController.h"
#import "ShipDesignerViewController.h"


@implementation ProductionItemChooserViewController

@synthesize productionQueueViewController;
@synthesize choiceSelector;
@synthesize choiceSelectorObject;






- (id)initWithNibName:(NSString *)nibName bundle:(NSBundle *)nibBundle
{
    self = [super initWithNibName:nibName bundle:nibBundle];
    if (self)
    {
        choiceSelector          = nil;
        choiceSelectorObject    = nil;
    }
    return self;
}


//
//
//
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil selector:(SEL)pSelector object:(id)pObject
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        choiceSelector          = pSelector;
        choiceSelectorObject    = pObject;
    }
    
    return self;        
}



//
//
//
-(void)update
{
    [[self tableView] reloadData];
}



//
//
//
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //
    // Set the table title.
    //
    [self setTitle:@"Production Items"];
    
    //
    // Display a standard "add" button in the navigation bar.
    //
    UIBarButtonItem *addButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(insertNewObject)];
    self.navigationItem.rightBarButtonItem = addButton;
    [addButton release];
}




//
// Called to add a new game to the DB.
//
- (void)insertNewObject 
{    
    //NSTimeInterval  secondsSince1970    = [[NSDate date] timeIntervalSince1970];
    //NSString*       gameName    = [NSString stringWithFormat:@"Game_%ld",(unsigned long long)(secondsSince1970*100.0)];
    
    [[self tableView] reloadData];
    [self showDetailView];
}


- (void)didReceiveMemoryWarning
{
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}



#pragma mark Table view methods




//
//
//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    //
    // return the number sections (always 1).
    //
    return 3;
}



//
//
//
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if(section == 0)
    {
        return @"Starbases";
    }
    else if(section == 1)
    {
        return @"Scanners";
    }
    else if(section == 2)
    {
        return @"Ships";
    }
    else
    {
        return @"<None>";
    }
}



//
// Customize the number of rows in the table view.
//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSArray*    itemArray   = nil;
    
    if(section == 0)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:1];
    }
    else if(section == 1)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:2];
    }
    else if(section == 2)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:0];
    }
    else
    {
        itemArray   = nil;
    }
    
    return [itemArray count];
}


//
// Customize the appearance of table view cells.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString*        CellIdentifier  = @"ProductionItemChooserCell";
    UITableViewCell*        cell            = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    //
    //
    //
    if (cell == nil) 
    {
        cell    = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:CellIdentifier] autorelease];
        [cell setAccessoryType:UITableViewCellAccessoryDetailDisclosureButton];
    }
    
    //
	// Configure the cell.
    //
    NSArray*        itemArray   = nil;

    if(indexPath.section == 0)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:1];
    }
    else if(indexPath.section == 1)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:1];
    }
    else if(indexPath.section == 2)
    {
        itemArray   = [[PlayerModel instance] allShipTemplatesWithHullType:0];
    }
    
    NSDictionary*   item        = [itemArray objectAtIndex:indexPath.row];
    NSString*       name        = [item valueForKey:@"Name"];
    NSString*       keyPath     = @"KeyPath";
    
	cell.textLabel.text         = name;
    cell.detailTextLabel.text   = keyPath;
	
    return cell;
}



//
//
//
-(void)showDetailView
{
    //
    // Create the ProductionItemChooserView
    //
	ShipDesignerViewController*  newViewController = [[ShipDesignerViewController alloc] initWithNibName:@"ShipDesignerView" bundle:nil];
    
    //
    // Show the ProductionItemChooserView
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController pushViewController:newViewController animated:YES];
	[newViewController release];    
}

//
// Show the ship detail view.
//
- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
    [self showDetailView];
}

//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
    NSArray*    itemArray   = [[PlayerModel instance] allProductionItems];

    //
    // Tell the ProductionQueueViewController which item has been chosen.
    //
    //[productionQueueViewController setChosenItem:@"chosen item"];
    if(choiceSelectorObject != nil)
    {
        if([choiceSelectorObject respondsToSelector:choiceSelector] == YES)
        {
            ProductionItem* item    = [itemArray objectAtIndex:indexPath.row];
            [choiceSelectorObject performSelector:choiceSelector withObject:item];
        }
        
        //
        // The user has chosen an item, so lets return to the parent view.
        //
        UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
        [navController popViewControllerAnimated:YES];
    }
    else
    {
        //
        // This is just a browsing view... no action to be performed.
        //
    }
    
}


//
//
//
- (void)dealloc
{
    [super dealloc];
}


@end
