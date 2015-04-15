#
#
#
#





links               = {}

liveWorkingMemory   = []
modelWorkingMemory  = []




def StrengthenLink( links, symbolA, symbolB ):
    """
    """
    if symbolA in links:
        if symbolB in links[symbolA]:
            links[symbolA][symbolB]     = links[symbolA][symbolB]+1
        else:
            links[symbolA][symbolB]     = 1
    else:
        links[symbolA] = {symbolB:1}



def DecayLink( links, symbolA, symbolB ):
    """
    """
    if symbolA in links:
        if symbolB in links[symbolA]:
            links[symbolA][symbolB]     = links[symbolA][symbolB]-1
        else:
            links[symbolA][symbolB]     = 1
    else:
        links[symbolA] = {symbolB:1}




def Cycle( memory, model ):
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









StrengthenLink( links, 0, 1 )
StrengthenLink( links, 0, 2 )
StrengthenLink( links, 0, 3 )
StrengthenLink( links, 1, 2 )
StrengthenLink( links, 1, 3 )
StrengthenLink( links, 2, 1 )
StrengthenLink( links, 2, 0 )

print(links)

StrengthenLink( links, 0, 1 )
StrengthenLink( links, 2, 0 )

print(links)




