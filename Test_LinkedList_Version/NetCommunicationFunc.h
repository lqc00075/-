#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
/*初始化网络库*/
int initialNetBase();
/*接收信息字节长度*/
int receiveInfoLength(const SOCKET* clientSocket);
/*发送信息长度*/
int sendInfoLength(const SOCKET* clientSocket, int length);
/*接收一条信息*/
int receiveOneBarInfo(const SOCKET* clientSocket, char** info);
/*发送一条信息*/
int sendOneBarInfo(const SOCKET* clientSocket, const char* info);
/*创建客户端套接字并与之建立连接*/
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port);
/*子线程 用于接受信息*/
DWORD WINAPI receiveInfoThreadFunc(LPVOID param);
/*子线程 用于发送信息*/
DWORD WINAPI sendInfoThreadFunc(LPVOID param);
extern char resultCell[20];// 声明全局变量