#!/usr/bin/env python3

#
#
#
#
#


from flask import Flask, render_template, send_from_directory, request, jsonify
import json


app = Flask(__name__)



@app.route('/API/DistributionJob', methods=['POST','GET'])
def Log():
    print(request)
    response = jsonify({'123123':'1231'})
    response.status_code = 200
    return response 


if __name__ == "__main__":
    app.run()



