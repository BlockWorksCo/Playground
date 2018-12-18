#!/usr/bin/env python3

#
#
#
#
#


from flask import Flask, render_template, send_from_directory, request, jsonify
import uuid
import glob
import re
import multiprocessing
import time


app = Flask(__name__)



#
# Get the address of a node ID (EUI-64)
#
def HashedNodeId(id):
    return '0123456789%s'%(id)


#
# Create a DistributionJob
#
@app.route('/API/DistributionJob', methods=['POST'])
def DistributionJob():
    targetList  = request.json['TargetList']
    blob  = request.json['BLOB']
    print(targetList)
    print(blob)

    jobId    = uuid.uuid4().hex
    for nodeId in targetList:
        nodeAddress = HashedNodeId( nodeId )
        with open('Jobs/%s_%s.Request'%(jobId,nodeAddress), 'w') as f:
            f.write(blob)
            f.close()

    response = jsonify({'JobID':jobId})
    response.status_code = 200
    return response 


#
# Get DistributionJob status and results
#
@app.route('/API/DistributionJob/<jobId>', methods=['GET'])
def GetDistributionJobStatus(jobId):
    print('result of %s'%(jobId))
    responseFiles = glob.glob('Jobs/%s_*.Response'%(jobId))
    print( responseFiles )
    
    result  = {}
    result['JobID'] = jobId
    result['Results']   = []
    for responseFile in responseFiles:
        hashedNodeId  = re.compile("_(.*).Response").findall( responseFile )[0]
        result['Results'].append( hashedNodeId )
    return jsonify( result ) 


#
# Remove a DistributionJob results
#
@app.route('/API/DistributionJob/<jobId>', methods=['DELETE'])
def RemoveDistributionJob(jobId):
    print('remove of %s'%(jobId))
    responseFiles = glob.glob('Jobs/%s_*.Response'%(jobId))
    print( responseFiles )
    
    result  = {}
    result['JobID'] = jobId
    result['Results']   = []
    for responseFile in responseFiles:
        hashedNodeId  = re.compile("_(.*).Response").findall( responseFile )[0]
        result['Results'].append( hashedNodeId )
    return jsonify( result ) 





#
def gatewayForNodeEUI( eui ):
    return eui[:8]


#
#
#
def Distributor():

    while True:
        print('tick...\n')

        # Get a list of the outstanding jobs.
        jobs    = glob.glob('Jobs/*.Request')

        # Create a set of all the Nodes that the Jobs are targetting.
        nodes    = set()
        for job in jobs:
            targetNode  = re.compile('[0-9a-fA-F]+_([0-9a-fA-F]+).Request').findall(job)[0]
            nodes.add( targetNode )
            
        # Determine the set of gateways from the set of nodes.
        gateways    = set()
        for node in nodes:
            targetGateway   = gatewayForNodeEUI( node )
            gateways.add( targetGateway )

        # Determine the Jobs for the first gateway in the set
        gatewayToProcess    = gateways.pop()

        # Find all the Jobs that target our chosen gateway
        allJobs         = ' '.join( jobs )
        jobsForGateway  = re.compile('([0-9a-fA-F]+_%s[0-9a-fA-F]+.Request)'%gatewayToProcess).findall( allJobs )

        # For each job being sent to our chosen gateway, append the data to the payload.
        payload = ''
        for job in jobsForGateway:
            payload += open( 'Jobs/'+job ).read()

        # Send the payload to the gateway
        print( payload )
        
        #
        #print( jobsForGateway )
        #print( gatewayToProcess )
        #print( gateways )
        print( nodes )

        time.sleep(1.0)





if __name__ == "__main__":

    #
    for i in range(1):
        p   = multiprocessing.Process( target=Distributor )
        p.start()

    #
    app.run()




