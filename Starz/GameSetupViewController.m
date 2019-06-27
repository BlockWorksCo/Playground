//
//  GameSetupViewController.m
//  Starz
//
//  Created by Steve Tickle on 22/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "GameSetupViewController.h"
#import "SliderTableCell.h"
#import "GameSetupItemHelpViewController.h"
#import "GameModel.h"
#import "UniverseModel.h"


@implementation GameSetupViewController


/*
- (id)initWithStyle:(UITableViewStyle)style {
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if (self = [super initWithStyle:style]) {
    }
    return self;
}
*/

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        // Custom initialization
        [self setTitle:@"Game Setup"];
    }
    return self;
}

/*
- (void)viewDidLoad {
    [super viewDidLoad];

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}
*/

/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/
/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/

//
//
//
- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
    
    //
    // Create the game.
    //
    [[GameModel instance] setupGame];
}



- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

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

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{
    return 3;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch(section)
    {
        case 0:
        {
            return 2;
            break;
        }
        
        case 1:
        {
            return 4;
            break;
        }
            
        case  2:
        {
            return 1;
            break;
        }
            
        default:
        {
            return 0;
            break;
        }
    }
}


//
//
//
-(void)setupItemInfoAtIndexPath:(NSIndexPath*)indexPath itemName:(NSString**)itemName keyPath:(NSString**)keyPath target:(id*)object helpText:(NSString**)helpText uiClassName:(NSString**)uiClassName
{
    
    //
    // Set up the cell...
    //
    NSUInteger  section     = indexPath.section;
    NSUInteger  row         = indexPath.row;
    
    *object = nil;
    
    switch (section)
    {
        case 0:
        {
            if(row == 0) {*itemName   = @"Universe Size";        *keyPath=@"theUniverseSize";     *object   = [UniverseModel instance]; *helpText=@"Universe size";      *uiClassName=@"SliderTableCell";}
            if(row == 1) {*itemName   = @"Universe Density";     *keyPath=@"theUniverseDensity";  *object   = [UniverseModel instance]; *helpText=@"Universe density";   *uiClassName=@"SliderTableCell";}
            break;
        }
            
        case 1:
        {
            if(row == 0) {*itemName   = @"Mine Build Rate";     *keyPath=@"theHumanPlayer.theMineBuildRate";     *object   = [GameModel instance]; *helpText=@"rate of mine building";     *uiClassName=@"SliderTableCell";}
            if(row == 1) {*itemName   = @"Factory Build Rate";  *keyPath=@"theHumanPlayer.theFactoryBuildRate";  *object   = [GameModel instance]; *helpText=@"rate of factory building";  *uiClassName=@"SliderTableCell";}
            if(row == 2) {*itemName   = @"Research Rate";       *keyPath=@"theHumanPlayer.theResearchRate";     *object   = [GameModel instance];  *helpText=@"rate of research";          *uiClassName=@"SliderTableCell";}
            
            if(row == 3) {*itemName   = @"Extra ships";       *keyPath=@"theHumanPlayer.theExtraShipsFlag";      *object   = [GameModel instance]; *helpText=@"Extra Ships";          *uiClassName=@"SwitchTableCell";}
            break;
        }
            
        case 2:
        {
            if(row == 0) {*itemName   = @"Intelligence Level";  *keyPath=@"theAIPlayer.theIntelligenceLevel";    *object   = [GameModel instance]; *helpText=@"AI level"; *uiClassName=@"SliderTableCell";}
            break;
        }
            
        default:
        {
            break;
        }
    }
}


//
//
//
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if(section == 0)
    {
        return @"Game Parameters";
    }
    else if(section == 1)
    {
        return @"Human Player";
    }
    else if(section == 2)
    {
        return @"AI Player";
    }
    else
    {
        return @"<None>";
    }
}


//
//
//
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath 
{
    NSString*   itemName        = @"";
    NSString*   keyPath         = @"";
    NSString*   helpText        = @"";
    NSString*   uiClassName     = @"";
    id          targetObject    = nil;
    
    //
    //
    //
    [self setupItemInfoAtIndexPath:indexPath itemName:&itemName keyPath:&keyPath target:&targetObject helpText:&helpText uiClassName:&uiClassName];

    //
    //
    //
    int             height      = [NSClassFromString(uiClassName) height];

    //
    //
    //
    return height;
} 



//
// Customize the appearance of table view cells.
//
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString*   itemName        = @"";
    NSString*   keyPath         = @"";
    NSString*   helpText        = @"";
    NSString*   uiClassName     = @"";
    id          targetObject    = nil;
    
    //
    //
    //
    [self setupItemInfoAtIndexPath:indexPath itemName:&itemName keyPath:&keyPath target:&targetObject helpText:&helpText uiClassName:&uiClassName];
    
    //
    //
    //
    SliderTableCell*    cell            = (SliderTableCell*)[tableView dequeueReusableCellWithIdentifier:uiClassName];
    
    //
    //
    //
    if (cell == nil)
    {
        cell    = [[[NSClassFromString(uiClassName) alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:uiClassName] autorelease];
    }
    
    //
    //
    //
    [cell setItemName:itemName keyPath:keyPath target:targetObject helpText:helpText];
    
	
    return cell;
}


//
//
//
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString*   itemName        = @"";
    NSString*   keyPath         = @"";
    NSString*   helpText        = @"";
    NSString*   uiClassName     = @"";
    id          targetObject    = nil;
    
    //
    //
    //
    [self setupItemInfoAtIndexPath:indexPath itemName:&itemName keyPath:&keyPath target:&targetObject helpText:&helpText uiClassName:&uiClassName];
    
    //
    // Navigation logic may go here. Create and push another view controller.
    //
	GameSetupItemHelpViewController*    newView = [[GameSetupItemHelpViewController alloc] initWithNibName:@"GameSetupItemHelpView" bundle:nil];
    
    //
    // Set the help text to the setup item help text.
    //
    [newView setHelpText:helpText];
    
    //
    //
    //
	[self.navigationController pushViewController:newView animated:YES];
	[newView release];
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/


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


- (void)dealloc {
    [super dealloc];
}


@end

