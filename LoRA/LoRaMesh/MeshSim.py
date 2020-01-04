#!/usr/bin/env python





import random
import math





def InitPopulation():
    random.seed()
    population  = []
    for i in range(100):
        population.append({'x':random.random(),'y':random.random()}) 

    return population



def CycleSim(time, population):

    # Clear the received packets.
    for node in population:
        node['receivedData']    = ''
        node['receivedPower']   = 0.0 

    # Transmit from each node to all other nodes, taking into account
    # threshold and otehr packets.
    for fromNode in population:
        if fromNode.get('transmittingPacket') != None:
            for toNode in population:
                if toNode != fromNode:
                    dx  = fromNode['x'] - toNode['x']
                    dy  = fromNode['y'] - toNode['y']
                    distanceBetweenNodes    = math.sqrt((dx*dx)+(dy*dy))
                    receivedPower           = distanceBetweenNodes/(fromNode['transmittingPower']*fromNode['transmittingPower'])
                    if receivedPower > 0.004 and receivedPower > toNode['receivedPower']:
                        toNode['receivedData']    = fromNode['transmittingPacket']
                        toNode['receivedPower']   = receivedPower
                        print(toNode)


if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    time        = 0


    population[37]['transmittingPacket']    = 'Hello World'
    population[37]['transmittingPower']     = 15

    while True:
        print(time)
        CycleSim(time, population)
        time    = time + 1


