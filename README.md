# Multi-Client WebSocket Server in C++

This repository demonstrates how to create a WebSocket server in C++ that can handle multiple clients simultaneously. It uses the [WebSocket++](https://github.com/zaphoyd/websocketpp) library and ASIO for asynchronous network communication.

## Features

- Accept multiple clients at once.
- Print incoming messages from clients to the console.
- Reply to clients with an echo of their message.
- Handle client connections and disconnections gracefully.
- Thread-safe management of clients using mutexes.

## Usage

1. Clone the repository:

```bash
git clone https://github.com/kamalesh-p/multi-client-websocket.git
cd multi-client-websocket
```

Compile the server (make sure you have WebSocket++ and ASIO installed):
```bash
g++ multi_client_server.cpp -o server -std=c++17 -pthread -lboost_system
```

Run the server:
```bash
./server
```

Connect clients using a WebSocket client (like a browser, Postman, or custom client). Messages sent by the client will be printed in the server console and echoed back.

Example
```
When a client sends:

Hello Server


The server output:

Client 1 received: Hello Server


And the client receives:

Echo: Hello Server
```
Notes

This is a simple demonstration for learning purposes.

For production use, proper cleanup, exception handling, and security (TLS) should be implemented.

Follow my GitHub to see how I develop my coding skills and explore more C++ networking projects!
