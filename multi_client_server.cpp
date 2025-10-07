#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <unordered_map>
#include <mutex>

typedef websocketpp::server<websocketpp::config::asio> websocket_server;
using websocketpp::connection_hdl;

class Client {
public:
    Client(websocket_server* server_ptr, connection_hdl hdl, int client_id)
        : server_ptr(server_ptr), hdl(hdl), client_id(client_id) {}

    void on_message(websocket_server::message_ptr msg) {
        std::cout << "Client " << client_id << " received: " << msg->get_payload() << std::endl;
        send("Echo: " + msg->get_payload());
    }

    void send(const std::string& msg) {
        try {
            server_ptr->send(hdl, msg, websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cerr << "Send failed for client " << client_id << ": " << e.what() << std::endl;
        }
    }

    void on_close() {
        std::cout << "Client " << client_id << " disconnected" << std::endl;
    }

    websocket_server* server_ptr;
    connection_hdl hdl;
    int client_id;
};

class MultiClientServer {
public:
    MultiClientServer() {
        server.init_asio();

        server.set_open_handler([this](connection_hdl hdl) {
            int client_id = next_id++;
            auto client = new Client(&server, hdl, client_id);

            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients[hdl] = client;
            }

            std::cout << "Client " << client_id << " connected" << std::endl;
        });

        server.set_message_handler([this](connection_hdl hdl, websocket_server::message_ptr msg) {
            Client* client = nullptr;
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                auto it = clients.find(hdl);
                if (it != clients.end()) {
                    client = it->second;
                }
            }
            if (client) {
                client->on_message(msg);
            }
        });

        server.set_close_handler([this](connection_hdl hdl) {
            Client* client = nullptr;
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                auto it = clients.find(hdl);
                if (it != clients.end()) {
                    client = it->second;
                    clients.erase(it);
                }
            }
            if (client) {
                client->on_close();
                delete client;
            }
        });
    }

    void run(uint16_t port) {
        server.set_reuse_addr(true);
        server.listen(port);
        server.start_accept();
        server.run();
    }

private:
    websocket_server server;
    std::unordered_map<connection_hdl, Client*, std::owner_less<connection_hdl>> clients;
    std::mutex clients_mutex;
    int next_id = 1;
};

int main() {
    MultiClientServer multi_client_server;
    multi_client_server.run(9002);
}
