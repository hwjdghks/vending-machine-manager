#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 6667
#define MAX_CLIENTS 10

void handle_receive(int client_sock) {
    char buffer[1024] = {0};
    while (true) {
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::yield();
                continue;
            } else {
                std::cerr << "recv error: " << strerror(errno) << std::endl;
                break;
            }
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;
    }
    close(client_sock);
}

void handle_send(int client_sock) {
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (send(client_sock, message.c_str(), message.size(), 0) < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::yield();
                continue;
            } else {
                std::cerr << "send error: " << strerror(errno) << std::endl;
                break;
            }
        }
    }
    close(client_sock);
}

void handle_client(int client_sock) {
    std::thread recv_thread(handle_receive, client_sock);
    std::thread send_thread(handle_send, client_sock);

    recv_thread.join();
    send_thread.join();
}

void server() {
    int server_sock, client_sock;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created" << std::endl;

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Bind successful" << std::endl;

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Listening on port " << PORT << std::endl;

    // Set the server socket to non-blocking mode
    fcntl(server_sock, F_SETFL, O_NONBLOCK);

    std::vector<std::thread> client_threads;

    while (true) {
        if ((client_sock = accept(server_sock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::yield();
                continue;
            } else {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
        }
        std::cout << "Accepted a connection. fd: " << client_sock << std::endl;

        // Set the client socket to non-blocking mode
        fcntl(client_sock, F_SETFL, O_NONBLOCK);

        client_threads.emplace_back(handle_client, client_sock);
    }

    for (auto& thread : client_threads) {
        thread.join();
    }

    close(server_sock);
}

int main() {
    server();
    return 0;
}
