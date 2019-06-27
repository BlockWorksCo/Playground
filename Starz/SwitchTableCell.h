//
//  SliderTableCell.h
//  Starz
//
//  Created by Steve Tickle on 22/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface SwitchTableCell : UITableViewCell
{
    UISwitch*       switchControl;
    NSString*       itemName;
    NSString*       keyPath;
    NSString*       helpText;
    id              targetObject;
}

@property(retain) NSString* itemName;

+ (int)height;
- (void)setItemName:(NSString*)name keyPath:(NSString*)newKeyPath target:(id)object helpText:(NSString*)newHelpText;


@end
