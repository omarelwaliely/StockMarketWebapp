import json
import random

class StockService:
    def __init__(self, stocks_file, change_percent, random_change_amount):
        random.seed()
        self.stocks_file = stocks_file #making this a local variable so we can write to file later
        self.stocks_data = self.read_stocks() #saving the data so there is a local instance this allows for o(1) search time since python dicts are hashmaps
        self.change_percent = change_percent #this is the change percentage (Y)
        self.random_change_amount = random_change_amount #this is the change amount bounds (Z)

    def read_stocks(self):
        try:
            #we open the file and return the json data
            with open(self.stocks_file, 'r') as file:
                stocks_data = json.load(file)
            return stocks_data
        except FileNotFoundError:
            #the file doesnt exist print it and close
            print("File not found!")
            return 

    def write_stocks(self, data):
        #the file will either exist or be created and we dump the data inside
        with open(self.stocks_file, 'w') as file:
            json.dump(data, file)

    def buy(self, stock_code, amount):
        if stock_code in self.stocks_data: #make sure the stocks code exists
            self.stocks_data[stock_code]["amount"] -= amount #subtract the stocks that are bought
            self.stocks_data[stock_code]["price"] *= ((1-self.change_percent) *amount) #update the price by y%
        self.write_stocks(self.stocks_data) #write back to file for future requests
    
    def sell(self, stock_code, amount):
        if stock_code in  self.stocks_data:  #make sure the stocks code exists
            self.stocks_data[stock_code]["amount"] += amount #add the stocks that are sold
            self.stocks_data[stock_code]["price"] *= ((1-self.change_percent) *amount) #update the price by 1-y%
        self.write_stocks(self.stocks_data) #write back to file for future requests

    def random_change(self):
        for _ , data in self.stocks_data.items():
            change = random.uniform(-self.random_change_amount, self.random_change_amount+1) #random number between and including -z -> z
            data["price"] += change #change by that amount
        self.write_stocks(self.stocks_data) #write back to file for future requests

service = StockService("stocks.json", 0.01, 0.1)
# service.buy("ABC",1)
# service.sell("AAB",4)
service.random_change()

