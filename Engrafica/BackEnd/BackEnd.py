#!/usr/bin/env python3 

from flask import Flask, request, send_from_directory, redirect, url_for
import random
from flask_cors import CORS


# set the project root directory as the static folder, you can set others.
app = Flask(__name__, static_url_path='')
CORS(app)

# CORS:
# https://flask-cors.readthedocs.io/en/latest/

@app.route('/<path:path>')
def staticFiles(path):
    return send_from_directory('static', path)

@app.route('/Step1Submit', methods=['POST'])
def Step1Submit():
    return redirect('/Step2.html')

@app.route('/Step2Submit', methods=['POST'])
def Step2Submit():
    return redirect('/Step3.html')

@app.route('/Step3Submit', methods=['POST'])
def Step3Submit():
    return redirect('/Step4.html')

@app.route('/Step4Submit', methods=['POST'])
def Step4Submit():
    return redirect('/Step4.html')

@app.route('/Progress', methods=['GET'])
def Progress():
    return 'width: %d%%'%(int(random.random()*100))

if __name__ == "__main__":
    app.run(host='0.0.0.0',port=8081,ssl_context=('cert.pem', 'key.pem'))


