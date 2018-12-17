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




if __name__ == "__main__":
    app.run()



