


import select
import socket


class TCPTransport:
    """
    """

    def __init__(self):
        """
        """
        pass


    def Run(self):
        """
        """
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = ('localhost', 9999)
        sock.bind(server_address)
        sock.listen(1)

        while True:
            self.connection, client_address = sock.accept()

            while True:
                r, w, e = select.select((self.connection,), (), (), 1.0)
                if r:
                    data = self.connection.recv(1024)
                    numberOfBytes   = len(data)

                    if numberOfBytes == 0:
                        self.connection.close()
                        break

                    elif numberOfBytes > 0:
                        for byte in data:
                            self.upperLevel.ByteReceived(byte)


    def Link(self, upper, lower):
        """
        """
        self.upperLevel = upper
        self.lowerLevel = lower


    def OutputBytes(self, data):
        """
        """
        self.connection.send(data)




