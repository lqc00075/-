#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
int initialNetBase();
int receiveInfoLength(const SOCKET* clientSocket);
int sendInfoLength(const SOCKET* clientSocket, int length);
int receiveOneBarInfo(const SOCKET* clientSocket, char** info);
int sendOneBarInfo(const SOCKET* clientSocket, const char* info);
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port);
DWORD WINAPI receiveInfoThreadFunc(LPVOID param);
DWORD WINAPI sendInfoThreadFunc(LPVOID param);
#pragma comment(lib,"ws2_32.lib")
int main() {
	initialNetBase();//初始化网络库

	//创建客户端套接字
	SOCKET clientSocket;
	int ret = createClientSocketAndConnect(&clientSocket, "127.0.0.1", 8080);
	if (ret != 0) {
		printf("与服务器连接失败\n");
		return -1;
	}
	HANDLE sendInfoThread = CreateThread(NULL, 0, sendInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (sendInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}
	HANDLE recvInfoThread = CreateThread(NULL, 0, receiveInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (recvInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}

	WaitForSingleObject(sendInfoThread, INFINITE);
	WaitForSingleObject(recvInfoThread, INFINITE);
	closesocket(clientSocket);
	WSACleanup();
}
/*创建客户端套接字以及和指定服务器IP和端口建立连接*/
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port) {
	//创建套接字
	*clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*clientSocket == INVALID_SOCKET) {
		printf("客户端套接字创建失败\n");
		return -1;
	}
	//寻服务器地址
	struct sockaddr_in serverAddr;		//创建地址结构体
	serverAddr.sin_family = AF_INET;	//设置网络协议族
	serverAddr.sin_port = htons(port);			//设置端口
	if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {//将人能看懂的转换为机器能看懂的
		printf("错误 无效的IP地址\n");
		return -1;
	}
	if (connect(*clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("连接失败\n");
		return -1;
	}
	return 0;
}

/*初始化网络库 就相当于是打开网络 告诉操作系统 嘿 这个程序需要联网*/
int initialNetBase() {
	WSADATA wsaData;
	//MAKEWORD 是一个宏 表示需要使用的版本号 这里表示2.2版本 它会自动合成一个16位的版本号
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);//返回0 说明初始化成功 返回非0 说明初始化失败 网络打不开
	if (res != 0) {
		printf("网络初始化失败，不能联网\n");
		return -1;
	}
	return 0;
}

/*发送信息长度*/
int sendInfoLength(const SOCKET* clientSocket, int length) {
	if (length == 0) {
		printf("长度为0，无法发送\n");
		return -1;
	}
	int netWorkLength = htonl(length);//将主机序转为网络序
	int totalSent = 0;
	while (totalSent < sizeof(int)) {
		int tempRes = send(*clientSocket, (char*)&netWorkLength + totalSent,
			sizeof(int) - totalSent, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("发送长度信息失败\n");
			return -1;
		}
		totalSent += tempRes;
	}
	printf("长度信息发送成功：[%d] 字节\n\n", length);
	return 0;
}

/*返回值为接收到的信息长度*/
int receiveInfoLength(const SOCKET* clientSocket) {
	int hostLength = 0;
	int totalGet = 0;
	while (totalGet < sizeof(int)) {
		int tempRes = recv(*clientSocket, (char*)&hostLength + totalGet, sizeof(int) - totalGet, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("接收长度信息失败\n");
			return -1;
		}
		totalGet += tempRes;
	}
	hostLength = ntohl(hostLength);//收到的网络序 转回来变成主机序
	printf("成功接收信息长度：[%d] 字节\n\n", hostLength);
	return hostLength;
}

/*接收到一条信息*/
int receiveOneBarInfo(const SOCKET* clientSocket, char** info) {
	if (*clientSocket == INVALID_SOCKET) return-1;
	struct sockaddr_in clientAddr;		//为获取客户端IP服务
	int addrLen = sizeof(clientAddr);
	int clientPort = -1;
	char clientIP[INET_ADDRSTRLEN] = "未知IP";
	//获取IP 端口信息
	if (getpeername(*clientSocket, (struct sockaddr*)&clientAddr, &addrLen) == 0) {
		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		clientPort = ntohs(clientAddr.sin_port);
	}

	//先计算需要获取的字节长度
	int receiveLength = receiveInfoLength(clientSocket);
	*info = malloc(receiveLength + 1);
	if (*info == NULL) {
		printf("内存分配失败\n");
		return -1;
	}
	int totalRecvLength = 0;
	while (totalRecvLength < receiveLength) {
		int tempRes = recv(*clientSocket, *info + totalRecvLength, receiveLength - totalRecvLength, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("接收信息失败\n");
			free(*info);
			return -1;
		}
		totalRecvLength += tempRes;
	}
	(*info)[receiveLength] = '\0';
	printf("成功收到IP:%s, 端口:%d的信息，信息为「 %s 」\n\n", clientIP, clientPort, *info);
	return 0;
}

/*发送一条信息*/
int sendOneBarInfo(const SOCKET* clientSocket, const char* info) {
	if (*clientSocket == INVALID_SOCKET) return -1;
	if (info == NULL) {
		printf("发送的信息不能为空！");
		return -1;
	}
	sendInfoLength(clientSocket, strlen(info));//先发送一个长度信息
	int totalSent = 0;
	while (totalSent < strlen(info)) {
		int tempRes = send(*clientSocket, info + totalSent, strlen(info) - totalSent, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("发送信息失败\n");
			return -1;
		}
		totalSent += tempRes;
	}
	printf("成功发送一条信息：{ %s }\n\n", info);
	return 0;
}

DWORD WINAPI receiveInfoThreadFunc(LPVOID param) {
	SOCKET sock = (SOCKET)param;
	while (1) {
		char* info = NULL;
		receiveOneBarInfo(&sock, &info);
		free(info);
	}
}

DWORD WINAPI sendInfoThreadFunc(LPVOID param) {
	SOCKET sock = (SOCKET)param;
	printf("这里是[360B]\n");
	sendOneBarInfo(&sock, "360B");
	while (1) {
		printf("[360B(客户端)]请输入信息：\t\n");
		char message[256];
		fgets(message, sizeof(message), stdin);
		message[strcspn(message, "\n")] = '\0';
		sendOneBarInfo(&sock, message);
	}
}