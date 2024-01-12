import React, { useEffect, useState } from 'react';
import useSocket from '../services/useSocket';
import {
  Button,
  FormControl,
  InputLabel,
  MenuItem,
  Select,
  Typography,
  Paper,
  CssBaseline,
  TextField,
} from '@mui/material';

const StockPage = () => {
  const [amount, setAmount] = useState(0);
  const [action, setAction] = useState('');
  const [selectedStock, setSelectedStock] = useState('');
  const socket = useSocket('ws://localhost:3001');
  const [stocks, setStocks] = useState({});
  const [fetch, setFetch] = useState(false);
  const accessEndpointChange = async(stockAmount,curselectedStock) => {
      try {
        if (socket.readyState === WebSocket.OPEN) {
          const body = {
            amount: stockAmount,
            stock: curselectedStock,
            email: "test@gmail.com" //placeholder

          };
          const message = {
            endpoint: '/changestock',
            header: {token:'token'},
            body: body,
          };
  
          socket.send(JSON.stringify(message));

          await new Promise((resolve) => {
            socket.addEventListener('message', (event) => {
              const data = JSON.parse(event.data);
              if (data.code === 200) {
                console.log('Success');
                setFetch(!fetch)
              } else {
                console.error('Change failed:', data.body.message);
              }
              resolve();
            });
          });
        } else {
          console.error('WebSocket connection is not open');
        }
      } catch (error) {
        console.error('Error during change request:', error);
      }
    };
    const accessEndpointFetch = async() => {
      try {
        if (socket.readyState === WebSocket.OPEN) {
          const body = {
            email: "test@gmail.com" //placeholder

          };
          const message = {
            endpoint: '/aquirestocks',
            header: {token:'token'},
            body: body,
          };
  
          socket.send(JSON.stringify(message));

          await new Promise((resolve) => {
            socket.addEventListener('message', (event) => {
              const data = JSON.parse(event.data);
              if (data.code === 200) {
                console.log('Success');
                console.log(data.body);
                setStocks(data.body.stocks);
              } else {
                console.error('Aquire failed:', data.body.message);
              }
              resolve();
            });
          });
        } else {
          console.error('WebSocket connection is not open');
        }
      } catch (error) {
        console.error('Error during Aquire request:', error);
      }
    };

  const handleBuySell = () => {
    if (action === 'buy') {
      accessEndpointChange(amount, selectedStock);
    } else if (action === 'sell') {
      accessEndpointChange(-amount, selectedStock);
    }
  };
  useEffect(() => {
    const fetchData = async () => {
      try {
        await new Promise((resolve) => {
          if (socket && socket.readyState === WebSocket.OPEN) {
            resolve();
          } else {
            socket.addEventListener('open', resolve);
          }
        });
        await accessEndpointFetch();
        console.log("Data fetched successfully");
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };
  
    fetchData();
  }, [socket,fetch]);

  return (
    <div>
      <CssBaseline />
      <Paper
        elevation={3}
        style={{
          padding: '20px',
          maxWidth: '400px',
          margin: 'auto',
          marginTop: '50px',
        }}
      >
        <Typography variant="h4" align="center" gutterBottom>
          Stock Buy/Sell Page
        </Typography>

        <FormControl fullWidth>
          <InputLabel>Action</InputLabel>
          <Select
            value={action}
            label="Action"
            onChange={(e) => setAction(e.target.value)}
          >
            <MenuItem value="buy">Buy</MenuItem>
            <MenuItem value="sell">Sell</MenuItem>
          </Select>
        </FormControl>

        {action && (
          <FormControl fullWidth style={{ marginTop: '10px' }}>
            <InputLabel>Select Stock</InputLabel>
            <Select
              value={selectedStock}
              label="Select Stock"
              onChange={(e) => setSelectedStock(e.target.value)}
            >
              {stocks &&
              Object.keys(stocks).map((stockSymbol) => (
                <MenuItem key={stockSymbol} value={stockSymbol}>
                  {stocks[stockSymbol].name}
                </MenuItem>
              ))}
            </Select>
          </FormControl>
        )}

        <FormControl fullWidth style={{ marginTop: '10px' }}>
          <TextField
            label="Amount"
            type="number"
            value={amount}
            onChange={(e) => {
              const inputAmount = parseInt(e.target.value, 10);
              if (!isNaN(inputAmount) && inputAmount >= 0) {
                setAmount(inputAmount);
              }
            }}
          />
        </FormControl>
      

        <Typography variant="body1" align="center">
          Selected Action: {action}
        </Typography>
        <Typography variant="body1" align="center">
          Selected Stock: {selectedStock}
        </Typography>
        <Typography variant="body1" align="center">
          Amount: {amount}
        </Typography>

        <Button
          fullWidth
          variant="contained"
          color="primary"
          onClick={handleBuySell}
          style={{ marginTop: '20px' }}
        >
          {action === 'buy' ? 'Buy' : 'Sell'}
        </Button>
      </Paper>
    </div>
  );
};

export default StockPage;
