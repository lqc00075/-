#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
/*��ʼ�������*/
int initialNetBase();
/*������Ϣ�ֽڳ���*/
int receiveInfoLength(const SOCKET* clientSocket);
/*������Ϣ����*/
int sendInfoLength(const SOCKET* clientSocket, int length);
/*����һ����Ϣ*/
int receiveOneBarInfo(const SOCKET* clientSocket, char** info);
/*����һ����Ϣ*/
int sendOneBarInfo(const SOCKET* clientSocket, const char* info);
/*�����ͻ����׽��ֲ���֮��������*/
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port);
/*���߳� ���ڽ�����Ϣ*/
DWORD WINAPI receiveInfoThreadFunc(LPVOID param);
/*���߳� ���ڷ�����Ϣ*/
DWORD WINAPI sendInfoThreadFunc(LPVOID param);
extern char resultCell[20];// ����ȫ�ֱ���