#!/usr/bin/env python3

#
#
#
#
#


from flask import Flask, render_template, send_from_directory, request



app = Flask(__name__)



@app.route('/API/DistributionJob', methods=['POST','GET'])
def Log():
    return '{Done}'


if __name__ == "__main__":
    app.run()



