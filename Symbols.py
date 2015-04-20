#
#
#
#



class Generation:
    """
    """

    links               = {}
    inputSymbols        = []

    liveWorkingMemory   = []
    modelWorkingMemory  = []

    youngerGeneration   = None


    def __init__(self, youngerGeneration=None ):
        """
        """
        self.youngerGeneration  = youngerGeneration



    def StrengthenLink( self, links, symbolA, symbolB ):
        """
        """
        if symbolA in links:
            if symbolB in links[symbolA]:
                links[symbolA][symbolB]     = links[symbolA][symbolB]+1
            else:
                links[symbolA][symbolB]     = 1
        else:
            links[symbolA] = {symbolB:1}




    def DecayLink( self, links, symbolA, symbolB ):
        """
        """
        if symbolA in links:
            if symbolB in links[symbolA]:
                links[symbolA][symbolB]     = links[symbolA][symbolB]-1
            else:
                links[symbolA][symbolB]     = 1
        else:
            links[symbolA] = {symbolB:1}




    def PushInto( self, links, symbol ):
        """
        """
        self.inputSymbols.append( symbol )

        #
        # Strengthen the links between symbols in working memory.
        #
        for symbolFrom in self.inputSymbols:
            for symbolTo in self.inputSymbols:
                if symbolTo != symbolFrom:
                    self.StrengthenLink( links, symbolFrom, symbolTo )




    def Cycle( self, links ):
        """
        """
        for symbolFrom in links[0]:
            pass




links           = {}
generation0     = Generation()
generation1     = Generation( generation0 )

generation0.PushInto( links, 0 )
generation0.PushInto( links, 1 )
generation0.PushInto( links, 2 )
generation0.PushInto( links, 3 )

generation1.Cycle( links )

print( links )




