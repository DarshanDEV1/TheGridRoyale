#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. Define server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Change this line: Use inet_addr() instead of inet_pton()
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid address." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // 4. Connect to the server
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    std::cout << "Connected to server!" << std::endl;

    // 5. Send data to server
    const char* hello = "Hello from client";
    send(client_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent." << std::endl;

    // 6. Receive data from server
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        std::cout << "Server: " << buffer << std::endl;
    }

    // 7. Close the socket
    closesocket(client_socket);

    // 8. Cleanup Winsock
    WSACleanup();

    return 0;
}
