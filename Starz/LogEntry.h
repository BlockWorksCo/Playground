//
//  LogEntry.h
//  Starz
//
//  Created by Steve Tickle on 23/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface LogEntry :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * Text;
@property (nonatomic, retain) NSNumber * Year;
@property (nonatomic, retain) NSManagedObject * LogEntryToGame;

@end



