#!/usr/bin/env python





import random
import math





def InitPopulation():
    random.seed()
    population  = []
    for i in range(100):
        population.append({'x':random.random(),'y':random.random(),'receivedData':'', 'receivedPower':0.0, 'inFlightPackets':[], 'RootNode':False}) 

    return population




def ProcessPacket(time, node):

    if node['receivedData'] != '':

        node['inFlightPackets'].append( node['receivedData'] )

        #packetAge   = time - node['receivedTime']
        #if packetAge > 10:
            #node['transmittingPacket']  = node['receivedData']
            #node['transmittingPower']   = 15

    return node



def CycleSim(time, population):

    # Clear the received packets
    for node in population:
        if node.get('receivedData') != None:
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
                    receivedPower           = fromNode['transmittingPower'] / 10*(distanceBetweenNodes*distanceBetweenNodes)
                    if receivedPower > 0.7 and receivedPower > toNode['receivedPower']:
                        toNode['receivedData']    = fromNode['transmittingPacket']
                        toNode['receivedPower']   = receivedPower
                        toNode['receivedTime']    = time
                        print(toNode)

    # Clear the transmittedData.
    for node in population:
        if node.get('transmittingPacket') != None:
            del node['transmittingPacket']

    # Process the received packets.
    for node in population:
        node    = ProcessPacket(time, node)

    return population



if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    time        = 0


    population[37]['transmittingPacket']    = 'Hello World'
    population[37]['transmittingPower']     = 15
    
    population[23]['RootNode']              = True

    while True:
        print(time)
        population  = CycleSim(time, population)
        time    = time + 1


