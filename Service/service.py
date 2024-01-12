import json
import random
import threading
import time
class StockService:
    def __init__(self, stocks_file, change_percent, random_change_amount, wait_time): 
        random.seed()
        self.stocks_file = stocks_file #making this a local variable so we can write to file later
        print(self.stocks_file)
        self.stocks_data = self.read_stocks() #saving the data so there is a local instance this allows for o(1) search time since python dicts are hashmaps
        self.change_percent = change_percent #this is the change percentage (Y)
        self.random_change_amount = random_change_amount #this is the change amount bounds (Z)
        self.wait_time = wait_time #how long to wait (N)
        self.stop_event = threading.Event() #the socket and changing json need different threads

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
            if self.stocks_data[stock_code]["amount"] <0:
                self.stocks_data[stock_code]["amount"] += amount
                return False
            self.stocks_data[stock_code]["price"] *= ((1-self.change_percent) *amount) #update the price by y%
            self.write_stocks(self.stocks_data) #write back to file for future requests
            return True
        else:
            print(f"{stock_code} is not on file.")
            return False
    
    def sell(self, stock_code, amount):
        if stock_code in self.stocks_data:  #make sure the stocks code exists
            self.stocks_data[stock_code]["amount"] += amount #add the stocks that are sold
            self.stocks_data[stock_code]["price"] *= ((1-self.change_percent) *amount) #update the price by 1-y%
            self.write_stocks(self.stocks_data) #write back to file for future requests
            return True
        else:
            print(f"{stock_code} is not on file.")
            return False
    def fetch_stocks(self):
        return self.stocks_data
    def random_change(self):
        while not self.stop_event.is_set():
            for _ , data in self.stocks_data.items():
                change = random.uniform(-self.random_change_amount, self.random_change_amount+1) #random number between and including -z -> z
                data["price"] += change #change by that amount
            self.write_stocks(self.stocks_data) #write back to file for future requests
            time.sleep(self.wait_time)
    def start_simulation(self):
        thread = threading.Thread(target=self.random_change) #starting simulation of thread that will randomly change stock
        thread.start()

    def stop_simulation(self):
        self.stop_event.set()

