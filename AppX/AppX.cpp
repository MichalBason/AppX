// appX.cpp
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "serverSocket.h"


using namespace std;


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")


#define DEFAULT_PORT "27015"
#define MATRIX_SIZE 256


//class serverSocket
//{
//
//private:
//    WSADATA wsaData;
//
//    SOCKET ListenSocket = INVALID_SOCKET;
//    SOCKET ClientSocket = INVALID_SOCKET;
//
//    struct addrinfo* result = NULL;
//    struct addrinfo hints;
//
//    int iSendResult;
//
//public:  
//
//    serverSocket() {
//        initializeWinsock();
//        createSocket();
//        setupListeningSocket();
//        acceptClient();
//    };
//
//    void cleanup() {
//        closesocket(ClientSocket);
//        WSACleanup();       
//    };
//
//    int initializeWinsock() {
//        int iResult;
//        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//        if (iResult != 0) {
//            cout<<"WSAStartup failed with error: "<< iResult <<endl;
//            return -1;
//        }
//
//        ZeroMemory(&hints, sizeof(hints));
//        hints.ai_family = AF_INET;
//        hints.ai_socktype = SOCK_STREAM;
//        hints.ai_protocol = IPPROTO_TCP;
//        hints.ai_flags = AI_PASSIVE;
//
//        // Resolve the server address and port
//        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//        if (iResult != 0) {
//            cout<< "getaddrinfo failed with error: "<< iResult <<endl;
//            WSACleanup();
//            return -1;
//        }
//        return 0;
//    }
//
//    int createSocket() {
//        // Create a SOCKET for the server to listen for client connections.
//        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//        if (ListenSocket == INVALID_SOCKET) {
//            cout<< "socket failed with error: "<< WSAGetLastError() << endl;
//            freeaddrinfo(result);
//            WSACleanup();
//            return -1;
//        }
//        return 0;
//    }
//
//    int setupListeningSocket() {
//        // Setup the TCP listening socket
//        int iResult;
//        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//        if (iResult == SOCKET_ERROR) {
//            cout << "bind failed with error: " << WSAGetLastError() << endl;
//            freeaddrinfo(result);
//            cleanup();
//            return -1;
//        }
//
//        freeaddrinfo(result);
//
//        iResult = listen(ListenSocket, SOMAXCONN);
//        if (iResult == SOCKET_ERROR) {
//            cout << "listen failed with error: " << WSAGetLastError() << endl;
//            cleanup();
//            return -1;
//        }
//        return 0;
//    }
//
//    int acceptClient() {
//        // Accept a client socket
//        cout << "wait for the client connect" << endl;
//        ClientSocket = accept(ListenSocket, NULL, NULL);
//        if (ClientSocket == INVALID_SOCKET) {
//            cout << "accept failed with error: " << WSAGetLastError() << endl;
//            cleanup();
//            return -1;
//        }
//        // No longer need server socket
//        closesocket(ListenSocket);
//        return 0;
//    }
//
//    int shutdownConnection() {
//        // shutdown the connection since we're done
//        int iResult;
//        iResult = shutdown(ClientSocket, SD_SEND);
//        if (iResult == SOCKET_ERROR) {
//            cout << "shutdown failed with error: " << WSAGetLastError() << endl;
//            cleanup();
//            return 1;
//        }
//        return 0;
//    }
//
//    int sendData(char* sendbuf, int size) {
//        //send sendbuf
//        iSendResult = send(ClientSocket, sendbuf, size, 0);
//        if (iSendResult == SOCKET_ERROR) {
//            cout << "send failed with error: " << WSAGetLastError() << endl;
//            cleanup();
//            return -1;
//        }
//        cout << "Bytes sent: " << size << endl;
//        return iSendResult;
//    }
//
//};


class FullImageMatrix {

private:
    short* matrix;
    //int sumMetrix;

public:
    FullImageMatrix() {
        matrix = new short[MATRIX_SIZE * MATRIX_SIZE];
        //sumMetrix = 0;
        fillRandomMatrix();
    }

    ~FullImageMatrix() {
        delete[] matrix;
    }

    void fillRandomMatrix() {
        int randNum;
        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; ++i) {
            randNum = rand() % 257;
            matrix[i] = randNum;
        }
    }

    int sendMetrix(serverSocket* sock) {
        //send the matrix
        int size = MATRIX_SIZE * MATRIX_SIZE * sizeof(short);
        int iResult = sock->sendData((char*)matrix, size);
        if (iResult == -1) {
            return -1;
        }
        return iResult;
    }

    };



int __cdecl main(int argc, char** argv)
{
        srand((unsigned)time(NULL));
        
        serverSocket* servSock = new serverSocket(DEFAULT_PORT);

        FullImageMatrix* mat;

        int iResult;  
        while (true) {
            mat = new FullImageMatrix();
            iResult = mat->sendMetrix(servSock);
            delete mat ;
            if (iResult == -1)
                break;

            Sleep(1000); // Sleep for 1 second
        }
        servSock->shutdownConnection();
        delete servSock;
        return 0;
    }


