#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
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
SOCKET clientSockets[20] = { 0 };
ClientsInfo clientsSocketInfo[20];

int main() {
	printf("0708号服务器正式开服啦！\n");
	//初始化网络库 目的是为了启动网路 联网功能打开
	initialNetBase();
	//初始化一些数据
	int result = 0;
	SOCKET serverSocket;
	//初始化服务器端socket以及绑定IP端口
	initialServerSocketAndBind(&serverSocket, 8082);
	//监听以及接收
	int ret = listenFunc(&serverSocket);
	if (ret != 0) {
		printf("无法监听\n");
		return -1;
	}

	//SOCKET clientSocket = acceptFunc(&serverSocket);
	HANDLE acceptThread = CreateThread(NULL, 0, acceptClientThreadFunc, (LPVOID)serverSocket, 0, NULL);
	//HANDLE broadThread = CreateThread(NULL, 0, broadAllClientsSocket, NULL, 0, NULL);

	//getchar();
	//sendToDesignatedClient("082C");
	int communicationMode = 0; // 0: 多人, 1: 单人
	char currentTarget[20] = ""; // 当前单人模式的目标用户名

	while (1) {
		printf("\n输入m切换多人，s切换单人，或直接输入消息：\n> ");
		char input[256];
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';
		if (strcmp(input, "m") == 0) {
			communicationMode = 0;
			printf("已切换到【多人模式】，你可以输入信息内容啦\n");
		} else if (strcmp(input, "s") == 0) {
			communicationMode = 1;
			printf("已切换到【单人模式】，请输入你想指定连接的用户，现在已上线的用户有：082C 360B 095A\n> ");
			fgets(currentTarget, sizeof(currentTarget), stdin);
			currentTarget[strcspn(currentTarget, "\n")] = '\0';
			printf("已成功切换至指定用户，当前用户为【%s】\n", currentTarget);
		} else {
			if (communicationMode == 0) {
				// 广播
				for (int i = 0; i < 20; i++) {
					if (clientSockets[i] != 0) {
						sendOneBarInfo(&clientSockets[i], input);
					}
				}
			} else {
				// 单人
				SOCKET clientCurSocket = INVALID_SOCKET;
				for (int i = 0; i < 20; i++) {
					if (strcmp(currentTarget, clientsSocketInfo[i].clientName) == 0) {
						clientCurSocket = clientsSocketInfo[i].clientSocket;
						break;
					}
				}
				if (clientCurSocket != INVALID_SOCKET) {
					sendOneBarInfo(&clientCurSocket, input);
				} else {
					printf("未找到该用户，请重新切换目标用户。\n");
					communicationMode = 0; // 自动切回多人模式或提示重新输入
				}
			}
		}
	}


	WaitForSingleObject(acceptThread, INFINITE);

	#pragma region test
	//printf("*****************************************\n");
	//sendOneBarInfo(&clientSocket, "你好这里是0708号服务器，很高兴为您服务");
	//char* info = NULL;
	//receiveOneBarInfo(&clientSocket, &info);
	//free(info);
	//while (1) {
	//	char* tempInfo = NULL;
	//	receiveOneBarInfo(&clientSocket, &tempInfo);

	//	printf("[服务器]请输入文本信息\t");
	//	char message[256];
	//	fgets(message, sizeof(message), stdin);
	//	message[strcspn(message, "\n")] = '\0';
	//	sendOneBarInfo(&clientSocket, message);

	//	free(tempInfo);
	//}
	#pragma endregion

	//关闭连接
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
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
/*接收一个客户端的连接*/
SOCKET acceptFunc(const SOCKET* serverSocket) {
	SOCKET clientSocket = accept(*serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		printf("无法与客户端建立连接\n");
		return INVALID_SOCKET;
	}
	return clientSocket;
}
/*默认设置地址格式为IPV4 监听本地所有IP出口*/
int initialServerSocketAndBind(SOCKET* serverSocket, int port) {
	*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*serverSocket == INVALID_SOCKET) {
		printf("服务器套接字创建失败\t错误码为%ld\n", WSAGetLastError());
		return WSAGetLastError();
	}
	struct sockaddr_in serverIpAddress;
	serverIpAddress.sin_family = AF_INET;
	serverIpAddress.sin_port = htons(port);//主机序转网络序 16位 端口用
	//类似的还有 htonl 主机序转网络序 32位 地址用
	//		   ntohs 网络序转主机序 16位 端口用
	//		   ntohl 网络序转主机序 32位 地址用
	serverIpAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(*serverSocket, (struct sockaddr*)&serverIpAddress, sizeof(serverIpAddress)) == SOCKET_ERROR) {
		printf("绑定失败\t错误代码为%ld", WSAGetLastError());
		return WSAGetLastError();
	} else {
		printf("服务器地址为本地环回地址127.0.0.1\n");
		printf("绑定端口成功，当前端口为：%d,可以开始准备监听\n", port);
		return 0;
	}
}
/*监听和接收 注意这里不会关闭服务器的Socket 以及清理网络库 主要是想的自己在外部统一管理*/
int listenFunc(const SOCKET* serverSocket) {
	if (listen(*serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("监听失败\t错误代码%ld\n", WSAGetLastError());
		return WSAGetLastError();
	}
	return 0;
}
/*发送表示即将发送信息的长度*/
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
/*接收一条信息*/
int receiveOneBarInfo(const SOCKET* clientSocket, char** info) {
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
	if (receiveLength == -1) {
		printf("客户端IP:%s, 端口:%d 已断开\n\n", clientIP, clientPort);
		return -1;
	}
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
	if (strcmp(*info, "082C") == 0) {
		strcpy(clientsSocketInfo[0].clientName, "082C");
		clientsSocketInfo[0].clientSocket = *clientSocket;
		printf("082C用户已上线\n");
	} else if (strcmp(*info, "360B") == 0) {
		strcpy(clientsSocketInfo[1].clientName, "360B");
		clientsSocketInfo[1].clientSocket = *clientSocket;
		printf("360B用户已上线\n");
	} else if (strcmp(*info, "095A") == 0) {
		strcpy(clientsSocketInfo[2].clientName, "095A");
		clientsSocketInfo[2].clientSocket = *clientSocket;
		printf("095A用户已上线\n");
	}
	return 0;
}
/*发送一条信息*/
int sendOneBarInfo(const SOCKET* clientSocket, const char* info) {
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
/*接收信息线程函数*/
DWORD WINAPI receiveClientInfoThreadFunc(LPVOID param) {
	SOCKET sock = (SOCKET)param;
	while (1) {
		char* tempInfo = NULL;
		int ret = receiveOneBarInfo(&sock, &tempInfo);
		if (ret == -1) {
			for (int i = 0; i < 20; i++) {
				if (clientSockets[i] == sock) {
					clientSockets[i] = 0;
					closesocket(sock);
					return;
				}
			}
		}
		if (ret != -1)
			free(tempInfo);
	}
}
/*发送信息线程函数*/
DWORD WINAPI sendClientInfoThreadFunc(LPVOID param) {
	SOCKET sock = (SOCKET)param;
	while (1) {
		printf("[服务器]请输入文本信息\t");
		char message[256];
		fgets(message, sizeof(message), stdin);
		message[strcspn(message, "\n")] = '\0';
		sendOneBarInfo(&sock, message);
	}
}
/*接收客户端连接线程函数*/
DWORD WINAPI acceptClientThreadFunc(LPVOID param) {
	int index = 0;
	SOCKET serverSocket = (SOCKET)param;
	while (index < 20) {
		SOCKET clientSocket = acceptFunc(&serverSocket);
		if (clientSocket == INVALID_SOCKET) {
			continue;
		}
		clientSockets[index] = clientSocket;
		HANDLE receiveThread = CreateThread(NULL, 0, receiveClientInfoThreadFunc, (LPVOID)clientSockets[index], 0, NULL);
		//HANDLE sendThread = CreateThread(NULL, 0, sendClientInfoThreadFunc, (LPVOID)clientSockets[index], 0, NULL);
		while (1) {
			int flag = 0;
			for (int i = 0; i < 20; i++) {
				if (clientSockets[i] == 0) {
					index = i;
					flag = 1;
					break;
				}
			}
			if (flag) break;
			Sleep(1000); // 加上这个，CPU占用会大幅下降
		}
	}
}