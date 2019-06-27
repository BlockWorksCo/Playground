//
//  MapScrollView.m
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "MapScrollView.h"
#import "MapViewController.h"
#import "PlanetDetailsViewController.h"
#import "ShipDesignListViewController.h"
#import "GameSetupViewController.h"
#import "Model.h"
#import <UIKit/UIKit.h>
#import <QuartzCore/CoreAnimation.h>
#import "PlanetView.h"
#import "ProductionItemChooserViewController.h"
#import "StarzAppDelegate.h"
#import "LogViewController.h"
#import "GameModel.h"
#import "UniverseModel.h"



@implementation MapScrollView


//
// Called at 1Hz
//
/*
- (void)frameRedraw
{
    [self setNeedsDisplay];
}
 */


//
//
//
/*
+(Class)layerClass
{
	//return [MapTiledLayer class];
	//return [CATiledLayer class];
	//return [CAShapeLayer class];
    //return [PlanetView class];
}
 */




//
//
//
/*
- (void)drawRect:(CGRect)rect
{
    [super drawRect:rect];    
}
*/


//
//
//
- (void)viewDidLoad
{
    //
    //
    //
    int         universeSideLength  = (int)([[UniverseModel instance] theUniverseSize] * 4096.0);
    MapView*    mapView = [[MapView alloc] initWithFrame:CGRectMake(0, 0, universeSideLength, universeSideLength)];
    
    //UIView* view    = [self view];
    //[self createNewLayerInLayer:[self layer]];

    //
    //
    //
    //[self createNewPlanetViewInLayer:[self layer]];
    //[self createNewPlanetViewInLayer:[self layer]];
    //[self createNewPlanetViewInLayer:[mapView layer]];
    
    
    
    
    //[mapView setOpaque:YES];
    //[self setOpaque:YES];
    
    //
    // Set the map size to the size of the universe.
    //
    [self addSubview:mapView];
    [self setContentSize:CGSizeMake(universeSideLength,universeSideLength)];
    
    //[self setZooming:YES];
    [self setMaximumZoomScale:0.1];
    [self setMinimumZoomScale:10.0];

    [self setBackgroundColor:[UIColor blackColor]];
    
    //
    // Set the tiles to be 512x512.
    //
    //CATiledLayer*   tiledLayer  = (CATiledLayer*)[mapView layer];
    //[tiledLayer setTileSize:CGSizeMake(64,64)];
        
    //
    // Start a time to ping us at 1Hz.
    //
    //[[NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(frameRedraw) userInfo:nil repeats:YES] retain];
}


- (void)dealloc 
{
    [super dealloc];
}





//
//
//
- (void) touchesEnded: (NSSet *) touches withEvent: (UIEvent *) event
{
    id          navController   = [theMapViewController getNavigationController];
    UIView*     view            = [[self subviews] objectAtIndex:0];
    
    //
    // Determine which planet was touched.
    //
    CGPoint    point        = [[touches anyObject] locationInView:view];

    Planet* touchedPlanet   = [[UniverseModel instance] planetClosestToPoint:point];
    //Planet* touchedPlanet   = [[[Model instance] getAllPlanets] objectAtIndex:0];
    
    //
    // Create the PlanetDetailsView
    //
	PlanetDetailsViewController *planetDetailsViewController = [[PlanetDetailsViewController alloc] initWithNibName:@"PlanetView" bundle:nil];
    
    //
    // Tell the view which Planet to show details of...
    //
    [planetDetailsViewController setThePlanet:touchedPlanet];

    //
    // Show the PlanetDetailsView.
    //
	[navController pushViewController:planetDetailsViewController animated:YES];
	[planetDetailsViewController release];
}



//
//
//
- (IBAction)TurnButtonPressed:(id)sender
{
    //
    // Navigation logic may go here. Create and push another view controller.
    //
    LogViewController*      newViewController   = [[LogViewController alloc] initWithNibName:@"LogView" bundle:nil];
    
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    
    [navController pushViewController:newViewController animated:YES];
    [newViewController release];        
}


//
//
//
- (IBAction)ShipsButtonPressed:(id)sender
{
    id  navController   = [theMapViewController getNavigationController];
    
    // Navigation logic may go here. Create and push another view controller.
	ShipDesignListViewController *shipDesignListViewController = [[ShipDesignListViewController alloc] initWithNibName:@"ShipDesignList" bundle:nil];
    
	[navController pushViewController:shipDesignListViewController animated:YES];
	[shipDesignListViewController release];
}


//
//
//
- (IBAction)OrdersButtonPressed:(id)sender
{
    [theMapViewController togglePlanetNames];
    
    static int  yearCounter = 3000;
    [[GameModel instance] addLogEntry:@"button pressed" ForYear:yearCounter ToGame:[[GameModel instance] theCurrentGame]];
    yearCounter++;
}


//
// Show the browser view.
//
- (IBAction)StatisticsButtonPressed:(id)sender
{
    //
    // Create the ProductionItemChooserView
    //
	ProductionItemChooserViewController*  newViewController = [[ProductionItemChooserViewController alloc] initWithNibName:@"ProductionItemChooserView" bundle:nil];
    
    //
    // Show the ProductionItemChooserView
    //
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [navController pushViewController:newViewController animated:YES];
	[newViewController release];    
    
}



- (IBAction)SetupButtonPressed:(id)sender
{
    id  navController   = [theMapViewController getNavigationController];
    
    //
    // Navigation logic may go here. Create and push another view controller.
    //
	GameSetupViewController*      setupViewController   = [[GameSetupViewController alloc] initWithNibName:@"GameSetupView" bundle:nil];
    
	[navController pushViewController:setupViewController animated:YES];
	[setupViewController release];
    
}


@end
