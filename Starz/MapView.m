//
//  MapView.m
//  Starz
//
//  Created by Steve Tickle on 31/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "MapView.h"
#import <QuartzCore/CoreAnimation.h>
#import "MapTiledLayer.h"


@implementation MapView

   



//
//
//
/*
+(Class)layerClass
{
	//return [CALayer class];
	return [MapTiledLayer class];
	//return [CATiledLayer class];
	//return [CAShapeLayer class];
}
 */




/*
//
//
//
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self setBackgroundColor:[UIColor blackColor]];

}
*/

//
//
//
- (void)drawPlanetInContext:(CGContextRef)ctx XPosition:(int)x YPosition:(int)y Size:(int)size Name:(NSString*)name
{
    CGContextSetRGBFillColor(ctx, 255, 0, 0, 1);
    CGRect  planet  = CGRectMake(x,y,size,size);
    //CGContextFillEllipseInRect(ctx, planet);
    
    CGContextSetRGBFillColor(ctx, 0, 255, 0, 1);
    planet  = CGRectMake(x+3,y+3,size-6,size-6);
    //CGContextFillEllipseInRect(ctx, planet);
    
    //
    //
    //
    CGAffineTransform   transform = CGAffineTransformMake(1.0,0.0, 0.0, -1.0, 0.0, 0.0);    
    CGContextSetTextMatrix(ctx, transform);
    
    //
    //
    //
    CGContextSelectFont(ctx, "Verdana", 20, kCGEncodingMacRoman);
    //CGContextSetCharacterSpacing (ctx, 10); 
    CGContextSetTextDrawingMode (ctx, kCGTextStroke); 
	
    CGContextSetRGBFillColor (ctx, 1.0, 1.0, 1.0, 1.0);
    CGContextSetRGBStrokeColor (ctx, 1.0, 1.0, 1.0, 1.0);
    
    //
    //
    //
    char        cName[256];
    strcpy(&cName[0], [name UTF8String]);
    CGContextShowTextAtPoint(ctx, x, y, cName, strlen(cName));
}




/*
//
//
//
- (void)drawRect:(CGRect)rect
{
    //[super drawRect:rect];
 
    [self drawMap];
}
*/

- (void)dealloc
{
    [super dealloc];
}


@end
