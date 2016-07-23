import SimpleHTTPServer
import SocketServer

PORT = 8080


counter = 0


class SimpleServer(SimpleHTTPServer.SimpleHTTPRequestHandler):


    def __init__(self, *args):
        SimpleHTTPServer.SimpleHTTPRequestHandler.__init__(self, *args)

    def do_GET(self):
        global counter
        response    = '<h1>Counter %d</h1>'%(counter)
        counter     = counter + 1

        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-length", len(response))
        self.end_headers()
        self.wfile.write(response)





httpd = SocketServer.TCPServer(("", PORT), SimpleServer)

print "serving at port", PORT
httpd.serve_forever()
