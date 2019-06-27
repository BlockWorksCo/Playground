//
//  NameGenerator.m
//  Starz
//
//  Created by Steve Tickle on 01/08/2009.
//  Copyright 2009 ZooKoo. All rights reserved.
//

#import "NameGenerator.h"
#import <stdio.h>



@implementation NameGenerator



int     chainLength = 2;




//
//
//
void stripWhitespace(char* src, char* dst)
{
    *dst    = 0;
    while((*src) != 0)
    {
        int     c   = *src;
        if((isalnum(c) != 0) || (c == '\'') || (c == ' '))
        {
            *dst    = *src;
            src++;
            dst++;
            *dst    = 0;
        }
        else
        {
            src++;
        }
    }
}

//
//
//
- (id)initWithFile:(NSString*)fileName
{
    //
    //
    //
    self    = [super init];
    
    //
    //
    //
    theDictionary           = [[NSMutableDictionary alloc] init];
    theStartDictionaryKeys  = [[NSMutableArray alloc] init];
    
    //
    //
    //
    FILE*       inFile  = NULL;
    char        cFileName[256];
    
    //
    //
    //
    [fileName getFileSystemRepresentation:cFileName maxLength:sizeof(cFileName)];
    
    inFile  = fopen(cFileName, "r");
    if(inFile != NULL)
    {
        while(feof(inFile) == 0)
        {
            char    line[256];
            char    word[256];
            char*   result;
            
            result  = fgets(line, sizeof(line), inFile);
            if(result != NULL)
            {
                stripWhitespace(line, word);
                strcat(word, ".");
                
                //
                // Add the chains from the word to the dictionary
                //
                for(int i=0; i<strlen(word); i++)
                {
                    NSString*   newKey      = [NSString stringWithCString:&word[i] length:chainLength];
                    NSString*   newValue    = [NSString stringWithFormat:@"%c",word[i+chainLength]];
                    
                    //
                    //
                    //
                    
                    
                    //
                    //
                    //
                    NSMutableSet*    arrayForKey = [theDictionary objectForKey:newKey];
                    if(arrayForKey == 0)
                    {
                        NSMutableSet*    newArray    = [[NSMutableSet alloc] initWithObjects:newValue,nil];
                        [theDictionary setValue:newArray forKey:newKey];
                        
                        if(i == 0)
                        {
                            [theStartDictionaryKeys addObject:newKey];
                        }
                    }
                    else
                    {
                        NSMutableSet*     array       = (NSMutableSet*)[theDictionary valueForKey:newKey];
                        [array addObject:newValue];

                        if(i == 0)
                        {
                            [theStartDictionaryKeys addObject:newKey];
                        }
                    }
                }
                
                //
                // Get the keys array from the dictionary to allow random access to the keys.
                //
                theDictionaryKeys   = [theDictionary allKeys];
                
               // NSLog([NSString stringWithCString:word]);
            }
        }
        
        fclose(inFile);
    }
    else
    {
        NSLog(@"Cant open names file.");
        return 0;
    }
    
    
    return self;
}


//
//
//
- (void)dealloc
{
    [super dealloc];
}




//
//
//
- (NSString*)nextName
{
    srand(clock());
    
    char                temp[256];
    int                 randomIndex     = rand() % ([theStartDictionaryKeys count]-1);
    NSString*           newKey          = [theStartDictionaryKeys objectAtIndex:randomIndex];
    NSMutableString*    newName         = [NSMutableString stringWithString:newKey];
    NSString*           nextKey         = newKey;
    NSString*           characterToAppend;    
    
    do
    {
        NSSet*              valuesForKey        = [theDictionary valueForKey:nextKey];
        NSArray*            valuesArray         = [valuesForKey allObjects];
        int                 arraySize           = [valuesArray count];
        int                 randomIndex         = rand() % arraySize;
        
        characterToAppend   = [valuesArray objectAtIndex:randomIndex];        
        if([characterToAppend isEqual:@"."] == NO)
        {
            [newName appendFormat:@"%@",characterToAppend];
        }
        
        nextKey         = [newName substringFromIndex:[newName length]-chainLength];
        
        strcpy(&temp[0], [characterToAppend UTF8String]);
                
    } while(([characterToAppend isEqual:@"."] == NO) && ([newName length] < 10));
    
    //NSLog(newName);
    
    return newName;
}

@end







