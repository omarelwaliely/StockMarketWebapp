from service_socket import ServiceSocket

def main():
    service_socket = ServiceSocket('localhost', 54002,"/home/omar/stocks.json", 0.01, 0.1, 1)
    service_socket.service.start_simulation()
    service_socket.connect()
main()

        
        