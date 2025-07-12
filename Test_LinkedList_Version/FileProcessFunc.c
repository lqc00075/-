#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"FileProcessFunc.h"
/*加载单个指定txt文件*/
int loadDataFromTxt(LinkedList* list, char* fileName) {
	if (fileName == NULL) {
		printf("文件名无效\n");
		return -1;
	}
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("文件名:  %s\t打开失败\n", fileName);
		return -1;
	}

	int count = 0;
	double tempI = 0, tempQ = 0;
	while (fscanf(file, "%lf", &tempI) == 1) {
		if (fscanf(file, "%lf", &tempQ) != 1) {
			printf("文件数据不成对\n");
			return -1;
		}
		Node* node = createNode(tempI, tempQ, count);
		addHeadList(list, node);
		count++;
	}
	// 检查是否因IO错误退出循环
	if (ferror(file)) {
		fprintf(stderr, "读取文件时发生错误\n");
		fclose(file); // 关闭文件
		return -1;
	}
	fclose(file);
	return 0;
}
/*创建指定名的文件如datax.txt,SSx.txt,PSS.txt*/
void createFileName(E_Type_File fileType, char** fileName, int index) {
	int fileNameSize = 0;
	switch (fileType)
	{
	case data:
		fileNameSize = snprintf(NULL, 0, "data%d.txt", index);
		*fileName = malloc(fileNameSize + 1);
		snprintf(*fileName, fileNameSize + 1, "data%d.txt", index);
		break;
	case ss:
		fileNameSize = snprintf(NULL, 0, "SS%d.txt", index);
		*fileName = malloc(fileNameSize + 1);
		snprintf(*fileName, fileNameSize + 1, "SS%d.txt", index);
		break;
	case pss:
		fileNameSize = snprintf(NULL, 0, "PSS%d.txt", index);
		*fileName = malloc(fileNameSize + 1);
		snprintf(*fileName, fileNameSize + 1, "PSS%d.txt", index);
		break;
	default:
		printf("文件名创建失败，请检查是否是data.txt,ss.txt,pss.txt文件\n");
		break;
	}
	return;
}
/*加载同类型的全部文件 放到一个指针数组中 三级指针是外部不变的地址 解引用一次得到指针数组 解引用两次得到每个list的地址*/
void loadAllSamePrefixTxtFile(LinkedList*** dataSetList, int* fileCount, E_Type_File fileType) {
	int capacity = INITIAL_CAPACITY;
	*fileCount = 0;
	*dataSetList = malloc(sizeof(LinkedList*) * capacity);
	for (int i = 0; ; i++) {
		char* fileName = NULL;
		LinkedList* list = malloc(sizeof(LinkedList));
		initialLinkedList(list);
		createFileName(fileType, &fileName, i);//二级指针返回
		int success = loadDataFromTxt(list, fileName);
		if (success == -1) {
			free(fileName);
			free(list);
			printf("此类型%d文件已经读取完毕(0代表data数据,\t1代表SS数据,\t2代表PSS数据)\n\n", fileType);
			break;
		}
		if (*fileCount >= capacity) {
			capacity *= 2;
			LinkedList** temp = realloc(*dataSetList, sizeof(LinkedList*) * capacity);
			if (temp == NULL) {
				printf("为数据集即dataSetList内存分配失败\n");
				return;
			} else {
				*dataSetList = temp;
			}
			//*dataSetList = realloc(*dataSetList, sizeof(LinkedList*) * capacity);
		}
		(*dataSetList)[(*fileCount)++] = list;
		free(fileName);
	}
	*dataSetList = realloc(*dataSetList, sizeof(LinkedList*) * (*fileCount));
	return;
}

/*创建每一个data结果文件*/
char* createResFileName(const char* dataFileName, const char* ssOrPssFileName,E_Type_File type) {
	//
	//sscanf(dataFileName, "data%d.txt", &dataNum);
	//sscanf(ssFileName, "SS%d.txt", &ssNum);
	//int fileNameSize = snprintf(NULL, 0, "data%dss%d", dataNum, ssNum);
	//char* fileName = (char*)malloc(fileNameSize + 1);
	//snprintf(fileName, fileNameSize + 1, "data%dss%d", dataNum, ssNum);
	//return fileName;
	int dataNum = 0, ssOrPssNum = 0, fileNameSize = 0;
	char* fileName = NULL;
	if (sscanf(dataFileName, "data%d.txt", &dataNum) != 1) {
		printf("提取基本文件名data中的数字失败\n");
		return NULL;
	}
	switch (type)
	{
	case ss:
		if (sscanf(ssOrPssFileName, "SS%d.txt", &ssOrPssNum) != 1) {
			printf("提取确定信号SS中的数字失败\n");
			return NULL;
		}
		fileNameSize = snprintf(NULL, 0, "data%dss%d", dataNum, ssOrPssNum);
		fileName = (char*)malloc(fileNameSize + 1);
		snprintf(fileName, fileNameSize + 1, "data%dss%d", dataNum, ssOrPssNum);
		return fileName;
	case pss:
		if (sscanf(ssOrPssFileName, "PSS%d.txt", &ssOrPssNum) != 1) {
			printf("提取确定信号PSS中的数字失败\n");
			return NULL;
		}
		fileNameSize = snprintf(NULL, 0, "data%dss%d", dataNum, ssOrPssNum);
		fileName = (char*)malloc(fileNameSize + 1);
		snprintf(fileName, fileNameSize + 1, "data%dss%d", dataNum, ssOrPssNum);
		return fileName;
	default:
		break;
	}
}
