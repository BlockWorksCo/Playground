//
//  GameListViewController.m
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "GameListViewController.h"
#import "GameSetupViewController.h"
#import "MapViewController.h"
#import "Game.h"
#import "GameModel.h"


@implementation GameListViewController

//
//
//


//
//
//
- (void)viewDidLoad 
{
    [super viewDidLoad];

    //
    // Display a standard "add" button in the navigation bar.
    //
    UIBarButtonItem *addButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(insertNewObject)];
    self.navigationItem.rightBarButtonItem = addButton;
    [addButton release];
    
    
    self.navigationItem.titleView.opaque    = YES;
    
    [self navigationController].navigationBar.barStyle  = UIBarStyleBlackOpaque;
    //self.navigationItem.navigationBar.barStyle = UIBarStyleBlackOpaque;    
}




//
// Called to add a new game to the DB.
//
- (void)insertNewObject 
{    
    NSTimeInterval  secondsSince1970    = [[NSDate date] timeIntervalSince1970];
    NSString*       gameName    = [NSString stringWithFormat:@"Game_%ld",(unsigned long long)(secondsSince1970*100.0)];
    
    [[GameModel instance] createOrOpenGame:gameName];
    [[self tableView] reloadData];    
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
    //
    // Return the number of games.
    //
    return [[[GameModel instance] getGameList] count];
}


//
// Customize the appearance of table view cells.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString*        CellIdentifier  = @"Cell";
    UITableViewCell*        cell            = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    //
    //
    //
    if (cell == nil) 
    {
        cell    = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
    }
    
    //
	// Configure the cell.
    //
	cell.textLabel.text     = [[[GameModel instance] getGameList] objectAtIndex:indexPath.row];
    cell.detailTextLabel.text   = @"detail...";
	
    return cell;
}


//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
    //
    // Get the name of the chosen game and tell the Model about it.
    //
    NSString*   gameName    = [[[GameModel instance] getGameList] objectAtIndex:indexPath.row];
    [[GameModel instance] createOrOpenGame:gameName];

    //
    // If the game has been setup, go to the map, otherwise to the setup view.
    //
    if([[GameModel instance] isSetup] == YES)
    {
        //
        // Go to the Map view.
        //
        MapViewController*      mapViewController   = [[MapViewController alloc] initWithNibName:@"MapView" bundle:nil];
        
        [self.navigationController pushViewController:mapViewController animated:YES];
        [mapViewController release];
    }
    else
    {
        //
        // Go to the Setup view.
        //
        GameSetupViewController*      setupViewController   = [[GameSetupViewController alloc] initWithNibName:@"GameSetupView" bundle:nil];
        
        [self.navigationController pushViewController:setupViewController animated:YES];
        [setupViewController release];        
    }
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


//
// Override to support editing the table view.
//
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{   
    //
    //
    //
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        //
        // Delete the game from the database.
        //
        [[GameModel instance] removeGame:[[[GameModel instance] allGames] objectAtIndex:indexPath.row]];
        
        //
        // Delete the game from the list.
        //
		[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
}



/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


//
// Destructor.
//
- (void)dealloc
{
    [super dealloc];
}


@end

