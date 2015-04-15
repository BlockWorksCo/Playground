#
#
#
#



timestamp   = 0
def CurrentTimestamp():
    """
    """
    return timestamp + 1






symbols             = set( [0] )
links               = {}

liveWorkingMemory   = []
modelWorkingMemory  = []




def LinkSymbolToSymbol( links, symbolA, symbolB ):
    """
    """
    symbols.add( symbolA )
    symbols.add( symbolB )

    if symbolA in links:
        if symbolB in links[symbolA]:
            links[symbolA][symbolB]     = links[symbolA][symbolB]+1
        else:
            links[symbolA][symbolB]     = 1
    else:
        links[symbolA] = {symbolB:1}



def Cycle( memory, model ):
    """
    """
    newSymbols  = FindNewSymbols( symbols )
    for symbol in newSymbols:
        InsertSymbol( symbols, symbol )








LinkSymbolToSymbol( links, 0, 1 )
LinkSymbolToSymbol( links, 0, 2 )
LinkSymbolToSymbol( links, 0, 3 )
LinkSymbolToSymbol( links, 1, 2 )
LinkSymbolToSymbol( links, 1, 3 )
LinkSymbolToSymbol( links, 2, 1 )
LinkSymbolToSymbol( links, 2, 0 )

print(symbols)
print(links)

LinkSymbolToSymbol( links, 0, 1 )
LinkSymbolToSymbol( links, 2, 0 )

print(symbols)
print(links)




