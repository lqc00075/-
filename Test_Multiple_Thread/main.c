#include <windows.h>
#include <stdio.h>
DWORD WINAPI TestThreadFunc(LPVOID param);

int main() {
	printf("这里是主线程………………\n");
	int inta = 666;
	//创建一个线程
	HANDLE sonThread = CreateThread(NULL,0, TestThreadFunc,&inta,0,NULL);
	if (sonThread == NULL) {
		printf("子线程创建失败\n");
		return -1;
	}
	printf("线程创建成功，等待线程结束\n");
	WaitForSingleObject(sonThread, INFINITE);
	printf("线程已结束，主线程继续\n");
	CloseHandle(sonThread);
	return 0;
}

DWORD WINAPI TestThreadFunc(LPVOID param) {
	
	int a = *(int*)param;
	printf("这里是刘清城创建的一个子线程,这里有一个参数叫做%d\n",a);
	for (int i = 0; i < 10; i++) {
		printf("刘清城戒骄戒躁，静下心来\n");
	}
	printf("好的，现在你已经非常安静了\n");
	return 0;
}