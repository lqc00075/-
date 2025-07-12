#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"FileProcessFunc.h"
/*���ص���ָ��txt�ļ�*/
int loadDataFromTxt(LinkedList* list, char* fileName) {
	if (fileName == NULL) {
		printf("�ļ�����Ч\n");
		return -1;
	}
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("�ļ���:  %s\t��ʧ��\n", fileName);
		return -1;
	}

	int count = 0;
	double tempI = 0, tempQ = 0;
	while (fscanf(file, "%lf", &tempI) == 1) {
		if (fscanf(file, "%lf", &tempQ) != 1) {
			printf("�ļ����ݲ��ɶ�\n");
			return -1;
		}
		Node* node = createNode(tempI, tempQ, count);
		addHeadList(list, node);
		count++;
	}
	// ����Ƿ���IO�����˳�ѭ��
	if (ferror(file)) {
		fprintf(stderr, "��ȡ�ļ�ʱ��������\n");
		fclose(file); // �ر��ļ�
		return -1;
	}
	fclose(file);
	return 0;
}
/*����ָ�������ļ���datax.txt,SSx.txt,PSS.txt*/
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
		printf("�ļ�������ʧ�ܣ������Ƿ���data.txt,ss.txt,pss.txt�ļ�\n");
		break;
	}
	return;
}
/*����ͬ���͵�ȫ���ļ� �ŵ�һ��ָ�������� ����ָ�����ⲿ����ĵ�ַ ������һ�εõ�ָ������ ���������εõ�ÿ��list�ĵ�ַ*/
void loadAllSamePrefixTxtFile(LinkedList*** dataSetList, int* fileCount, E_Type_File fileType) {
	int capacity = INITIAL_CAPACITY;
	*fileCount = 0;
	*dataSetList = malloc(sizeof(LinkedList*) * capacity);
	for (int i = 0; ; i++) {
		char* fileName = NULL;
		LinkedList* list = malloc(sizeof(LinkedList));
		initialLinkedList(list);
		createFileName(fileType, &fileName, i);//����ָ�뷵��
		int success = loadDataFromTxt(list, fileName);
		if (success == -1) {
			free(fileName);
			free(list);
			printf("������%d�ļ��Ѿ���ȡ���(0����data����,\t1����SS����,\t2����PSS����)\n\n", fileType);
			break;
		}
		if (*fileCount >= capacity) {
			capacity *= 2;
			LinkedList** temp = realloc(*dataSetList, sizeof(LinkedList*) * capacity);
			if (temp == NULL) {
				printf("Ϊ���ݼ���dataSetList�ڴ����ʧ��\n");
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

/*����ÿһ��data����ļ�*/
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
		printf("��ȡ�����ļ���data�е�����ʧ��\n");
		return NULL;
	}
	switch (type)
	{
	case ss:
		if (sscanf(ssOrPssFileName, "SS%d.txt", &ssOrPssNum) != 1) {
			printf("��ȡȷ���ź�SS�е�����ʧ��\n");
			return NULL;
		}
		fileNameSize = snprintf(NULL, 0, "data%dss%d", dataNum, ssOrPssNum);
		fileName = (char*)malloc(fileNameSize + 1);
		snprintf(fileName, fileNameSize + 1, "data%dss%d", dataNum, ssOrPssNum);
		return fileName;
	case pss:
		if (sscanf(ssOrPssFileName, "PSS%d.txt", &ssOrPssNum) != 1) {
			printf("��ȡȷ���ź�PSS�е�����ʧ��\n");
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
