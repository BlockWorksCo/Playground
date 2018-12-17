#!/usr/bin/env python3



from flask import Flask, render_template, send_from_directory, request
import syslog



app = Flask(__name__)

@app.route('/stuff/<path:path>')
def send_js(path):
    return send_from_directory('stuff', path)


@app.route("/Index")
def Index():
    return render_template('Index.html')



@app.route('/Log', methods=['POST'])
def Log():
    print( '%s'%(request.form['message']) )
    syslog.syslog(syslog.LOG_INFO, request.form['message'])

    return 'Done'

@app.route("/")
def main():
    return "Welcome!"


if __name__ == "__main__":
    syslog.openlog(logoption=syslog.LOG_PID, ident='SUS')
    app.run()



