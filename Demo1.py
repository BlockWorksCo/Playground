"""
"""


import Coaster
import random


class Demo1(Coaster.CoasterWindow):
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


    def DrawFrame(self):
        """
        """
        for x in range(16):
            for y in range(12):
                currentR,currentG,currentB  = self.GetPixelColour(x,y)
                r   = int(currentR * 0.9)
                g   = int(currentG * 0.9)
                b   = int(currentB * 0.9)
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
    MainWindow = Demo1(48)
    MainWindow.MainLoop()



