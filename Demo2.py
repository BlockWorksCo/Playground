"""
"""


import Coaster
import random


class Demo2(Coaster.CoasterWindow):
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
                r   = int(r * 0.95)
                g   = int(g * 0.95)
                b   = int(b * 0.95)
                self.SetPixelColour( newFrame, x,y, r,g,b )

        self.frame = newFrame



    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition
        print(touchPosition)

        r   = random.randint(0,255)
        g   = random.randint(0,255)
        b   = random.randint(0,255)

        px  = x/self.blockSize
        py  = y/self.blockSize
        self.SetPixelColour( self.frame, px+0,py+0, r,g,b )
        self.SetPixelColour( self.frame, px+0,py-1, r,g,b )
        self.SetPixelColour( self.frame, px+0,py+1, r,g,b )
        self.SetPixelColour( self.frame, px-1,py+0, r,g,b )
        self.SetPixelColour( self.frame, px+1,py+0, r,g,b )




if __name__ == "__main__":
    MainWindow = Demo2(48)
    MainWindow.MainLoop()



