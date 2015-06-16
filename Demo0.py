"""
"""


import Coaster


class Demo0(Coaster.CoasterWindow):
    """
    Simple coaster demo.
    """
    
    def __init__(self, blockSize):
        """
        """
        Coaster.CoasterWindow.__init__(self, blockSize)

        for x in range(16):
            for y in range(12):
                self.SetPixelColour( self.frame, x,y, 255,0,0 )

        self.frameCount = 0





    def DrawFrame(self):
        """
        """

        self.frameCount = self.frameCount + 1

        if (self.frameCount%10) == 0:
            self.SetPixelColour( self.frame, 7,5, 255,0,0 )
            print('On')

        if (self.frameCount%10) == 5:
            self.SetPixelColour( self.frame, 7,5, 0,0,0 )
            print('Off')



    def HandleTouch(self, touchPosition):
        """
        """
        x,y = touchPosition
        print(touchPosition)



if __name__ == "__main__":
    MainWindow = Demo0(48)
    MainWindow.MainLoop()



