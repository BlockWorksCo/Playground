//
//  NameGenerator.h
//  Starz
//
//  Created by Steve Tickle on 01/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NameGenerator : NSObject 
{
    NSMutableDictionary*    theDictionary;
    NSMutableArray*         theStartDictionaryKeys;
    NSArray*                theDictionaryKeys;
    
}


- (id)initWithFile:(NSString*)fileName;
- (NSString*)nextName;

@end
