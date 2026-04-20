const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.json());

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.post('/data', (req, res) => {
    const { value, distance } = req.body;
    const timestamp = new Date().toLocaleTimeString();
    
    // Broadcast data silently to the frontend
    io.emit('sensor-update', { 
        value: value, 
        distance: distance, 
        time: timestamp 
    });
    
    res.sendStatus(200);
});

const PORT = 3000;
server.listen(PORT, '0.0.0.0', () => {
    console.log(`Server started! Website: http://localhost:${PORT}`);
});