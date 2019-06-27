//
//  ShipDesignerViewController.m
//  Starz
//
//  Created by Steve Tickle on 17/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "ShipDesignerViewController.h"
#import "ShipDesignerView.h"
#import "Model.h"
#import "HullChooserViewController.h"
#import "TechnologyItemChooserViewController.h"
#import "ShipTemplate.h"
#import "Model.h"
#import "PlayerModel.h"
#import "GameModel.h"


@implementation ShipDesignerViewController


@synthesize theHulls;
@synthesize theHull;
@synthesize theHullShapeMask;


//
// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
//
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])
    {
        //
        // Get an array of all the known hulls from the Model.
        //
        [self setTheHulls:[[GameModel instance] allHulls]];
        
        //
        // Get the shape mask for the first hull and use that.
        //
        [self setTheHull:[theHulls objectAtIndex:0]];
        [self setTheHullShapeMask:[theHull ShapeMask]];        
        
    }
    
    return self;
}

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
    // Set up the text fields.
    //
    [theHullNameTextField setText:[theHull Name]];
    [theShipNameTextField setText:@"Type01"];
    
    //
    // Disable the read only fields.
    //
    //[theHullNameTextField setEnabled:NO];
    [theGermaniumTextField setEnabled:NO];
    [theIroniumTextField setEnabled:NO];
    [theBoraniumTextField setEnabled:NO];
    
    ShipDesignerView*   hullView    = (ShipDesignerView*)[self view];
    [hullView setShipTemplateName:[theShipNameTextField text]];
    [theHullNameTextField setText:[hullView shipTemplateName]];
    [hullView setShapeMask:[theHull ShapeMask]];
    
    //
    // Create the default ShipTemplate.
    //
    [self createShipTemplateWithName:[theShipNameTextField text] andHull:theHull];

    
    //
    //
    //
    /*
    CGRect  pickerFrame = [theHullPickerView frame];
    int     height      = pickerFrame.size.height;
    pickerFrame.origin.y       = 480-height;
    
    [theHullPickerView setFrame:pickerFrame];
    [[self view] insertSubview:theHullPickerView atIndex:0];
     */
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


- (void)dealloc
{
    [super dealloc];
}



//
//
//
-(void)hullChosen:(Hull*)newHull
{
    NSLog(@"Hull %@ chosen", [newHull Name]);
    
    //
    //
    //
    [self setTheHull:newHull];
    [self setTheHullShapeMask:[newHull ShapeMask]];    
    
    [(ShipDesignerView*)[self view] setShapeMask:[newHull ShapeMask]];
    
    //
    // Update the text fields.
    //
    [theHullNameTextField setText:[theHull Name]];
    
    //
    //
    //
    [[self view] setNeedsDisplay];
}


//
//
//
-(IBAction)hullNameTouched:(id)sender
{
    //
    // Go to the Map view.
    //
    HullChooserViewController*      newViewController   = [[HullChooserViewController alloc] initWithNibName:@"HullChooserView" bundle:nil selector:@selector(hullChosen:) object:self];
    
    [self.navigationController pushViewController:newViewController animated:YES];
    [newViewController release];
}


//
//
//
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)thePickerView
{
    
    return 1;
}


//
//
//
- (NSInteger)pickerView:(UIPickerView *)thePickerView numberOfRowsInComponent:(NSInteger)component
{
    return 0;
}

//
// Create a ship template with the given name and using the given Hull.
//
-(void)createShipTemplateWithName:(NSString*)name andHull:(Hull*)hull
{
    ShipTemplate*   template    = [[PlayerModel instance] getShipTemplateForName:name AndHull:hull];

    //
    // See if the Template already exists.
    //
    if(template == nil)
    {
        //
        // Attempt to create the template.
        //
        template    = [[PlayerModel instance] addShipTemplateWithName:name andHull:hull];
        if(template != nil)
        {
            [template setShipTemplateToHull:hull];
            ShipDesignerView*   hullView    = (ShipDesignerView*)[self view];
            [hullView setShipTemplateName:name];
            
        }
        else
        {
            NSLog(@"Cold not create ShipTemplate named %@",name);
        }
    }
    else
    {
        NSLog(@"Ship template for name %@ already exists.",name);
    }
}


//
//
//
-(void)techItemChosen:(TechnologyItem*)item atPosition:(CGPoint)position
{
    ShipTemplate*   template        = [[PlayerModel instance] getShipTemplateForName:[theShipNameTextField text] AndHull:nil];                        
    NSArray*        fixings         = [[PlayerModel instance] techFixingsForShipTemplate:template atPosition:position];
    NSUInteger      numberOfFixings = [fixings count];
    
    if((numberOfFixings != 0) && (fixings != nil))
    {
        //
        // Set the fixing at this point to hold this item.
        //
        TechFixing* fixing  = [fixings objectAtIndex:0];
        [fixing setTechFixingToTechnologyItem:item];
        
        int x   = [[fixing xPosition] intValue];
        int y   = [[fixing yPosition] intValue];
        NSLog(@"setting %@ at (%d,%d) on ship \"%@\"",[item Name], x,y, [template Name]);
    }
    else
    {
        //
        // create a new fixing at this point for this item.
        //
        [[PlayerModel instance] addTechFixingForItem:item toShipTemplate:template atPosition:position];
        
        int x   = position.x;
        int y   = position.y;
        NSLog(@"adding %@ at (%d,%d) on ship \"%@\"",[item Name], x,y, [template Name]);
    }
    
    NSLog(@"tech item chosen %@", [item Name]);
}



//
//
//
- (void)textFieldDidEndEditing:(UITextField *)textField
{
    NSString*       templateName    = [textField text];
    
    //
    // the ship name has been set, so lets set up the entry in the database.
    //
    [self createShipTemplateWithName:templateName andHull:theHull];
}


//
//
//
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    return YES;
}

//
//
//
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    
    return NO;
}



@end
