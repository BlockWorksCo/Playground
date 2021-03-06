"""
"""


import os
import sys
import pygame
import time
import random



class ChameleoasterWindow:
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
                c   = pygame.Color(r,g,b)
                self.SetPixelColour(x,y, c )


    def SetPixelColour(self, x,y, colour):
        """
        """
        self.frame[y][x]    = colour


    def GetTouches(self):
        """
        """
        button1,button2,button3     = pygame.mouse.get_pressed()

        if button1 == True or button2 == True or button3 == True:
            position = pygame.mouse.get_pos()
        else:
            position = None

        return position


    def DrawFrame(self):
        """
        """
        for x in range(16):
            for y in range(12):
                currentR,currentG,currentB,a  = self.frame[y][x]
                r   = int(currentR * 0.9)
                g   = int(currentG * 0.9)
                b   = int(currentB * 0.9)
                c   = pygame.Color(r,g,b)
                self.SetPixelColour(x,y, c )



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

        r   = random.randint(0,255)
        g   = random.randint(0,255)
        b   = random.randint(0,255)
        c   = pygame.Color(r,g,b)

        self.SetPixelColour(x/self.blockSize,y/self.blockSize, c )




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





if __name__ == "__main__":
    MainWindow = ChameleoasterWindow(48)
    MainWindow.MainLoop()



