

import SimpleHTTPServer
import SocketServer
import time
import sys



PORT = int(sys.argv[1])


counter = 0
counter2 = 0

class SimpleServer(SimpleHTTPServer.SimpleHTTPRequestHandler):


    def __init__(self, *args):
        SimpleHTTPServer.SimpleHTTPRequestHandler.__init__(self, *args)

    def do_GET(self):

        if self.path.endswith('/Ping'):
            global counter
            response    = '<h1>Counter %d</h1>'%(counter)
            counter     = counter + 1

            time.sleep(2.0)
            self.send_response(200)
            self.send_header("Access-Control-Allow-Origin", "*")
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(response))
            self.end_headers()
            self.wfile.write(response)

        elif self.path.endswith('/Pong'):
            global counter2
            response    = '<h1>Counter %d</h1>'%(counter2)
            counter2     = counter2 + 1

            self.send_response(200)
            self.send_header("Access-Control-Allow-Origin", "*")
            self.send_header("Content-type", "text/html")
            self.send_header("Content-length", len(response))
            self.end_headers()
            self.wfile.write(response)

        else:
            return SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)





httpd = SocketServer.TCPServer(("", PORT), SimpleServer)

print "serving at port", PORT
httpd.serve_forever()
