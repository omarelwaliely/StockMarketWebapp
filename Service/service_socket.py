import socket

class ServiceSocket:
    def __init__(self, server_ip, server_port):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((server_ip, server_port))  
    def connect(self):
        while True:
            message = input("Enter message to send: ")
            if message == 'quit':
                break
            self.client_socket.send(message.encode())
            response = self.client_socket.recv(4096)
            print("Received:", response.decode())
        self.client_socket.close()
svc = ServiceSocket('localhost',54001)
svc.connect()