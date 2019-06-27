//
//  GameSetupItemHelpView.h
//  Starz
//
//  Created by Steve Tickle on 26/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface GameSetupItemHelpViewController : UIViewController
{
    IBOutlet UITextView*    textView;
    NSString*               helpText;
}

@property (retain) UITextView*          textView;
@property (retain) NSString*            helpText;

@end
