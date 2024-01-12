const express = require('express');
const http = require('http');
const net = require('net');
const WebSocket = require('ws');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
const tcpServer = net.createConnection({ host: 'localhost', port: '54003' });

wss.on('connection', (ws) => {
  console.log('WebSocket connection established');
  ws.on('message', (message) => {
    console.log('Received message from client:', message);
    tcpServer.write(message);
  });
  tcpServer.on('data', (data) => {
    ws.send(data.toString());
  });
});

server.listen(3001, () => {
  console.log('Bridge server listening on port 3001');
});
