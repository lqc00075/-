#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NetCommunicationFunc.h"


int main() {
	initialNetBase();//初始化网络库
	//创建客户端套接字
	SOCKET clientSocket;
	int ret = createClientSocketAndConnect(&clientSocket, "127.0.0.1", 8082);
	if (ret != 0) {
		printf("与服务器连接失败\n");
		return -1;
	}
	HANDLE sendInfoThread = CreateThread(NULL, 0, sendInfoThreadFunc,(LPVOID)clientSocket, 0, NULL);
	if (sendInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}
	HANDLE recvInfoThread = CreateThread(NULL, 0, receiveInfoThreadFunc,(LPVOID)clientSocket, 0, NULL);
	if (recvInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}
	WaitForSingleObject(sendInfoThread, INFINITE);
	WaitForSingleObject(recvInfoThread, INFINITE);
	closesocket(clientSocket);
	WSACleanup();
}

