from service_socket import ServiceSocket
import os

def main():
    current_directory = os.getcwd()
    file_path = os.path.join(current_directory, 'stocks.json')
    service_socket = ServiceSocket('localhost', 54002,file_path, 0.01, 0.1, 1)
    service_socket.service.start_simulation()
    service_socket.connect()
main()

        
        