import socket
from service import StockService
import json
class Endpoints:
    def __init__(self, svc:StockService):
        self.svc = svc
    def execute(self,response):
        try:
            endpoint = response['endpoint']
            
        except:
            response_data = {"endpoint" : "/notfound","message": "No endpoint given","status": False}
            return json.dumps(response_data)

        if endpoint:
            if endpoint == "/changestock":
                return self.changeStock(response)
            elif endpoint == "/fetchstocks":
                return self.fetch_stocks(response)
            else:
                response_data = {"endpoint" : "/notfound","message": "endpoint not found","status": False}
                return json.dumps(response_data)

    def changeStock(self,response):
        try:
            if(response['amount']<0):
                if(self.svc.sell(response['stock'], -response['amount'])):
                    response_data = {"endpoint": "/processedchange", "status": True}
                    return json.dumps(response_data)
                response_data = {"endpoint": "/processedchange","status": False}
                return json.dumps(response_data)
            else:
                if(self.svc.buy(response['stock'], response['amount'])):
                    response_data = {"endpoint": "/processedchange","status": True}
                    return json.dumps(response_data)
                response_data = {"endpoint" : "/processedchange","status": False,"message" : "You are not allowed to perform that action at this time."}
                return json.dumps(response_data)
        except:
            response_data = {"endpoint" : "/processedchange","status": False,"message" : "error in JSON format"}
            return json.dumps(response_data)
    def fetch_stocks(self,response):
        try:
            response_data = {"endpoint" : "/processaquirestocks","status": True,"stocks" : self.svc.fetch_stocks()}
            return json.dumps(response_data)
        except:
            response_data = {"endpoint" : "/processaquirestocks","status": False,"message" : "error in JSON format"}
            return json.dumps(response_data)



        
        