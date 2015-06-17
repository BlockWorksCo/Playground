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
                self.SetPixelColour( self.frame, x,y, r,g,b )


    def GetPixelColour(self, frame, x,y):
        """
        """
        #print('%d,%d'%(x,y))
        r,g,b,a = frame[y][x]
        return r,g,b


    def SetPixelColour(self, frame, x,y, r,g,b):
        """
        """
        #x = x%16
        #y = y%12
        if x >= 0 and x < 16 and y >= 0 and y < 12:
            frame[y][x]    = pygame.Color(r,g,b)        




    def DrawCircle( self, frame, x0, y0, radius, r,g,b ):
        """
        """
        x               = radius;
        y               = 0;
        decisionOver2   = 1 - x         # Decision criterion divided by 2 evaluated at x=r, y=0

        while x >= y:
            self.SetPixelColour( frame,  x + x0,  y + y0,  r,g,b )
            self.SetPixelColour( frame,  y + x0,  x + y0,  r,g,b )
            self.SetPixelColour( frame, -x + x0,  y + y0,  r,g,b )
            self.SetPixelColour( frame, -y + x0,  x + y0,  r,g,b )
            self.SetPixelColour( frame, -x + x0, -y + y0,  r,g,b )
            self.SetPixelColour( frame, -y + x0, -x + y0,  r,g,b )
            self.SetPixelColour( frame,  x + x0, -y + y0,  r,g,b )
            self.SetPixelColour( frame,  y + x0, -x + y0,  r,g,b )
            y = y + 1
            if decisionOver2 <= 0:            
                decisionOver2 = decisionOver2 +  (2 * y + 1)   # Change in decision criterion for y -> y+1            
            else:
                x = x - 1
                decisionOver2 = decisionOver2 + (2 * (y - x) + 1)   # Change for y -> y+1, x -> x-1


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


    def GetWrappedPixelColour( self, frame, position ):
        """
        """
        x,y = position
        wrappedX,wrappedY = self.WrappedCoordinate( x,y )
        return self.GetPixelColour( frame, wrappedX,wrappedY )



    def DrawFrame(self):
        """
        """
        pass


    def CopyFrameToWindow(self, frame ):
        """
        """
        for x in range(16):
            for y in range(12):
                pixelColour     = frame[y][x]
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
            self.CopyFrameToWindow( self.frame )

            #
            # Wait a bit.
            #
            time.sleep(0.1)


