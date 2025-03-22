#include <iostream>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
    int server_sock, client_sock;
    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    char buffer[1024] = {0};
    socklen_t opt = sizeof(rem_addr);

    server_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (server_sock < 0) {
        std::cerr << "Bluetooth socket creation failed\n";
        return -1;
    }

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;

    if (bind(server_sock, (struct sockaddr*)&loc_addr, sizeof(loc_addr)) < 0) {
        std::cerr << "Bluetooth bind failed\n";
        return -1;
    }

    listen(server_sock, 1);
    std::cout << "Waiting for Bluetooth connection...\n";

    client_sock = accept(server_sock, (struct sockaddr*)&rem_addr, &opt);
    if (client_sock < 0) {
        std::cerr << "Accept failed\n";
        return -1;
    }

    char client_address[18];
    ba2str(&rem_addr.rc_bdaddr, client_address);
    std::cout << "Connected to " << client_address << "\n";

    while (true) {
        int bytes_read = read(client_sock, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
