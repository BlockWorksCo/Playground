#!/usr/bin/env python3

#
#
#
#
#


from flask import Flask, render_template, send_from_directory, request, jsonify
import uuid


app = Flask(__name__)



#
# Get the address of a node ID (EUI-64)
#
def NodeAddressFromID(id):
    return '2a03:0500:0001::0200::%s'%(id)


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
        nodeAddress = NodeAddressFromID( nodeId )
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
    return jsonify({'JobID':jobId,'Results':['a','b']}) 




if __name__ == "__main__":
    app.run()



