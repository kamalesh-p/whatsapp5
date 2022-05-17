#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <iostream> 
//#include <sys/locking.h>

using namespace std;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



/**********
    
    kamalesh

**********/ 

// Functions
const int emoji_len = 3;

void replaceChar(char s[DEFAULT_BUFLEN], char oldchar, char newchar);
void replaceChar(char newstring[DEFAULT_BUFLEN], char oldstring[DEFAULT_BUFLEN], char oldchar, char newchar);
void gettime(char currenttime[50]);
void formatFromMessage(char storemessage[DEFAULT_BUFLEN], char mesg[DEFAULT_BUFLEN], char id[25], char to[15], char sent[50], char receive[50], char read[50], char emoji[50], char note[50]);
void formatToMessage(char storemessage[DEFAULT_BUFLEN], char mesg[DEFAULT_BUFLEN], char id[25], char from[15], char emoji[50], char note[50]);
int stoi(char s[15]);
void split(char word[][30], char sentence[DEFAULT_BUFLEN], int* num = new (int));
int split(char word[15][DEFAULT_BUFLEN], char buff[DEFAULT_BUFLEN * 15]);
void stringcpy(char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN]);
void analyse(char buff[DEFAULT_BUFLEN], char word0[DEFAULT_BUFLEN], char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN], int* num);
void analyse(char buff[DEFAULT_BUFLEN], char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN], int* num);
int process_output(SOCKET localClientSocket, char name[15], int clientnumber, int num, char word1[DEFAULT_BUFLEN], char word2[DEFAULT_BUFLEN]);
void encode(char s[DEFAULT_BUFLEN * 5]);
void decode(char s[DEFAULT_BUFLEN * 5]);

class threadreceive {
public:
    SOCKET localClientSocket;
    int ClientNumber;
    char name[15];
    threadreceive(SOCKET localClientSocket, int ClientNumber, char name[15]) {
        this->localClientSocket = localClientSocket;
        this->ClientNumber = ClientNumber;
        strcpy_s(this->name, name);
    }
    void operator()(int num);
};

class Socket {
private:
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    const char* sendbuf;

    /**********

    kamalesh

    **********/
public:
    int CountClientNumber = 0;
    Socket() {
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            return;
        }
        printf("Server start\n");
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET; // hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            return;
        }

        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        freeaddrinfo(result);
        //SOMAXCONN
        iResult = listen(ListenSocket, 1);
        //cout << "iresult" << iResult << endl;
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }
    }
    void accept_client();
    int receivee(SOCKET localClientSocket, int ClientNumber, char ReceiveName[15]) {
        iResult = recv(localClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            recvbuf[iResult] = '\0';
            char word1[DEFAULT_BUFLEN], word2[DEFAULT_BUFLEN];
            int num;

            // analyse users code
            analyse(recvbuf, word1, word2, &num);
            cout << "from " << ReceiveName << ": " << recvbuf << endl;
            // check if signed in user
            if (num != 2 && num != 3 && num != 18 && !strlen(ReceiveName) && ReceiveName[0] == '\0') {
                char message[] = "#Login";
                char tempname[15];
                sprintf_s(tempname, "Client-%d\0", CountClientNumber);
                return sendd(localClientSocket, message, tempname);
            }
            if (num == -1) {
                return 1;
            }
            else {
                // output
                return process_output(localClientSocket, ReceiveName, CountClientNumber, num, word1, word2);
            }
        }
        else if (iResult == 0) {
            printf("Connection closing...\n");
            client_left(localClientSocket);
        }
        else {
            client_left(localClientSocket);
            // left online
            return 0;
        }
        return 1;
    }
    int sendd(SOCKET ClientSocketSend, char SendStatement[DEFAULT_BUFLEN], char SendName[15]) {
        // Send
        const char* sendbuf = SendStatement;
        iSendResult = send(ClientSocketSend, sendbuf, strlen(sendbuf), 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("ERROR: send failed\n");
            client_left(ClientSocketSend);
            return 0;
            // left online
        }
        cout << "sent to " << SendName << ": " << sendbuf << endl;
        return 1;
    }
    void client_left(SOCKET localClientSocket);
};

Socket* winsocket;

// Linked List
#include "LinkedList.h"
Clients* client;

// Myfiles
#include "myfiles.h"
Myfiles* myfiles;
//myfiles->messagefile.loginid;
//myfiles->messagefile;

// Functions
#include "functions.h"

void threadreceive::operator()(int num) {
    cout << "Started receiving from " << name << endl;
    while (winsocket->receivee(localClientSocket, ClientNumber, name));
}

void Socket::accept_client() {
    while (1) {
        // Accept a client socket
        cout << "Server Listening for new Client" << endl;
        SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
        }
        cout << "Client Accepted" << endl;
        CountClientNumber++;

        client->append(ClientSocket);
    }
}
void Socket::client_left(SOCKET localClientSocket) {
    client->remove(localClientSocket);
    cout << "client left the clan" << endl;
}

int __cdecl main(void)
{
    
/**********

    kamalesh

**********/
    myfiles = new Myfiles();
    client = new Clients();
    winsocket = new Socket();

    winsocket->accept_client();
    return 100;
}