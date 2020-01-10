#!/usr/bin/env python





import random
import math
import binascii
import collections
import sys


populationSize          = 100
xScale                  = 9.0
yScale                  = 9.0
receiverSensitivity     = 9
maxHopCount             = 10



def InitPopulation():
    random.seed()
    population  = []
    for i in range(populationSize):
        population.append({
                            'x':random.random()*xScale,
                            'y':random.random()*yScale,
                            'receivedData':'', 
                            'receivedPower':0.0, 
                            'inFlightPackets':[], 
                            'RootNode':False,
                            'outputQueue':collections.deque(),
                            'hopCount':maxHopCount,
                          }) 

    return population


header  = \
"""
<svg width="110%%" height="auto" viewBox="0 0 %d %d" xmlns="http://www.w3.org/2000/svg">
  <style>
    .loliteNode { fill: black }
    .hiliteNode { fill: red }
    .timeLabel { font: italic 0.3pt sans-serif; fill: black }
    .nodeId { font: italic 0.1pt sans-serif; fill: red }
    .txPacket { font: italic 0.1pt sans-serif; fill: blue }
  </style>

    <xxrect x="0" y="0" width="10" height="10" style="stroke: #000000; fill:none;"/>
"""%(xScale,yScale)

footer  = \
"""
</svg>
"""

outFile = open('run.svg', "wt+")

def ShowHeader():
    outFile.write(header)


def ShowFooter():
    outFile.write(footer)


def ShowFrame(population, time):
    """
    """
    outFile.write('\n<svg>\n')
    outFile.write('<text x="0.5" y="0.5" class="timeLabel">Time:%d</text>\n'%(time))
    for index,node in enumerate(population):
        if index==37 or index == 23:
            outFile.write('<circle class="hiliteNode" cx="%f" cy="%f" r="0.1"/>\n'%(node['x']+0.1,node['y']-0.1))
        else:
            outFile.write('<circle class="loliteNode" cx="%f" cy="%f" r="0.1"/>\n'%(node['x']+0.1,node['y']-0.1))
        outFile.write('<text x="%f" y="%f" class="nodeId">%d</text>\n'%(node['x']+0.1,node['y']+0.1,index))
        if node.get('transmittingPacket') != None:
            outFile.write('<text x="%f" y="%f" class="txPacket">%s</text>\n'%(node['x'],node['y'],node['transmittingPacket']))

    outFile.write('<set id="start" attributeName="opacity" begin="0s" to="0" />')
    outFile.write('<set attributeName="opacity" begin="%ds" to="1" />'%(time+1))
    outFile.write('<set attributeName="opacity" begin="%ds" to="0" />'%(time+2))

    outFile.write('</svg>\n')


