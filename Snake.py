"""
"""


import Coaster
import random
import math

class Snake(Coaster.CoasterWindow):
    """
    The Chameleoaster Class - This class handles the main 
    initialization and creating of the Game.
    """
    
    def __init__(self, blockSize):
        """
        """
        Coaster.CoasterWindow.__init__(self, blockSize)

        self.frameCount             = 0
        self.flashColour            = None
        self.flashCount             = 0
        self.Reset()



    def IsPositionWithinBody(self, hx,hy ):
        """
        """
        for x,y,colour in self.body:
            if hx == x and hy == y:
                return True

        return False


    def Reset(self):
        """
        """
        self.body                       = [(8,6,(0,255,0))]
        self.xOffset                    = 1
        self.yOffset                    = 0
        self.speed                      = 10
        self.increaseDifficultyInterval = 30
        self.bodyLength                 = 2     # score        

        self.MakeFood()


    def Flash(self, colour):
        """
        """
        self.flashColour    = colour
        self.flashCount     = 0


    def MoveSnake(self):
        """
        """

        #
        # Add an element to the head in the direction we're moving in.
        #
        oldX,oldY,c = self.body[0]
        newX        = (oldX+self.xOffset) % 15
        newY        = (oldY+self.yOffset) % 12

        #
        # Detect if we hit our own body or not.
        #
        if self.IsPositionWithinBody( newX,newY ) == True:
            self.Reset()
        else:
            self.body   = [( newX,newY, (0,255,0) )] + self.body

        #
        # Detect if we hit the food or not.
        #
        if newX == self.foodX and newY == self.foodY:            
            self.MakeFood()
            self.bodyLength = self.bodyLength + 1

            self.Flash( (0,0,255) )

        #
        # If the body is too long, chop off the tail.
        #
        while len(self.body) > self.bodyLength:
            self.body   = self.body[:-1]


    def DrawSnake(self):
        """
        """
        brightness  = 1.0
        for x,y,colour in self.body:
            r,g,b       = colour

            self.SetPixelColour( self.frame, x,y, int(r*brightness),int(g*brightness),int(b*brightness) )

            brightness  = brightness - (1.0 / len(self.body) )



    def Clear(self, colour):
        """
        """
        r,g,b   = colour

        for x in range(16):
            for y in range(12):
                self.SetPixelColour( self.frame, x,y, r,g,b )


    def MakeFood(self):
        """
        """
        self.foodX   = random.randint(0,15)
        self.foodY   = random.randint(0,11)
        print('*** New food @ %d,%d ***'%(self.foodX,self.foodY) )



    def DrawFrame(self):
        """
        """

        #
        # Keep track of time.
        #
        self.frameCount = self.frameCount + 1

        #
        # Move the snake every so often...
        #
        if self.frameCount % self.speed == 0:
            self.MoveSnake()

        #
        # Increase the speed every so often...
        #
        if self.frameCount % self.increaseDifficultyInterval == 0:
            if self.speed > 1:
                self.speed  = self.speed - 1


        #
        # Flash if needed.
        #
        backgroundColour    = (0,0,0)

        if self.flashColour != None and (self.flashCount%2) == 0:
            backgroundColour    = self.flashColour

        if self.flashCount > 3:
            self.flashColour    = None

        self.flashCount     = self.flashCount + 1

        #
        # Draw the game display.
        #
        self.Clear(backgroundColour)
        self.DrawSnake()
        self.SetPixelColour( self.frame, self.foodX,self.foodY, 255,0,0 )





    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition

        px  = x/self.blockSize
        py  = y/self.blockSize

        print(touchPosition)

        headX,headY,colour = self.body[0]

        dx  = abs(px - headX)
        dy  = abs(py - headY)

        self.xOffset    = 0
        self.yOffset    = 1

        if dx > dy and px > headX:
            self.xOffset    = 1
            self.yOffset    = 0

        if dx > dy and px < headX:
            self.xOffset    = -1
            self.yOffset    = 0

        if dy > dx and py > headY:
            self.xOffset    = 0
            self.yOffset    = 1

        if dy > dx and py < headY:
            self.xOffset    = 0
            self.yOffset    = -1





if __name__ == "__main__":
    random.seed()
    MainWindow = Snake(48)
    MainWindow.MainLoop()



