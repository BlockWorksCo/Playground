#
#
#
#



class Generation:
    """
    """

    links               = {}

    liveWorkingMemory   = []
    modelWorkingMemory  = []


    def __init__(self):
        """
        """
        pass


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




    def Cycle( self, memory, model ):
        """
        """

        #
        # Strengthen the links between symbols in working memory.
        #
        for symbolFrom in memory:
            for symbolTo in memory:
                StrengthenLink( symbolFrom, symbolTo )

        #
        # Decay links between all symbols
        #
        for symbolFrom in symbols:
            for symbolTo in symbols:
                DecayLinks( symbolFrom, symbolTo )








generation0     = Generation()


generation0.StrengthenLink( generation0.links, 0, 1 )
generation0.StrengthenLink( generation0.links, 0, 2 )
generation0.StrengthenLink( generation0.links, 0, 3 )
generation0.StrengthenLink( generation0.links, 1, 2 )
generation0.StrengthenLink( generation0.links, 1, 3 )
generation0.StrengthenLink( generation0.links, 2, 1 )
generation0.StrengthenLink( generation0.links, 2, 0 )

print(generation0.links)

generation0.StrengthenLink( generation0.links, 0, 1 )
generation0.StrengthenLink( generation0.links, 2, 0 )

print(generation0.links)




