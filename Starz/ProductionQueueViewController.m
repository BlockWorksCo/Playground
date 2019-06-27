//
//  ProductionQueueViewController.m
//  Starz
//
//  Created by Steve Tickle on 13/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "ProductionQueueViewController.h"
#import "ProductionItemChooserViewController.h"
#import "ProductionItem.h"
#import "StarzAppDelegate.h"
#import "UniverseModel.h"


@implementation ProductionQueueViewController


@synthesize thePlanet;
@synthesize chosenItem;
@synthesize toolbar;


//
//
//
-(void)setThePlanet:(Planet*)p
{
    thePlanet   = p;
    [thePlanet retain];
}

//
//
//
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        // Custom initialization
    }
    
    return self;
}


//
//
//
-(void)newItemChosen:(ProductionItem*)item
{
    NSLog(@"New ProductionItem chosen:%@",item);
    
    //
    // Add an entry to the table
    //
    ProductionQueueItem*    productionQueueItem = [[UniverseModel instance] addProductionQueueItemToPlanet:thePlanet];

    //
    // Set the queue Item attributes.
    //
    [productionQueueItem setName:[item Name]];
    [productionQueueItem setPriority:[NSNumber numberWithInteger:0]];

    
    [self.tableView reloadData];
}


//
// Called to add a new game to the DB.
//
- (void)insertNewObject 
{    
    //
    // Create the ProductionItemChooserView
    //
	ProductionItemChooserViewController*  newViewController = [[ProductionItemChooserViewController alloc] initWithNibName:@"ProductionItemChooserView" bundle:nil selector:@selector(newItemChosen:) object:self];

    //
    // Tell the item chooser view about ourselves.
    //
    [newViewController setProductionQueueViewController:self];
        
    //
    // Show the ProductionItemChooserView
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController pushViewController:newViewController animated:YES];
	[newViewController release];    
}





/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

- (void)viewWillDisappear:(BOOL)animated
{
    toolbar.hidden   = YES;
}

- (void)viewWillAppear:(BOOL)animated
{
    toolbar.hidden   = NO;
}



//
//
//
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //
    //
    //
    [self setTitle:@"Production Queue"];
    
    //
    // Display a standard "add" button in the navigation bar.
    //
    UIBarButtonItem *addButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(insertNewObject)];
    self.navigationItem.rightBarButtonItem = addButton;
    [addButton release];
    
    //
    //
    //
    [toolbar sizeToFit];
    CGFloat toolbarHeight = [toolbar frame].size.height;
    CGRect rootViewBounds = self.parentViewController.view.bounds;
    CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
    
    //Get the width of the parent view,
    CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
    
    //Create a rectangle for the toolbar
    CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
    
    //Reposition and resize the receiver
    [toolbar setFrame:rectArea];
    
    //UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [[[self parentViewController] view] addSubview:toolbar];
    //[navController setToolbar:toolbar];
    
    
    
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


#pragma mark Table view methods

//
//
//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}


//
//
//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSSet*    items     = [thePlanet PlanetToProductionQueueItem];
    
    return [items count];
}


//
//
//
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"ProductionQueueCell";
    
    //
    //
    //
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil)
    {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:CellIdentifier] autorelease];
    }
    
    
    //
    // Set up the cell...
    //
    NSUInteger  section     = indexPath.section;
    NSUInteger  row         = indexPath.row;
    NSString*   itemName    = @"";
    NSString*   keyPath     = @"";
    
    switch (section)
    {
        case 0:
        {
            if(row == 0) {itemName   = @"item one";             keyPath=@"Population";}
            if(row == 1) {itemName   = @"item two";             keyPath=@"Name";};
            break;
        }
            
        case 1:
        {
            if(row == 0) {itemName   = @"Mines";               keyPath=@"MineCount";}
            if(row == 1) {itemName   = @"Factories";           keyPath=@"FactoryCount";};
            break;
        }
            
        case 2:
        {
            if(row == 0) {itemName   = @"Scanner";             keyPath=@"ScannerType";}
            if(row == 1) {itemName   = @"Starbase";            keyPath=@"StarbaseType";};
            break;
        }
            
        case 3:
        {
            if(row == 0) {itemName   = @"Production queue";    keyPath=@"Population";}
            break;
        }
            
        default:
        {
            break;
        }
    }

    //
    //
    //
    NSSet*                  items   = [thePlanet PlanetToProductionQueueItem];
    ProductionQueueItem*    item    = [[items allObjects] objectAtIndex:indexPath.row];

    
    //
    // Set up the cell...
    //
    cell.textLabel.text         = [item Name];
    cell.detailTextLabel.text   = @"100%";
    
    return cell;
}


//
//
//
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if(section == 0)
    {
        return @"Production Item";
    }
    
    return @"";
}
    


//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //
    // Create the ProductionItemChooserView
    //
	ProductionItemChooserViewController*  newViewController = [[ProductionItemChooserViewController alloc] initWithNibName:@"ProductionItemChooserView" bundle:nil];
    
    //
    // Tell the item chooser view about ourselves.
    //
    [newViewController setProductionQueueViewController:self];
    
    //
    // Show the ProductionItemChooserView
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController pushViewController:newViewController animated:YES];
	[newViewController release];    
    
    
}


/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */


//
//
//
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{ 
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert)
    {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        
    }   
}


//
//
//
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}


//
//
//
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}




- (void)dealloc {
    [thePlanet release];
    [super dealloc];
}


@end
