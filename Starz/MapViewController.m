//
//  MapViewController.m
//  Starz
//
//  Created by Steve Tickle on 27/06/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "MapViewController.h"
#import <QuartzCore/QuartzCore.h>
#import "PlanetView.h"
#import "StarzAppDelegate.h"
#import "UniverseModel.h"


@implementation MapViewController

@synthesize theShapeLayer;
@synthesize theMapBackgroundImageView;


// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) 
    {
        // Custom initialization
        [self setTitle:@"Map View"];
        //[[self view] setWantsLayer:NO];
    }
    return self;
}


/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


- (void)viewWillDisappear:(BOOL)animated
{
    theToolbar.hidden   = YES;
}

- (void)viewWillAppear:(BOOL)animated
{
    theToolbar.hidden   = NO;
}


//
//
//
-(void)createNewPlanetViewInLayer:(CALayer*)parentLayer
{
    PlanetView* planetView  = [[PlanetView alloc] init];
    [planetView setFrame:CGRectMake(100, 50, 250, 75)];
    [planetView setMasksToBounds:YES];
    
    CGFloat components[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGColorRef whiteColor = CGColorCreate(colorSpace, components);
    [planetView setBackgroundColor:whiteColor]; 
    //[parentLayer addSublayer:planetView];
    [parentLayer insertSublayer:planetView atIndex:0];
    CGColorRelease(whiteColor);
    CGColorSpaceRelease(colorSpace);
}



-(void)drawPlanetsInShapeLayer
{
    CAShapeLayer*   shapeLayer  = [[CAShapeLayer alloc] init];
    [[[self view] layer] addSublayer:shapeLayer];

    //
    //
    //
    CGMutablePathRef thePath = CGPathCreateMutable();
    
    //
    // Draw a green solid circle
    //
    NSArray*   planets = [[UniverseModel instance] allPlanets];
    for(int i=0; i<[planets count]; i++)
    {
        Planet* planet      = [planets objectAtIndex:i];        
        int     size        = [[planet valueForKey:@"Diameter"] intValue];
        int     x           = [[planet valueForKey:@"XPosition"] intValue];
        int     y           = [[planet valueForKey:@"YPosition"] intValue];
        //NSString*   name    = [planet valueForKey:@"Name"];
        //CGRect  clipRect    = CGRectMake(x-size-10, y-size-10, (size*2)+20, (size*2)+20);
        
        CGRect  rect    = CGRectMake(x-(size/2), y-(size/2), size, size);
        CGPathAddEllipseInRect(thePath, NULL, rect);
        //[self drawPlanetInContext:ctx XPosition:x YPosition:y Size:size Name:name];
        
    }

    //
    //
    //
    [shapeLayer setPath:thePath];
    [shapeLayer setStrokeColor:[[UIColor redColor] CGColor]];
    [shapeLayer setFillColor:[[UIColor greenColor] CGColor]];
    [shapeLayer setOpaque:YES];
    CFRelease(thePath);
}


//
//
//
-(CALayer*)findLayerThatContainsPoint:(CGPoint)point
{
    NSArray*    layerArray  = [[[self view] layer] sublayers];
    
    //
    //
    //
    for(int i=1; i<[layerArray count]; i++)
    {
        CALayer*    thisLayer   = [layerArray objectAtIndex:i];
        CGRect      thisLayerBounds = [thisLayer bounds];
        if(CGRectContainsPoint(thisLayerBounds, point) == YES)
        {
            return thisLayer;
        }
    }
    
    return nil;
}


+(Class)layerClass
{
	return [CATiledLayer class];
}    


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
    [super viewDidLoad];
    [theMapScrollView viewDidLoad];
    
    
    //
    //
    //
    int                 universeSideLength  = (int)([[UniverseModel instance] theUniverseSize] * 4096.0);
    NSString*           filePath        = [[NSBundle mainBundle] pathForResource:@"Background8" ofType:@"jpg"];
    UIImage*            backgroundImage = [[UIImage alloc] initWithContentsOfFile:filePath];
    theMapBackgroundImageView   = [[UIImageView alloc] initWithImage:backgroundImage];
    //[theMapBackgroundImageView setCenter:CGPointMake(0.5,0.5)];
    [theMapBackgroundImageView setBounds:CGRectMake(0, 0, universeSideLength, universeSideLength)];
    [theMapBackgroundImageView setFrame:CGRectMake(0, 0, universeSideLength, universeSideLength)];
    [theMapBackgroundImageView setOpaque:YES];
    //[theMapBackgroundImageView setContentMode:UIViewContentModeRedraw|UIViewContentModeCenter|UIViewContentModeRedraw|UIViewContentModeScaleAspectFill];
    
    //[[self view] addSubview:theMapBackgroundImageView];
    [[self view] insertSubview:theMapBackgroundImageView atIndex:4];
    
    //
    //
    //
    
    //[self createNewLayerInLayer:[[self view] layer]];
    
    //
    //
    //
    /*
    CAReplicatorLayer*  replicatorLayer = [[CAReplicatorLayer alloc] init];
    [[[self view] layer] addSublayer:replicatorLayer];
    
    //[PlanetView createNewPlanetViewInLayer:[[self view] layer]];
    [PlanetView createNewPlanetViewInLayer:replicatorLayer];

    CATransform3D  transform   = CATransform3DMakeTranslation(100, 0, 0);
    
    int c=1;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            
            [replicatorLayer setInstanceTransform:transform];
            [replicatorLayer setInstanceCount:c];
            c++;
            
            if(i != 0)
            {
                transform   = CATransform3DMakeTranslation(100, 0, 0);
            }
            else
            {
                transform   = CATransform3DMakeTranslation(-1100, 100, 0);
            }
        }
    }
    */
    
    
    //
    //
    //
    //[self drawPlanetsInShapeLayer];
    
    //
    // Create the tiles.
    //
    int     rSize   = 256;
    for(int i=0; i<universeSideLength; i+=rSize)
    {
        for(int j=0; j<universeSideLength; j+=rSize)
        {
            CALayer*    newLayer    = [[CALayer alloc] init];
            CGRect      boundsRect  = CGRectMake(i,j, rSize, rSize);
            [newLayer setBounds:boundsRect];
            [newLayer setFrame:boundsRect];
            [newLayer setMasksToBounds:NO];
            [newLayer setHidden:NO];
            [[[self view] layer] addSublayer:newLayer];
        }
    }
    
    //CALayer*    parentLayer = [[[[self view] layer] sublayers] objectAtIndex:0];
    [[[self view] layer] setMasksToBounds:YES];
    
    
    //
    // Add the planets to the tiles.
    //
    NSArray*   planets = [[UniverseModel instance] allPlanets];
    for(int i=0; i<[planets count]; i++)
    {
        Planet* planet      = [planets objectAtIndex:i];        
        int     size        = [[planet valueForKey:@"Diameter"] intValue];
        int     x           = [[planet valueForKey:@"XPosition"] intValue];
        int     y           = [[planet valueForKey:@"YPosition"] intValue];
        
        CAShapeLayer*   shapeLayer  = [[CAShapeLayer alloc] init];
        CALayer*    parentLayer = [self findLayerThatContainsPoint:CGPointMake(x,y)];
        [parentLayer addSublayer:shapeLayer];
        CGMutablePathRef thePath = CGPathCreateMutable();
        
        CGRect  rect    = CGRectMake(x-(size/2), y-(size/2), size, size);
        CGPathAddEllipseInRect(thePath, NULL, rect);
        
        [shapeLayer setPath:thePath];
        [shapeLayer setStrokeColor:[[UIColor redColor] CGColor]];
        [shapeLayer setFillColor:[[UIColor greenColor] CGColor]];
        [shapeLayer setOpaque:YES];
        CFRelease(thePath);
        
    }
    
    [[theMapScrollView layer] setMasksToBounds:YES];
    
    //
    //
    //
    thePlanetNamesView    = [[UIView alloc] init];
    for(int i=0; i<[planets count]; i++)
    {
        Planet* planet      = [planets objectAtIndex:i];        
        NSString*   name    = [planet valueForKey:@"Name"];
        int     x           = [[planet valueForKey:@"XPosition"] intValue];
        int     y           = [[planet valueForKey:@"YPosition"] intValue];
        
        //
        //
        //
        UILabel*    label   = [[UILabel alloc] initWithFrame:CGRectMake(x,y,150,50)];
        [label setTextColor:[UIColor whiteColor]];
        [label setBackgroundColor:[UIColor clearColor]];
        [label setText:name];
        [thePlanetNamesView addSubview:label];
    }        
    [theMapScrollView addSubview:thePlanetNamesView];
    
    //
    //
    //
	//theMapScrollView = [[MapScrollView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 2448.0f, 3264.0f)];
	//tiledView.tiledLayer.tileSize = CGSizeMake(245.0f, 327.0f);
	//tiledView.tiledLayer.levelsOfDetail = 1;
	//tiledView.tiledLayer.levelsOfDetailBias = 0;
    
    //scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 320.0f, 460.0f)];  
    //scrollView.contentSize = tiledView.bounds.size;  
    //scrollView.maximumZoomScale = 4.0f;  
	//scrollView.minimumZoomScale = 0.5f;  
    //scrollView.delegate = tiledView;
    //ß[scrollView addSubview:tiledView];
    
    //[self.view addSubview:theMapScrollView];
    
    
    //
    //
    //
    [theToolbar sizeToFit];
    CGFloat toolbarHeight = [theToolbar frame].size.height;
    CGRect rootViewBounds = self.parentViewController.view.bounds;
    CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
    
    //Get the width of the parent view,
    CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
    
    //Create a rectangle for the toolbar
    CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
    
    //Reposition and resize the receiver
    [theToolbar setFrame:rectArea];
    
    UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    [[navController view] addSubview:theToolbar];
    
    //
    //
    //
    [theToolbar setOpaque:YES];
    //[[navController view] setOpaque:YES];
}


//
//
//
-(void)togglePlanetNames
{
    if([thePlanetNamesView isHidden] == YES)
    {
        [thePlanetNamesView setHidden:NO];
    }
    else
    {
        [thePlanetNamesView setHidden:YES];
    }
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
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc 
{
    [super dealloc];
}



- (id)getNavigationController
{
    return self.navigationController;
}


//
// When the user scrolls the map, lets cause the map to be redrawn to reflect this.
//
/*
- (void)scrollViewDidScroll:(UIScrollView*)scrollView
{
	CGPoint	p = scrollView.contentOffset;
    NSLog(@"x = %f, y = %f", p.x, p.y);

    [[self view] setNeedsDisplay];
     
}
*/






@end
