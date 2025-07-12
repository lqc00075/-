#pragma once
#include"LinkedList.h"
#define INITIAL_CAPACITY 10
typedef enum E_Type_File
{
	data,
	ss,
	pss
}E_Type_File;
/*加载指定文件到一个列表中*/
int loadDataFromTxt(LinkedList* list, char* fileName);
/*创建指定文件名例如datax.txt,SSx.txt,PSSx.txt*/
void createFileName(E_Type_File fileType, char** fileName, int index);
/*将指定类型的文件 存储至同一个结构体数组指针中 方便后续使用*/
void loadAllSamePrefixTxtFile(LinkedList*** dataSetList, int* fileCount, E_Type_File fileType);
/*创建最后的结果文件名*/
char* createResFileName(const char* dataFileName, const char* ssOrPssFileName, E_Type_File type);