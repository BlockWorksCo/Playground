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
                self.SetPixelColour(x,y, r,g,b )


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
        for x in range(16):
            for y in range(12):

                pixelValues     = []
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( (x+0, y-1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( (x+0, y+1) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( (x-1, y+0) )]
                pixelValues     = pixelValues + [self.GetWrappedPixelColour( (x+1, y+0) )]

                r,g,b = self.AverageColourOf( pixelValues )
                #currentR,currentG,currentB  = self.GetPixelColour(x,y)
                #r   = int(currentR * 0.9)
                #g   = int(currentG * 0.9)
                #b   = int(currentB * 0.9)
                self.SetPixelColour(x,y, r,g,b )



    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition
        print(touchPosition)

        r   = random.randint(0,255)
        g   = random.randint(0,255)
        b   = random.randint(0,255)

        self.SetPixelColour(x/self.blockSize,y/self.blockSize, r,g,b )




if __name__ == "__main__":
    MainWindow = Demo2(48)
    MainWindow.MainLoop()



