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
	initialNetBase();//��ʼ�������

	//�����ͻ����׽���
	SOCKET clientSocket;
	int ret = createClientSocketAndConnect(&clientSocket, "127.0.0.1", 8080);
	if (ret != 0) {
		printf("�����������ʧ��\n");
		return -1;
	}
	HANDLE sendInfoThread = CreateThread(NULL, 0, sendInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (sendInfoThread == NULL) {
		printf("sendInfoThread����ʧ��\n");
		return -1;
	}
	HANDLE recvInfoThread = CreateThread(NULL, 0, receiveInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (recvInfoThread == NULL) {
		printf("sendInfoThread����ʧ��\n");
		return -1;
	}

	WaitForSingleObject(sendInfoThread, INFINITE);
	WaitForSingleObject(recvInfoThread, INFINITE);
	closesocket(clientSocket);
	WSACleanup();
}
/*�����ͻ����׽����Լ���ָ��������IP�Ͷ˿ڽ�������*/
int createClientSocketAndConnect(SOCKET* clientSocket, char* serverIP, int port) {
	//�����׽���
	*clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*clientSocket == INVALID_SOCKET) {
		printf("�ͻ����׽��ִ���ʧ��\n");
		return -1;
	}
	//Ѱ��������ַ
	struct sockaddr_in serverAddr;		//������ַ�ṹ��
	serverAddr.sin_family = AF_INET;	//��������Э����
	serverAddr.sin_port = htons(port);			//���ö˿�
	if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {//�����ܿ�����ת��Ϊ�����ܿ�����
		printf("���� ��Ч��IP��ַ\n");
		return -1;
	}
	if (connect(*clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("����ʧ��\n");
		return -1;
	}
	return 0;
}

/*��ʼ������� ���൱���Ǵ����� ���߲���ϵͳ �� ���������Ҫ����*/
int initialNetBase() {
	WSADATA wsaData;
	//MAKEWORD ��һ���� ��ʾ��Ҫʹ�õİ汾�� �����ʾ2.2�汾 �����Զ��ϳ�һ��16λ�İ汾��
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);//����0 ˵����ʼ���ɹ� ���ط�0 ˵����ʼ��ʧ�� ����򲻿�
	if (res != 0) {
		printf("�����ʼ��ʧ�ܣ���������\n");
		return -1;
	}
	return 0;
}

/*������Ϣ����*/
int sendInfoLength(const SOCKET* clientSocket, int length) {
	if (length == 0) {
		printf("����Ϊ0���޷�����\n");
		return -1;
	}
	int netWorkLength = htonl(length);//��������תΪ������
	int totalSent = 0;
	while (totalSent < sizeof(int)) {
		int tempRes = send(*clientSocket, (char*)&netWorkLength + totalSent,
			sizeof(int) - totalSent, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("���ͳ�����Ϣʧ��\n");
			return -1;
		}
		totalSent += tempRes;
	}
	printf("������Ϣ���ͳɹ���[%d] �ֽ�\n\n", length);
	return 0;
}

/*����ֵΪ���յ�����Ϣ����*/
int receiveInfoLength(const SOCKET* clientSocket) {
	int hostLength = 0;
	int totalGet = 0;
	while (totalGet < sizeof(int)) {
		int tempRes = recv(*clientSocket, (char*)&hostLength + totalGet, sizeof(int) - totalGet, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("���ճ�����Ϣʧ��\n");
			return -1;
		}
		totalGet += tempRes;
	}
	hostLength = ntohl(hostLength);//�յ��������� ת�������������
	printf("�ɹ�������Ϣ���ȣ�[%d] �ֽ�\n\n", hostLength);
	return hostLength;
}

/*���յ�һ����Ϣ*/
int receiveOneBarInfo(const SOCKET* clientSocket, char** info) {
	if (*clientSocket == INVALID_SOCKET) return-1;
	struct sockaddr_in clientAddr;		//Ϊ��ȡ�ͻ���IP����
	int addrLen = sizeof(clientAddr);
	int clientPort = -1;
	char clientIP[INET_ADDRSTRLEN] = "δ֪IP";
	//��ȡIP �˿���Ϣ
	if (getpeername(*clientSocket, (struct sockaddr*)&clientAddr, &addrLen) == 0) {
		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		clientPort = ntohs(clientAddr.sin_port);
	}

	//�ȼ�����Ҫ��ȡ���ֽڳ���
	int receiveLength = receiveInfoLength(clientSocket);
	*info = malloc(receiveLength + 1);
	if (*info == NULL) {
		printf("�ڴ����ʧ��\n");
		return -1;
	}
	int totalRecvLength = 0;
	while (totalRecvLength < receiveLength) {
		int tempRes = recv(*clientSocket, *info + totalRecvLength, receiveLength - totalRecvLength, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("������Ϣʧ��\n");
			free(*info);
			return -1;
		}
		totalRecvLength += tempRes;
	}
	(*info)[receiveLength] = '\0';
	printf("�ɹ��յ�IP:%s, �˿�:%d����Ϣ����ϢΪ�� %s ��\n\n", clientIP, clientPort, *info);
	return 0;
}

/*����һ����Ϣ*/
int sendOneBarInfo(const SOCKET* clientSocket, const char* info) {
	if (*clientSocket == INVALID_SOCKET) return -1;
	if (info == NULL) {
		printf("���͵���Ϣ����Ϊ�գ�");
		return -1;
	}
	sendInfoLength(clientSocket, strlen(info));//�ȷ���һ��������Ϣ
	int totalSent = 0;
	while (totalSent < strlen(info)) {
		int tempRes = send(*clientSocket, info + totalSent, strlen(info) - totalSent, 0);
		if (tempRes == SOCKET_ERROR) {
			printf("������Ϣʧ��\n");
			return -1;
		}
		totalSent += tempRes;
	}
	printf("�ɹ�����һ����Ϣ��{ %s }\n\n", info);
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
	printf("������[360B]\n");
	sendOneBarInfo(&sock, "360B");
	while (1) {
		printf("[360B(�ͻ���)]��������Ϣ��\t\n");
		char message[256];
		fgets(message, sizeof(message), stdin);
		message[strcspn(message, "\n")] = '\0';
		sendOneBarInfo(&sock, message);
	}
}