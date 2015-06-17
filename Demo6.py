"""
"""


import Coaster
import random


class Demo3(Coaster.CoasterWindow):
    """
    The Chameleoaster Class - This class handles the main 
    initialization and creating of the Game.
    """
    
    def __init__(self, blockSize):
        """
        """
        Coaster.CoasterWindow.__init__(self, blockSize)

        for x in range(16):
            for y in range(12):
                r   = random.randint(0,255)
                g   = random.randint(0,255)
                b   = random.randint(0,255)
                self.SetPixelColour( self.frame, x,y, r,g,b )

        self.frameCount = 0

        self.dropR   = random.randint(0,254)
        self.dropG   = random.randint(0,254)
        self.dropB   = random.randint(0,254)

        self.rippleRadius   = 10
        self.rippleX        = 0
        self.rippleY        = 0



    def AverageColourOf( self, pixelValues ):
        """
        """
        averageR    = 0
        averageG    = 0
        averageB    = 0
        for r,g,b in pixelValues:
            averageR = averageR + r
            averageG = averageG + g
            averageB = averageB + b

        averageR = averageR / len(pixelValues)
        averageG = averageG / len(pixelValues)
        averageB = averageB / len(pixelValues)

        return (averageR, averageG, averageB)


    def DrawFrame(self):
        """
        """

        oldFrame    = self.frame
        newFrame    =  [[0 for i in xrange(16)] for i in xrange(12)]

        self.frameCount = self.frameCount + 1
        if (self.frameCount%2) == 0:
            x   = random.randint(0,15)
            y   = random.randint(0,11)

            r = self.dropR
            g = self.dropG
            b = self.dropB

            print('%d,%d'%(x,y))
            self.SetPixelColour( self.frame, x+0,y+0, r,g,b )
            self.SetPixelColour( self.frame, x+0,y-1, r,g,b )
            self.SetPixelColour( self.frame, x+0,y+1, r,g,b )
            self.SetPixelColour( self.frame, x-1,y+0, r,g,b )
            self.SetPixelColour( self.frame, x+1,y+0, r,g,b )

        if (self.frameCount%30) == 0:
            print('recolour')
            self.dropR   = random.randint(0,254)
            self.dropG   = random.randint(0,254)
            self.dropB   = random.randint(0,254)

            bgR   = random.randint(0,254)
            bgG   = random.randint(0,254)
            bgB   = random.randint(0,254)
            for x in range(16):
                for y in range(12):
                    self.SetPixelColour( oldFrame, x,y, bgR,bgG,bgB )

        for x in range(16):
            for y in range(12):

                pixelValues     = []
                
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x+0, y-1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x+0, y+1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x-1, y+0) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x+1, y+0) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x-1, y-1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x+1, y-1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x-1, y+1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( oldFrame, (x+1, y+1) )]

                r,g,b = self.AverageColourOf( pixelValues )
                r   = int(r * 0.8)
                g   = int(g * 0.8)
                b   = int(b * 0.8)
                self.SetPixelColour( newFrame, x,y, r,g,b )


        if self.rippleRadius < 15:
            #self.DrawCircle(  newFrame, self.rippleX,self.rippleY, self.rippleRadius+0, 0,255,0)
            self.DrawCircle(  newFrame, self.rippleX,self.rippleY, self.rippleRadius+1, 0,255,0)
            self.rippleRadius = self.rippleRadius + 1

        self.frame = newFrame



    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition
        print(touchPosition)

        r   = random.randint(0,254)
        g   = random.randint(0,254)
        b   = random.randint(0,254)

        px  = x/self.blockSize
        py  = y/self.blockSize

        self.rippleRadius   = 2
        self.rippleX        = px
        self.rippleY        = py




if __name__ == "__main__":
    MainWindow = Demo3(48)
    MainWindow.MainLoop()



