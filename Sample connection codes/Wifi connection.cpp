#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        return -1;
    }

    std::cout << "Waiting for connection...\n";
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed\n";
        return -1;
    }

    std::cout << "Connected!\n";

    while (true) {
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread > 0) {
            buffer[valread] = '\0';  // Null-terminate received data
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
