#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    std::cerr << "Welcome to the server!\n";
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int serverPort = 1234;
    int clientAddrSize = sizeof(clientAddr);
    char receiveBuffer[1024];
    std::string cmd;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket\n";
        WSACleanup();
        return 1;
    }

    // Set server address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address and port
    if (bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Could not bind socket\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Could not listen for incoming connections\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Accept incoming connections
    if ((clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
        std::cerr << "Could not accept incoming connection\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Close listening socket
    closesocket(listenSocket);

    // Loop to read user input and send to client
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, cmd);

        // Send command to client
        if (send(clientSocket, cmd.c_str(), cmd.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Could not send command to client\n";
            break;
        }
    }

    // Close client socket and cleanup Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
