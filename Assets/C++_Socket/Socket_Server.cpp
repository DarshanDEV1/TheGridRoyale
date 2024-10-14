#include <iostream>
#include <winsock2.h>  // Winsock library for sockets
#include <ws2tcpip.h>  // Extra header for IP address structures

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define PORT 8080

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // 2. Create a socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. Bind the socket to an IP address and port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // 4. Listen for incoming connections
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Waiting for connections on port " << PORT << "..." << std::endl;

    // 5. Accept a client connection
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Client connected!" << std::endl;

    // 6. Send and receive data
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        std::cout << "Received: " << buffer << std::endl;
        const char* hello = "Hello from server";
        send(client_socket, hello, strlen(hello), 0);
    }

    // 7. Close the sockets
    closesocket(client_socket);
    closesocket(server_socket);

    // 8. Cleanup Winsock
    WSACleanup();

    return 0;
}
