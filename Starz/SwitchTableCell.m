//
//  SliderTableCell.m
//  Starz
//
//  Created by Steve Tickle on 22/07/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "SwitchTableCell.h"
#import "Model.h"




@implementation SwitchTableCell


@synthesize itemName;



//
//
//
/*
- (id)initWithFrame:(CGRect)frame reuseIdentifier:(NSString *)reuseIdentifier 
{
    if (self = [super initWithFrame:frame reuseIdentifier:reuseIdentifier])
    {
        slider  = [[UISlider alloc] initWithFrame:frame];
        [self.contentView addSubview:slider];
    }
    return self;
}
*/



//
//
//
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])
    {
        //
        //
        //
        CGRect frame = [self.contentView bounds];

        //
        //
        //
        switchControl  = [[UISwitch alloc] initWithFrame:frame];
        [self.contentView addSubview:switchControl];
        
        //
        //
        //
        [switchControl addTarget:self action:@selector(valueChanged) forControlEvents:UIControlEventValueChanged];        
        
    }
    return self;
}



//
//
//
- (void)valueChanged
{
    //
    //
    //
    [targetObject setValue:[NSNumber numberWithBool:[switchControl isOn]] forKeyPath:keyPath];
}




//
//
//
- (void)layoutSubviews
{
    
    [super layoutSubviews];
    
	// getting the cell size
    CGRect contentRect = self.contentView.bounds;
    
	// In this example we will never be editing, but this illustrates the appropriate pattern
    if (!self.editing)
    {
        
		// get the X pixel spot
        CGFloat boundsX = contentRect.origin.x;
        contentRect.size.height = 150;
		CGRect frame;
        
		frame = CGRectMake(boundsX + 200, 6, 250, 30);
        switchControl.frame    = frame;
        
		CGRect textFrame = CGRectMake(boundsX + 10, 6, 190, 30);
        [[self textLabel] setFrame:textFrame];
	}    
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated 
{

    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}


- (void)dealloc 
{
    //
    // Lets unregister ourselves with the Model.
    //
    [targetObject removeObserver:self forKeyPath:keyPath];
    
    
    [super dealloc];
}




//
//
//
+ (int)height
{
    return 40;
}



//
//
//
- (void)observeValueForKeyPath:(NSString *)observedkeyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if([observedkeyPath isEqual:keyPath] == YES)
    {        
        BOOL   newValue        = [[change objectForKey:NSKeyValueChangeNewKey] boolValue];
        BOOL   existingValue   = [switchControl isOn];
        
        if(newValue != existingValue)
        {
            [switchControl setOn:newValue];
        }
    }
    else
    {
        //
        // Call super...
        //
        [super observeValueForKeyPath:observedkeyPath ofObject:object change:change context:context];
    }
    
}


//
//
//
- (void)prepareForReuse
{
    //
    // Lets unregister ourselves with the Model.
    //
    [targetObject removeObserver:self forKeyPath:keyPath];
    
	
    [super prepareForReuse]; //always call this!
	//reset custom stuff, which needs to be reseted
}

//
//
//
- (void)setItemName:(NSString*)name keyPath:(NSString*)newKeyPath target:(id)object helpText:(NSString*)newHelpText
{
    //
    //
    //
    itemName        = name;
    keyPath         = newKeyPath;
    helpText        = newHelpText;
    targetObject    = object;
    
    //
    // Get the Model object that represents the value in the table cell.
    //
    BOOL   value    = [[targetObject valueForKeyPath:keyPath] floatValue];
    
    //
    // Lets register to be notified when that value in the Model changes.
    //
    [targetObject addObserver:self forKeyPath:keyPath options:NSKeyValueObservingOptionNew context:nil];
    
    //
    // Set the value into the table cell.
    //
    self.textLabel.text = itemName;
    [switchControl setOn:value];
}

@end