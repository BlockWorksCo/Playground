import os, sys
import pygame
from pygame.locals import *
import time

if not pygame.font: print 'Warning, fonts disabled'
if not pygame.mixer: print 'Warning, sound disabled'

import random

class Chameleoaster:
    """
    The Chameleoaster Class - This class handles the main 
    initialization and creating of the Game.
    """
    
    def __init__(self, width=32*16,height=32*12):
        """
        """
        pygame.init()
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((self.width, self.height))

        self.frame  =  [[0 for i in xrange(16)] for i in xrange(12)]


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


    def MainLoop(self):
        """
        This is the Main Loop of the Game
        """        
        while 1:

            for x in range(16):
                for y in range(12):
                    r   = random.randint(0,255)
                    g   = random.randint(0,255)
                    b   = random.randint(0,255)
                    c   = pygame.Color(r,g,b)
                    self.SetPixelColour(x,y, c )


            touchPosition   = self.GetTouches()
            if touchPosition != None:
                print(touchPosition)


            for event in pygame.event.get():
                if event.type == pygame.QUIT: 
                    sys.exit()

            for x in range(16):
                for y in range(12):
                    pixelColour     = self.frame[y][x]
                    pygame.draw.rect(self.screen, pixelColour, pygame.Rect([x*32,y*32],[32,32]), 0)

            pygame.display.update()

            time.sleep(0.05)





if __name__ == "__main__":
    MainWindow = Chameleoaster()
    MainWindow.MainLoop()



