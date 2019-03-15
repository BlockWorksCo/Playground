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
import base64
import ctypes
import binascii
import socket
import os
import sqlite3




#
#
#
db = sqlite3.connect('ACM.sqlite3') 
cursor  = db.cursor()

customers_sql = """
CREATE TABLE outgoing (
    endpoint text PRIMARY KEY,
    first_name text NOT NULL,
    last_name text NOT NULL)"""
cur.execute(customers_sql)


#
#
#
app = Flask(__name__)




#
#
#
@app.route('/outgoingJobs', methods=['POST'])
def CreateOutgoingJob():

    print(request.json)

    # Get the metadata for the outgoingJob.
    endpoint    = request.json['Endpoint']
    targetList  = request.json['TargetList']
    blob        = request.json['BLOB']

    # Perform the operation.
    print(endpoint)
    print(targetList)
    print(blob)

    # Form the response.
    jobId    = uuid.uuid4().hex
    for nodeId in targetList:
        print(nodeId)

    response = jsonify({'JobID':jobId})
    response.status_code = 200
    return response 



#
#
#
@app.route('/incomming', methods=['GET'])
def ReturnIncomingData():

    print(request.json)

    # Get the metadata for the outgoingJob.
    endpoint    = request.json['Endpoint']
    targetList  = request.json['TargetList']
    blob        = request.json['BLOB']

    # Perform the operation.
    print(endpoint)
    print(targetList)
    print(blob)

    # Form the response.
    jobId    = uuid.uuid4().hex
    for nodeId in targetList:
        print(nodeId)

    response = jsonify({'JobID':jobId})
    response.status_code = 200
    return response 




if __name__ == "__main__":

    #
    app.run()




