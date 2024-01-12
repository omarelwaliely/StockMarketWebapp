import socket
import json
from endpoints import Endpoints
from service import StockService

class ServiceSocket:
    def __init__(self, server_ip, server_port, stocks_file, change_percent, random_change_amount, wait_time):
        self.service = StockService(stocks_file,change_percent, random_change_amount, wait_time) #initilizing service
        self.endpoint_manager = Endpoints(self.service) #creating the manager for deciding which endpoint to do
        self.server_ip = server_ip 
        self.server_port = server_port
    def connect(self): #connecting to specified port and waiting for next steps between server and service
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((self.server_ip, self.server_port))  
        print("Connected to server.")
        while True:
            response = self.client_socket.recv(4096)
            jsonResponse = ""
            try:
                jsonResponse = json.loads(response.decode('utf-8'))
                body_json = jsonResponse['body']
                message = self.endpoint_manager.execute(body_json)
                self.client_socket.send(message.encode())
                print(f"Sending message: {message}")
            except json.JSONDecodeError as e:
                print(f"ERROR PARSING JSON: {e}")
        self.client_socket.close()
        