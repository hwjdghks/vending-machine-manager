#include <iostream>
#include <thread>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <mutex>

std::vector<std::thread> threads;
std::mutex mtx;

void handle_client_read(int client_socket) {
    char buffer[512];
    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }
        std::cout << "Received: " << std::string(buffer, bytes_received) << std::endl;
    }
    close(client_socket);
}

void handle_client_write(int client_socket) {
    std::string message;
    while (std::getline(std::cin, message)) {
        send(client_socket, message.c_str(), message.size(), 0);
    }
    close(client_socket);
}

void handle_client(int client_socket) {
    std::thread read_thread(handle_client_read, client_socket);
    std::thread write_thread(handle_client_write, client_socket);

    read_thread.join();
    write_thread.join();
}

void server(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket\n";
        close(server_socket);
        return;
    }

    if (listen(server_socket, 10) == -1) {
        std::cerr << "Failed to listen on socket\n";
        close(server_socket);
        return;
    }

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        std::lock_guard<std::mutex> lock(mtx);
        threads.emplace_back(handle_client, client_socket);
    }

    close(server_socket);

    // Join all threads before exiting the program
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int main() {
    server(6667);
    return 0;
}