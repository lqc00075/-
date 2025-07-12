#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
/*创建每一个文件名*/
char* createSSFileName(int i) {
	int fileNameSize = snprintf(NULL, 0, "SS%d.txt", i);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "SS%d.txt", i);
	return fileName;
}

/*创建每一个PSS文件*/
char* createPSSFileName(int i) {
	int fileNameSize = snprintf(NULL, 0, "PSS%d.txt", i);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "PSS%d.txt", i);
	return fileName;
}

/*创建每一个data文件*/
char* createDataFileName(int i) {
	int fileNameSize = snprintf(NULL, 0, "data%d.txt", i);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "data%d.txt", i);
	return fileName;
}
/*创建每一个data结果文件*/
char* createResFileName(const char* dataFileName,const char* ssFileName) {
	int dataNum = 0, ssNum = 0;
	sscanf(dataFileName, "data%d.txt", &dataNum);
	sscanf(ssFileName, "PSS%d.txt", &ssNum);
	int fileNameSize = snprintf(NULL, 0, "data%dss%d", dataNum, ssNum);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "data%dss%d", dataNum, ssNum);
	return fileName;
}
