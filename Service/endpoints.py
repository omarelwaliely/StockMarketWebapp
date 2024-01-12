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
            response_data = {"message": "No endpoint given","status": False}
            return json.dumps(response_data)
            

        if endpoint:
            if endpoint == "/changestock":
                return self.changeStock(response)
            else:
                response_data = {"message": "endpoint not found","status": False}
                return json.dumps(response_data)
    def changeStock(self,response):
        try:
            if(response['amount']<0):
                if(self.svc.sell(response['stock'], -response['amount'])):
                    response_data = {"status": True}
                    return json.dumps(response_data)
                response_data = {"status": False}
                return json.dumps(response_data)
            else:
                if(self.svc.buy(response['stock'], response['amount'])):
                    response_data = {"status": True}
                    return json.dumps(response_data)
                response_data = {"status": False,"message" : "You are not allowed to perform that action at this time."}
                return json.dumps(response_data)
        except:
            response_data = {"status": False,"message" : "error in JSON format"}
            return json.dumps(response_data)



        
        