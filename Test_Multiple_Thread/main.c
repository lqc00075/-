#include <windows.h>
#include <stdio.h>
DWORD WINAPI TestThreadFunc(LPVOID param);

int main() {
	printf("���������̡߳�����������\n");
	int inta = 666;
	//����һ���߳�
	HANDLE sonThread = CreateThread(NULL,0, TestThreadFunc,&inta,0,NULL);
	if (sonThread == NULL) {
		printf("���̴߳���ʧ��\n");
		return -1;
	}
	printf("�̴߳����ɹ����ȴ��߳̽���\n");
	WaitForSingleObject(sonThread, INFINITE);
	printf("�߳��ѽ��������̼߳���\n");
	CloseHandle(sonThread);
	return 0;
}

DWORD WINAPI TestThreadFunc(LPVOID param) {
	
	int a = *(int*)param;
	printf("����������Ǵ�����һ�����߳�,������һ����������%d\n",a);
	for (int i = 0; i < 10; i++) {
		printf("����ǽ佾���꣬��������\n");
	}
	printf("�õģ��������Ѿ��ǳ�������\n");
	return 0;
}