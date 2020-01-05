#!/usr/bin/env python





import random
import math
import binascii


xScale  = 8.0
yScale  = 8.0
ageBeforeForwarding    = 10



def InitPopulation():
    random.seed()
    population  = []
    for i in range(100):
        population.append({'x':random.random()*xScale,'y':random.random()*yScale,'receivedData':'', 'receivedPower':0.0, 'inFlightPackets':[], 'RootNode':False, 'nonACKedPackets':[]}) 

    return population




def ProcessPacket(time, node, nodeIndex):

    # Add new packet to in-flight packets. If the hash is not in the history-list
    if node['receivedData'] != '':

        # If this is an ACK-packet, remove the ACKed packet from the in-flight list.
        if node['receivedData'][:4] == 'ACK:':
            ackHash = int(node['receivedData'][4:])
            print('node %d received ACK for %d'%(nodeIndex,ackHash))
            inFlightPackets = node['inFlightPackets']
            for index,packet in enumerate(inFlightPackets):
                if binascii.crc32(packet['packet']) == ackHash:
                    print('node %d found ACKed-packet in in-flight packets, removing it.'%(nodeIndex))
                    node['inFlightPackets']     = inFlightPackets[0:index]+inFlightPackets[index+1:]

        else:
            # *NOT* and ACK packet.

            # If this packet is in the nonACKedPacket list, don't do anything else with it (yet).
            thisHash    = binascii.crc32(node['receivedData'])
            if thisHash not in node['nonACKedPackets']:
                node['inFlightPackets'].append( {'packet':node['receivedData'],'time':time} )
            else:
                print('node %d dropping [%s] because already seen'%(nodeIndex,node['receivedData']))
            
            # If this node is a root, generate an ACK packet for the received packet.
            if node['RootNode'] == True:
                print('node %d generating ACK for [%s]'%(nodeIndex,node['receivedData']))
                node['transmittingPacket']  = 'ACK:'+str(binascii.crc32(node['receivedData']))
                node['transmittingPower']   = 15
            

    # Check all in-flight packets for ready-to-transmit? transmit one and remove it from
    # the in-flight list.
    inFlightPackets = node['inFlightPackets']
    for index,packet in enumerate(inFlightPackets):
        packetAge   = time - packet['time']
        if packetAge > ageBeforeForwarding:
            print('node %d forwarding [%s] because age is %d and no ACK seen for it...'%(nodeIndex,packet['packet'],packetAge))
            node['transmittingPacket']  = packet['packet']
            node['transmittingPower']   = 15
            node['inFlightPackets']     = inFlightPackets[0:index]+inFlightPackets[index+1:]
            node['nonACKedPackets'].append(binascii.crc32( packet['packet'] ))
            break


    return node



def CycleSim(time, population):

    # Clear the received packets
    for node in population:
        if node.get('receivedData') != None:
            node['receivedData']    = ''
            node['receivedPower']   = 0.0

    # Transmit from each node to all other nodes, taking into account
    # threshold and other packets.
    for index,fromNode in enumerate(population):
        if fromNode.get('transmittingPacket') != None:
            print('node %d transmitting [%s]'%(index, fromNode['transmittingPacket']))
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
                        #print(toNode)

    # Clear the transmittedData.
    for node in population:
        if node.get('transmittingPacket') != None:
            del node['transmittingPacket']

    # Process the received packets.
    for index,node in enumerate(population):
        node    = ProcessPacket(time, node, index)

    return population



if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    time        = 0


    population[37]['transmittingPacket']    = 'Hello World'
    population[37]['transmittingPower']     = 15
    
    population[23]['RootNode']              = True

    while True:
        print('\nTime: %d\n========================'%(time))
        population  = CycleSim(time, population)
        time    = time + 1


