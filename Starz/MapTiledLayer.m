//
//  MapTiledLayer.m
//  Starz
//
//  Created by Steve Tickle on 31/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "MapTiledLayer.h"
#import "PlanetView.h"


@implementation MapTiledLayer



//
//
//
/*
- (void)drawInContext:(CGContextRef)ctx
{
// 
    [super drawInContext:ctx];
    //NSLog(@"draw");
    CGMutablePathRef thePath = CGPathCreateMutable();
    
    CGPathMoveToPoint(thePath,NULL,15.0f,15.f);
    CGPathAddCurveToPoint(thePath,
                          NULL,
                          15.f,250.0f,
                          295.0f,250.0f,
                          295.0f,15.0f);
    
    CGContextBeginPath(ctx);
    CGContextAddPath(ctx, thePath );
    
    CGContextSetLineWidth(ctx,
                          [[self valueForKey:@"lineWidth"] floatValue]);
    CGContextStrokePath(ctx);
    
    // release the path
    CFRelease(thePath);    
}
 */



+ (CFTimeInterval)fadeDuration
{
    return 0;
}


- (void)dealloc
{
    [super dealloc];
}


@end