def ProcessPacket(time, node, nodeIndex):

    # Add new packet to in-flight packets. If the hash is not in the history-list
    if node['receivedData'] != '':

        # If this is an ACK-packet, mark the packet as ACKed.
        if node['receivedData'][:4] == 'ACK:':

            hopCount = int(node['receivedData'].split(':')[1])
            ackHash = int(node['receivedData'].split(':')[2])
            print('node %d received ACK for %d with hopCount %d'%(nodeIndex,ackHash,hopCount))

            ackForIndex = -1
            for index,packet in enumerate(node['inFlightPackets']):

                if binascii.crc32(packet['packet']) == ackHash:
                    ackForIndex = index
                    #print('node %d found ACKed-packet in in-flight packets, marking it as ACKed.'%(nodeIndex))

                    # This is an ACK to a packet we forwarded (and not already ACK-forwarded), so forward the ACK also.
                    # Make sure we increment the hopCount for this packet to inform all other nodes of their position.
                    if packet.get('ackSeenAtTime') == None:
                        print('node %d forwarding ACK [%s]'%(nodeIndex,node['receivedData']))
                        node['outputQueue'].append( 'ACK:%d:%s:'%(hopCount+1,str(binascii.crc32(node['receivedData']))) );

                        packet['ackSeenAtTime']     = time
                break

            if ackForIndex == -1:   
                print("node %d: ACK received but no originating message [%s]"%(nodeIndex, node['receivedData']))

            # Have we received the ACK we've been waiting for?
            if node.get('messageWaitingForACK') != None:
                if ackHash == node['messageWaitingForACK']:
                    print('node %d: packet received Ok! [%d]'%(nodeIndex, ackHash))
                    del node['messageWaitingForACK']

        else:
            # *NOT* and ACK packet.

            # If this packet has not been seen before, add it to the in-flight packets
            thisHash        = binascii.crc32(node['receivedData'])
            duplicatePacket = False
            for index,packet in enumerate(node['inFlightPackets']):
                if binascii.crc32(packet['packet']) == thisHash:
                    duplicatePacket = True

            if duplicatePacket == False:
            
                # This packet has not been seen before, add it to the in-flight packets as non-ACKed.

                # If this node is a root, generate an ACK packet for the received packet and mark it as ACKed.
                if node['RootNode'] == True:

                    node['inFlightPackets'].append( {'packet':node['receivedData'],'time':time,'forwarded':False} )
                    print('node %d generating ACK for [%s]'%(nodeIndex,node['receivedData']))
                    node['outputQueue'].append( 'ACK:%d:%s:'%(0,str(binascii.crc32(node['receivedData']))) );

                else:

                    #print('node %d storing packet for deduplication during transmission [%s]'%(nodeIndex,node['receivedData']))
                    node['inFlightPackets'].append( {'packet':node['receivedData'],'time':time,'forwarded':False} )

            #else:
                #print('node %d dropping [%s] because already seen'%(nodeIndex,node['receivedData']))
            

    # Check all in-flight packets for ready-to-transmit? transmit one and remove it from
    # the in-flight list.
    inFlightPackets = node['inFlightPackets']
    for index,packet in enumerate(inFlightPackets):
    
        packetAge   = time - packet['time']

        # The delay before forwarding should be related to the hopCount of the forwarding node so farther-out nodes
        # are less likely to forward.
        ageBeforeForwarding = node['hopCount']+2+(random.random()*2)

        if packetAge > ageBeforeForwarding and packet.get('ackSeenAtTime') == None and packet['forwarded'] == False:
            print('node %d forwarding [%s] because age is >%d and no ACK seen for it...'%(nodeIndex,packet['packet'],packetAge))
            node['outputQueue'].append( packet['packet'] );

            packet['forwarded']  = True
            break


    # If there are packets in the output-queue, transmit them...
    if len(node['outputQueue']) > 0:
        node['transmittingPacket']  = node['outputQueue'].pop()
        node['transmittingPower']   = 15


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
                    receivedPower           = fromNode['transmittingPower'] / (distanceBetweenNodes*distanceBetweenNodes)
                    if receivedPower > receiverSensitivity and receivedPower > toNode['receivedPower']:
                        toNode['receivedData']    = fromNode['transmittingPacket']
                        toNode['receivedPower']   = receivedPower
                        toNode['receivedTime']    = time
                        #print(toNode)

            # Mark this packet as having been forwarded (transmitted).
            node['inFlightPackets'].append( {'packet':fromNode['transmittingPacket'],'time':time,'forwarded':True} )
            

    # Clear the transmittedData.
    for node in population:
        if node.get('transmittingPacket') != None:
            del node['transmittingPacket']

    # Process the received packets.
    newPopulation   = []
    for index,node in enumerate(population):
        newPopulation.append( ProcessPacket(time, node, index) )

    # Produce the pretty pics.
    ShowFrame(newPopulation, time)

    return newPopulation



if __name__ == '__main__':

    print('LoRaMesh sim')

    population  = InitPopulation()
    time        = 0


    population[37]['transmittingPacket']    = 'Hello World'
    population[37]['transmittingPower']     = 15
    population[37]['messageWaitingForACK']  = binascii.crc32( population[37]['transmittingPacket'] )
    
    population[23]['RootNode']              = True

    ShowHeader()
    try:
        while True:
            print('\nTime: %d\n========================'%(time))
            population  = CycleSim(time, population)
            time    = time + 1
            if time>200:
                ShowFooter()
                break
    except KeyboardInterrupt:
        print "Bye"
        ShowFooter()
        sys.exit()


