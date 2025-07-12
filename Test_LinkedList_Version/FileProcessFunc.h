#pragma once
#include"LinkedList.h"
#define INITIAL_CAPACITY 10
typedef enum E_Type_File
{
	data,
	ss,
	pss
}E_Type_File;
/*����ָ���ļ���һ���б���*/
int loadDataFromTxt(LinkedList* list, char* fileName);
/*����ָ���ļ�������datax.txt,SSx.txt,PSSx.txt*/
void createFileName(E_Type_File fileType, char** fileName, int index);
/*��ָ�����͵��ļ� �洢��ͬһ���ṹ������ָ���� �������ʹ��*/
void loadAllSamePrefixTxtFile(LinkedList*** dataSetList, int* fileCount, E_Type_File fileType);
/*�������Ľ���ļ���*/
char* createResFileName(const char* dataFileName, const char* ssOrPssFileName, E_Type_File type);