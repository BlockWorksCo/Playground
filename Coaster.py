"""
"""


import os
import sys
import pygame
import time
import random



class CoasterWindow:
    """
    The Chameleoaster Class - This class handles the main 
    initialization and creating of the Game.
    """
    
    def __init__(self, blockSize):
        """
        """
        pygame.init()
        self.width      = blockSize * 16
        self.height     = blockSize * 12
        self.blockSize  = blockSize
        self.screen = pygame.display.set_mode((self.width, self.height))

        self.frame  =  [[0 for i in xrange(16)] for i in xrange(12)]

        for x in range(16):
            for y in range(12):
                r   = random.randint(0,255)
                g   = random.randint(0,255)
                b   = random.randint(0,255)
                self.SetPixelColour(x,y, r,g,b )


    def GetPixelColour(self, x,y):
        """
        """
        #print('%d,%d'%(x,y))
        r,g,b,a = self.frame[y][x]
        return r,g,b


    def SetPixelColour(self, x,y, r,g,b):
        """
        """
        self.frame[y][x]    = pygame.Color(r,g,b)        


    def GetTouches(self):
        """
        """
        button1,button2,button3     = pygame.mouse.get_pressed()

        if button1 == True or button2 == True or button3 == True:
            position = pygame.mouse.get_pos()
        else:
            position = None

        return position



    def WrappedCoordinate(self,x,y):
        """
        """
        while x < 0:
            x = x + 16

        while y < 0:
            y = y + 12

        while x >= 16:
            x = x - 16

        while y >= 12:
            y = y - 12

        #print('-- %d %d --'%(x,y))

        return x,y


    def GetWrappedPixelColour( self, position ):
        """
        """
        x,y = position
        wrappedX,wrappedY = self.WrappedCoordinate( x,y )
        return self.GetPixelColour( wrappedX,wrappedY )



    def DrawFrame(self):
        """
        """
        pass


    def CopyFrameToWindow(self):
        """
        """
        for x in range(16):
            for y in range(12):
                pixelColour     = self.frame[y][x]
                pygame.draw.rect(self.screen, pixelColour, pygame.Rect([x*self.blockSize,y*self.blockSize],[self.blockSize,self.blockSize]), 0)

        pygame.display.update()



    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition
        print(touchPosition)




    def MainLoop(self):
        """
        This is the Main Loop of the Game
        """        
        while True:

            #
            # Draw something interesting in  the frame.
            #
            self.DrawFrame()

            #
            # Handle touches
            #
            touchPosition   = self.GetTouches()
            if touchPosition != None:
                self.HandleTouch(touchPosition)


            #
            # Handle window events
            #
            for event in pygame.event.get():
                if event.type == pygame.QUIT: 
                    sys.exit()

            #
            # Copy the frame to the physical display.
            #
            self.CopyFrameToWindow()

            #
            # Wait a bit.
            #
            time.sleep(0.1)


