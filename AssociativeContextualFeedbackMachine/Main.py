#!/usr/bin/env python3


import time
import sys



contexts    = \
[
    {
        'Threshold':10,
        'Contents':[],
        'DecayRate':100,
    },
    {
        'Threshold':100,
        'Contents':[],
        'DecayRate':10,
    },
    {
        'Threshold':1000,
        'Contents':[],
        'DecayRate':1,
    },
]


numberOfSymbols     = 26
symbolStrengths     = [[0]*numberOfSymbols for i in range(numberOfSymbols)] 
symbolActivation    = [0]*numberOfSymbols


def StrengthenInContextSymbolStrengths(contextList):

    for context in contextList:
        for symbolA in context['Contents']:
            for symbolB in context['Contents']:
                if symbolA != symbolB:
                    symbolStrengths[symbolA][symbolB]   += 1
            


def DecayAssociationStrengths():
    pass


def PopulateContexts(cycleNumber):

    inputData   = "Hello World"
    position    = cycleNumber % len(inputData)
    symbol      = ord(inputData[position].lower()) - ord('a')

    for context in contexts:
        context['Contents'].append(symbol)

    print('[%03d]'%symbol)
    



def ShowStrengths(symbolStrengths):

    sys.stdout.write('\n')
    sys.stdout.write(' | ')
    for symbolB in range(numberOfSymbols):
        sys.stdout.write('%c '%(chr(ord('A')+symbolB)))

    sys.stdout.write('\n')
    sys.stdout.write(' |')
    for symbolB in range(numberOfSymbols):
        sys.stdout.write('--')

    sys.stdout.write('\n')
    for symbolA in range(numberOfSymbols):

        sys.stdout.write('%c| '%(chr(ord('A')+symbolA)))
        for symbolB in range(numberOfSymbols):
            strength    = symbolStrengths[symbolA][symbolB]
            c           = '.'
            if strength > 0:
                c   = chr(ord('a')+strength)
            sys.stdout.write('%c '%(c))
        print('')



if __name__ == '__main__':

    run         = True
    cycleNumber = 0

    while run == True:
        print('Cycle %d'%(cycleNumber))

        PopulateContexts(cycleNumber)
        StrengthenInContextSymbolStrengths(contexts)
        ShowStrengths(symbolStrengths)
        DecayAssociationStrengths()

        time.sleep(0.5)
        cycleNumber += 1

