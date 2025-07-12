#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
typedef struct {
	SOCKET clientSocket;
	char clientName[20];
}ClientsInfo;


int initialNetBase();
int initialServerSocketAndBind(SOCKET* serverSocket, int port);
int listenFunc(const SOCKET* serverSocket);
int receiveInfoLength(const SOCKET* clientSocket);
int sendInfoLength(const SOCKET* clientSocket, int length);
int receiveOneBarInfo(const SOCKET* clientSocket, char** info);
int sendOneBarInfo(const SOCKET* clientSocket, const char* info);

SOCKET acceptFunc(const SOCKET* serverSocket);
DWORD WINAPI receiveClientInfoThreadFunc(LPVOID param);
DWORD WINAPI sendClientInfoThreadFunc(LPVOID param);
DWORD WINAPI acceptClientThreadFunc(LPVOID param);