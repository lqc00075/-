#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define INITIAL_CAPACITY 10
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
	double i;
	double q;
	int id;
}Complex;
/*��ȡָ���ļ� ���������ⲿ����һ���ṹ������ĳ��� �Լ��ṹ������*/
Complex* readTxtFile(const char* fileName, int* complexCount);

/*ע��fgets()���� �ǰ��ж�ȡ */
void fgetsFuncToLoad(const char* fileName);

/*ע���ʽƥ�� ����ֵ�ǳɹ���ȡ�Ĳ�����Ŀ ͬʱ�ڴ����װ�������贫��ַ���� */
void fscanfFuncToLoad(const char* fileName);

/*����ÿһ���ļ���*/
char* createFileName(int i);

/*���㵥���ļ�ƽ���ź�ǿ��*/
double caculateAvgOfSignal(const Complex* complexs, int size);

/*�������ļ�*/
double* allFilesAvgOfSignal(int fileCount);