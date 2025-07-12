#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
int initialNetBase();
int receiveInfoLength(const SOCKET* clientSocket);
int sendInfoLength(const SOCKET* clientSocket, int length);
int receiveOneBarInfo(const SOCKET* clientSocket, char** info);
int sendOneBarInfo(const SOCKET* clientSocket, const char* info);
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port);
DWORD WINAPI receiveInfoThreadFunc(LPVOID param);
DWORD WINAPI sendInfoThreadFunc(LPVOID param);