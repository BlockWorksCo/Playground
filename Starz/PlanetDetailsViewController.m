//
//  PlanetDetailsViewController.m
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "PlanetDetailsViewController.h"
#import "ProductionQueueViewController.h"
#import "StarzAppDelegate.h"


@implementation PlanetDetailsViewController



@dynamic thePlanet;


//
//
//
- (void)setThePlanet:(Planet*)planet
{
    thePlanet   = planet;
    [thePlanet retain];  
    
    [self setTitle:[thePlanet valueForKey:@"Name"]];
}



//
//
//
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

#pragma mark Table view methods


//
//
//
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 4;
}


//
//
//
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if(section == 0)
    {
        return 2;
    }
    
    if(section == 1)
    {
        return 2;
    }
    
    if(section == 2)
    {
        return 2;
    }
    
    if(section == 3)
    {
        return 1;
    }
    
    return 0;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"PlanetDetailCell";
    
    //
    //
    //
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil)
    {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
    
    //
    // Set up the cell...
    //
    NSUInteger  section     = indexPath.section;
    NSUInteger  row         = indexPath.row;
    NSString*   itemName;
    NSString*   keyPath;
    
    switch (section)
    {
        case 0:
        {
            if(row == 0) {itemName   = @"Population";          keyPath=@"Population";}
            if(row == 1) {itemName   = @"Owner";               keyPath=@"Name";};
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
    // Set up the cell...
    //
    cell.textLabel.text         = itemName;
    id  value                   = [thePlanet valueForKey:keyPath];
    cell.detailTextLabel.text   = [value description];

    return cell;
}


//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	ProductionQueueViewController*  newViewController = [[ProductionQueueViewController alloc] initWithNibName:@"ProductionQueueView" bundle:nil];
    
    //
    // Tell the ProductionQueueView which planet to look at.
    //
    [newViewController setThePlanet:thePlanet];
    
    //
    // Show the ProductionQueueViewController
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController pushViewController:newViewController animated:YES];
	[newViewController release];
}



//
//
//
- (void)dealloc
{
    [thePlanet release];
    [super dealloc];
}


@end

