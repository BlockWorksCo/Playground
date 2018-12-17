#!/usr/bin/env python3

#
#
#
#
#


from flask import Flask, render_template, send_from_directory, request, jsonify
import tempfile


app = Flask(__name__)



def NodeAddressFromID(id):
    return '2a03:0500:0001::0200::%s'%(id)


@app.route('/API/DistributionJob', methods=['POST','GET'])
def DistributionJob():
    targetList  = request.json['TargetList']
    blob  = request.json['BLOB']
    print(targetList)
    print(blob)

    tmp = tempfile.NamedTemporaryFile(delete=False,dir='Jobs',suffix='.Status')
    for nodeId in targetList:
        nodeAddress = NodeAddressFromID( nodeId )
        with open(tmp.name+'_%s.Request'%(nodeAddress), 'w') as f:
            f.write(str(targetList))
            f.write(blob)
        f.close()

    response = jsonify({'JobID':tmp.name})
    response.status_code = 200
    return response 


if __name__ == "__main__":
    app.run()



