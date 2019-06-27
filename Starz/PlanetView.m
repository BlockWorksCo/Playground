//
//  PlanetView.m
//  Starz
//
//  Created by Steve Tickle on 06/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "PlanetView.h"


@implementation PlanetView



- (void)dealloc
{
    [super dealloc];
}


#if 0
- (void)drawRect:(CGRect)rect
{
    // Drawing code
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	CGContextClearRect(ctx, rect);
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 1.0);
	CGContextFillRect(ctx, rect);
}

//
//
//
- (void)drawInContext:(CGContextRef)ctx
{
    [super drawInContext:ctx];
/*
    CGRect bounds = [self bounds];
    CGRect frame = [self frame];
    CGRect rect = CGRectMake(0,0,100,100);
	//CGContextClearRect(ctx, rect);
	CGContextSetRGBFillColor(ctx, 1.0, 0.0, 0.0, 1.0);
	CGContextFillRect(ctx, rect);

	CGContextSetRGBFillColor(ctx, 0.0, 1.0, 0.0, 1.0);
	CGContextFillEllipseInRect(ctx, rect);
    
    //
    //
    //
    CGContextSetRGBFillColor (ctx, 1.0, 1.0, 1.0, 1.0);
    CGContextSetRGBStrokeColor (ctx, 1.0, 1.0, 1.0, 1.0);
    CGContextShowTextAtPoint(ctx, 10, 10, "Hello World",11);
*/
/*
	NSLog(@"drin");
	CGContextBeginPath (ctx); 
	[[UIColor redColor] setFill]; 
	[[UIColor redColor] setStroke]; 
	CGContextMoveToPoint (ctx, 50.0, 0.9); 
	CGContextAddLineToPoint (ctx, 0.0, 50.0);
	CGContextAddLineToPoint (ctx, 100.0, 50.0);
	CGContextClosePath (ctx); 
	CGContextDrawPath (ctx, kCGPathFillStroke);
*/
}


//
//
//
- (void)drawLayer:(CALayer *)theLayer inContext:(CGContextRef)ctx
{
    /*
    CGMutablePathRef thePath = CGPathCreateMutable();
    
    CGPathMoveToPoint(thePath,NULL,15.0f,15.f);
    CGPathAddCurveToPoint(thePath,
                          NULL,
                          15.f,250.0f,
                          295.0f,250.0f,
                          295.0f,15.0f);
    
    CGContextBeginPath(theContext);
    CGContextAddPath(theContext, thePath );
    
    CGContextSetLineWidth(theContext,
                          [[theLayer valueForKey:@"lineWidth"] floatValue]);
    CGContextStrokePath(theContext);
    
    // release the path
    CFRelease(thePath);
     */
    CGRect bounds = [self bounds];
    //CGRect frame = [self frame];
    CGRect rect = CGRectMake(0, 0, bounds.size.width, bounds.size.height);
	//CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 1.0);
	//CGContextFillRect(ctx, rect);
    
	CGContextSetRGBFillColor(ctx, 0.0, 0.0, 1.0, 1.0);
	CGContextFillEllipseInRect(ctx, rect);
    
    //
    //
    //
    //CGContextSetRGBFillColor (ctx, 1.0, 1.0, 1.0, 1.0);
    //CGContextSetRGBStrokeColor (ctx, 1.0, 1.0, 1.0, 1.0);
    //CGContextShowTextAtPoint(ctx, 10, 10, "Hello World",11);
    
}
#endif


//
//
//
+(void)createNewPlanetViewInLayer:(CALayer*)parentLayer
{
    PlanetView* planetView  = [[PlanetView alloc] init];
    //[planetView setFrame:CGRectMake(0, 0, 250, 250)];
    [planetView setBounds:CGRectMake(0, 0, 100,100)];
    //[planetView setMasksToBounds:YES];
    
    [parentLayer addSublayer:planetView];
    
    //[planetView setBackgroundColor:[UIColor redColor]];
    [planetView setPosition:CGPointMake(100,150)];
    //[planetView setContentsRect:CGRectMake(0,0,250,250)];
    [planetView setAnchorPoint:CGPointMake(0,0)];
    [planetView setNeedsDisplay];
    [planetView setDelegate:planetView];
    //[planetView setNeedsLayout];
}



@end
