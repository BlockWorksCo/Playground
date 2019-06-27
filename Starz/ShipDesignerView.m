//
//  ShipDesignerView.m
//  Starz
//
//  Created by Steve Tickle on 18/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "ShipDesignerView.h"
#import "TechnologyItemChooserViewController.h"
#import "ShipDesignerViewController.h"
#import "TechnologyItem.h"
#import "StarzAppDelegate.h"
#import "Model.h"
#import "TechFixing.h"
#import "Hull.h"
#import "ShipTemplate.h"
#import "PlayerModel.h"


@implementation ShipDesignerView


@synthesize touchedPoint;
@synthesize ShapeMask;
@synthesize shipTemplateName;


//
//
//
- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
    {
        //
        //
        //
        [self setTouchedPoint:CGPointZero];
    }
    return self;
}



//
//
//
- (void)drawRect:(CGRect)rect
{
    CGContextRef    context     = UIGraphicsGetCurrentContext();    
    
    if([ShapeMask length] != 72)
    {
        NSLog(@"blaa");
    }
    
    //
    // Draw the hull items.
    //
    for(int i=0; i<12; i++)
    {
        for(int j=0; j<6; j++)
        {
            int             shapeIndex  = (j*12)+i;
            unichar         type        = [ShapeMask characterAtIndex:shapeIndex];
            if(type != ' ')
            {
                CGContextSetRGBFillColor(context, 255, 0, 0, 1);
                CGContextSetRGBStrokeColor(context, 0, 0, 255, 1.0);
                CGContextSetLineWidth(context, 3);
                
                CGRect          newRect     = CGRectMake(40+(i*20), 50+(j*40), 18, 38);
                CGContextFillRect(context, newRect);    
            }
            
        }
    }
    
    //
    // Draw the touched item.
    //
    int     shapeIndex      = (12*touchedItem.y)+touchedItem.x;
    unichar type            = [ShapeMask characterAtIndex:shapeIndex];
    if((type != ' ') && (touchedItem.y >= 0) && (touchedItem.x >= 0))
    {
        CGContextSetRGBFillColor(context, 0, 255, 0, 1);
        CGRect          newRect     = CGRectMake(touchedPoint.x, touchedPoint.y, 40, 40);
        CGContextFillRect(context, newRect);
    }

    
}



//
//
//
-(void)processTouches: (NSSet *) touches withEvent: (UIEvent *) event
{
    //
    // Determine which module was touched.
    //
    CGPoint    point        = [[touches anyObject] locationInView:self];
    CGRect      hullRect    = CGRectMake(40, 50, 240, 270);
    
    //
    //
    //
    if(CGRectContainsPoint(hullRect, point) == YES)
    {
        touchedItem.x   = (int)(point.x - 40) / 20;
        touchedItem.y   = (int)(point.y - 50) / 40;
        if(touchedItem.y >= 5)
        {
            touchedItem.y = 5;
        }
        if(touchedItem.x >= 11)
        {
            touchedItem.x = 11;
        }
        
        int     shapeIndex      = (12*touchedItem.y)+touchedItem.x;
        unichar type            = [ShapeMask characterAtIndex:shapeIndex];
        if(type == ' ')
        {
            touchedItem.x   = -1;
            touchedItem.y   = -1;
        }
        else if(type == 'x')
        {
        }
        else if(type == 'y')
        {
            touchedItem.x   = touchedItem.x - 1;
        }
        
        touchedPoint.x  = (touchedItem.x*20)+40;
        touchedPoint.y  = (touchedItem.y*40)+50;
        
        [self setNeedsDisplay];
    }
}



//
//
//
-(void)techItemChosen:(TechnologyItem*)item
{
    UINavigationController*         navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
    NSArray*                        controllers     = [navController viewControllers];
    ShipDesignerViewController*     viewController  = (ShipDesignerViewController*)[controllers objectAtIndex:[controllers count]-2];
    
    //
    //
    //
    [viewController techItemChosen:item atPosition:touchedItem];
}


//
//
//
-(void)logShipTemplate:(ShipTemplate*)template
{
    NSArray*        fixings         = [[PlayerModel instance] allTechFixingsForShipTemplate:template];
    NSUInteger      numberOfFixings = [fixings count];
    Hull*           hull            = [template ShipTemplateToHull];
    
    NSLog(@"ShipTemplate \"%@\", hull \"%@\" %d fixings:", [template Name], [hull Name], numberOfFixings);
    for(int i=0; i<numberOfFixings; i++)
    {
        TechFixing*     fixing  = [fixings objectAtIndex:i];
        TechnologyItem* item    = [fixing TechFixingToTechnologyItem];
        
        NSLog(@"%d) %@ at position %@,%@", i, [item Name],[fixing xPosition],[fixing yPosition]);
    }
}


//
//
//º
-(void)logAllShipTemplates
{
    NSArray*        templates;
    
    //
    //
    //
    templates    = [[PlayerModel instance] allShipTemplatesWithHullType:0];
    NSLog(@"Hull type 0");
    for(int i=0; i<[templates count]; i++)
    {
        ShipTemplate*   template        = [templates objectAtIndex:i];
        [self logShipTemplate:template];
    }

    templates    = [[PlayerModel instance] allShipTemplatesWithHullType:1];
    NSLog(@"Hull type 1");
    for(int i=0; i<[templates count]; i++)
    {
        ShipTemplate*   template        = [templates objectAtIndex:i];
        [self logShipTemplate:template];
    }

    templates    = [[PlayerModel instance] allShipTemplatesWithHullType:2];
    NSLog(@"Hull type 2");
    for(int i=0; i<[templates count]; i++)
    {
        ShipTemplate*   template        = [templates objectAtIndex:i];
        [self logShipTemplate:template];
    }
}

//
//
//
- (void) touchesBegan: (NSSet *) touches withEvent: (UIEvent *) event
{
    [self processTouches:touches withEvent:event];
}

- (void) touchesEnded: (NSSet *) touches withEvent: (UIEvent *) event
{
    [self processTouches:touches withEvent:event];
    
    //
    // If an item has been touched...
    //
    if((touchedItem.x >= 0) && (touchedItem.y >= 0))
    {
        //
        // Go to the TechChooser view.
        //
        TechnologyItemChooserViewController*      newViewController   = [[TechnologyItemChooserViewController alloc] initWithNibName:@"TechnologyItemChooserView" bundle:nil selector:@selector(techItemChosen:) object:self];
        
        UINavigationController* navController   = [(StarzAppDelegate*)[[UIApplication sharedApplication] delegate] navigationController];
        [navController pushViewController:newViewController animated:YES];
        [newViewController release];
    }
    else
    {
        //
        // Log all ship templates.
        //
        [self logAllShipTemplates];
    }
}

- (void) touchesMoved: (NSSet *) touches withEvent: (UIEvent *) event
{
    [self processTouches:touches withEvent:event];
}





//
//
//
- (void)dealloc
{
    [super dealloc];
}


@end
