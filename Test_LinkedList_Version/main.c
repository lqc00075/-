#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NetCommunicationFunc.h"
#include "Map.h"
#include "LinkedList.h"
#include "FileProcessFunc.h"
#include "CellFunc.h"
#include "DataBaseFunc.h"
#define THRESHOLD 1000.0
//ƽ���ź�ǿ��ָ������ļ���Ŀ
#define FILE_COUNTS 3
#define INITIAL_CAPACITY_CELLS 1
#define DEFAULT_ENSURE_FILE_TYPE pss

#pragma region ��������
void caclateMultipleFileAvgSignalStrength(const LinkedList** dataSetList, int fileCount, double** resAvg);
void printDataSet(const LinkedList** dataSetList, E_Type_File fileType, int fileCount);
void addDataToMap(ResultsMap* resMap, const LinkedList** dataSetList, E_Type_File fileType, int fileCount);
void printAvgRes(const double* res, int fileCount);
double caculateSingleFileAvgSignalStrength(const LinkedList* list, const char* fileName);
void relatedTest(const ResultsMap* mapFilterd, const LinkedList** ssOrPssSetList, int dataFilteredFileCount, int ssOrPssFileCount, Cell** cells, int* cellsCount, int capacity, E_Type_File fileType);
void caculateRelated(const LinkedList* dataList, const LinkedList* ssOrPssList, double** res);
void insertOrderedDataFileInfoToAvgOfStrengthTable(const ResultsMap* orderedDataFileMap, MYSQL* connect);
void insertCellsInfoToRelatedTable(const Cell* cells, MYSQL* connect, int cellsCount);
void insertCellResultInfoToCellsSelectedTable(const Cell cell, MYSQL* connect, const char* resFileName);
#pragma endregion
#pragma comment(lib,"ws2_32.lib")

char resultCell[20];
int main() {

	#pragma region ���� �ļ��Ƿ��ȡ���� ����ƽ���ź�ǿ���Ƿ���ȷ
	//LinkedList* list = malloc(sizeof(LinkedList));
	//initialLinkedList(list);
	//���Լ�������
	//printList(&list);
	//char* fileName  = NULL ;
	//createFileName(data,&fileName, 6);
	//loadDataFromTxt(list,fileName);
	//printf("%s\n", fileName);
	//printList(list);
	//printf("%lf\n", caculateSingleFileAvgSignalStrength(list, fileName));
	//free(fileName);
	//destroyList(list);
	#pragma endregion

	#pragma region ��ʼ�� ��ʼ��MySql���� ���ݼ��Ľ��� ��ָ�������ʼ�� �Լ���¼��ϣ��ĳ�ʼ�� �������մ洢��ѡ���ĳ�ʼ��

	

	MYSQL* connect = NULL;
	initialMySqlAndConnectDatabase(&connect, "127.0.0.1", "root", "18598553608lqC", "mysql", 0, NULL, 0);
	dropDataBase(connect, "lqc00075");
	createDataBase(connect, "lqc00075");
	changeDataBase(connect, "lqc00075");
	int dataFileCount = 0;
	int ssOrPssFileCount = 0;
	int cellsCount = 0;
	//double* res = NULL;
	LinkedList** dataSetList = NULL;							//data���ݼ�
	LinkedList** ssOrPssSetList = NULL;							//ssOrPss���ݼ�
	ResultsMap mapOriginal;										//��ʼ���ݼ� �洢���е��ļ��������
	ResultsMap mapFiltered;										//ɸѡ������ֵ�����ݼ�
	ResultsMap mapTargetCountAndMax;							//ɸѡ������ֵͬʱ������Ŀ�����ݼ�
	Cell* cells = malloc(sizeof(Cell) * INITIAL_CAPACITY_CELLS);//�������õĶ�̬�ڴ���Ϊ�˴������չ�� ����Ժ��ʹ�÷ǳ��ǳ�������� ����Ϳ�������

	initialResultsMap(&mapOriginal, 2);
	initialResultsMap(&mapFiltered, 2);
	initialResultsMap(&mapTargetCountAndMax, 2);
	initialCells(cells, cellsCount);
	
	#pragma endregion

	#pragma region ͳһ�������ݿ�����Ҫ�ı�
	createDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength");//��ƽ���ź�ǿ�ȱ�avg_of_strength_Table
	createCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition");//����ѡ����related_table
	createCellSelectedResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult");//����С��ѡ���
	#pragma endregion

	#pragma region ����ͬһ���͵��ļ���һ��ָ�������д洢���� �Լ� ����ȡ��ָ�����龭��������ƽ���ź�ǿ�� �洢��ԭʼ�ļ����ݹ�ϣ����
	loadAllSamePrefixTxtFile(&dataSetList, &dataFileCount, data);//������ص���data���ݼ� ע��������ļ�����Ҫ�����ʱһ�� ��Ȼû������
	loadAllSamePrefixTxtFile(&ssOrPssSetList, &ssOrPssFileCount, DEFAULT_ENSURE_FILE_TYPE);//������ص���ssOrPss���ݼ�
	addDataToMap(&mapOriginal, dataSetList, data, dataFileCount);//������ص���data���ݼ� ע��������ļ�����Ҫ�ͼ���ʱһ�� ��Ȼû������ ���ﴫ���������ָ�� ���ڲ����м���� �洢��ƽ���ź�ǿ�ȼ��ļ���
	#pragma endregion

	#pragma region �˻����� ��ӡ��ʼƽ���ź�ǿ���ļ��� ��ӡ������ƽ���ź�ǿ�ȼ� ���������Ľ�������ݿ��� ��ӡ������ֵ��ƽ���ź�ǿ�ȼ� ��ӡ������ֵͬʱָ�������Ŀ���ļ���
	printf("δ����ǰ�����ݣ�\n\n");
	printMap(&mapOriginal);
	printf("---------------------------------\n\n");
	printf("���������� ��\n\n");
	sortMapByAvg(&mapOriginal);
	printMap(&mapOriginal);

	printf("---------------------------------\n\n");
	printf("��avg_of_strength_Table�в�������\n\n");
	insertOrderedDataFileInfoToAvgOfStrengthTable(&mapOriginal, connect);//��������data����������

	printf("---------------------------------\n\n");
	printf("����ָ����ֵҪ����ļ���(��ֵΪ��%lf)��\n\n", THRESHOLD);
	mapFiltered = *filterByThreshold(&mapOriginal, &mapFiltered, THRESHOLD);
	printMap(&mapFiltered);

	printf("---------------------------------\n\n");
	printf("����ָ����ֵͬʱָ���������ĿΪ%d���ļ�����\n\n", FILE_COUNTS);
	mapTargetCountAndMax = *targetCountsMaxAvgMap(&mapFiltered, &mapTargetCountAndMax, FILE_COUNTS);
	printMap(&mapTargetCountAndMax);

	printf("---------------------------------\n\n");
	#pragma endregion

	#pragma region ���� ���ݼ��Ƿ���ȷ��ȡ
	//for (int i = 0; i < fileCount; i++) {
	//	printList(dataSetList[i]);
	//}
	// �����Ƿ���ȷ��ȡ
	//printDataSet(dataSetList, data, fileCount);
	//printDataSet(ssOrPssSetList, ss, ssOrPssFileCount);
	// ���Լ�����
	//caclateMultipleFileAvgSignalStrength(dataSetList, fileCount, &res);
	//printAvgRes(res, fileCount);
	#pragma endregion

	#pragma region ���ڽ�����ؼ�����ͼ�¼���
	if (mapTargetCountAndMax.count == 0) {
		printf("û��������ֵƵ����Ϣ\n\n");
	} else {
		printf("\n");
		relatedTest(&mapTargetCountAndMax, ssOrPssSetList, dataFileCount, ssOrPssFileCount, &cells, &cellsCount, INITIAL_CAPACITY_CELLS, DEFAULT_ENSURE_FILE_TYPE);
		printf("��ѡ������%d\n", cellsCount);
		printf("��ѡ�����(δ����)��\n\n");
		printCells(cells, cellsCount);
		printf("\n");
		printf("��ѡ�����(������,���շ�ֵ��������)��\n\n");
		sortCellsBySingalStrength(cells);
		printf("---------------------------------\n\n");
		printCells(cells, cellsCount);
		printf("\n");
		printf("��related_table�в�������\n\n");
		printf("---------------------------------\n\n");
		insertCellsInfoToRelatedTable(cells, connect, cellsCount); //�����ѡ����Ϣ ����ط���Ϣ
		printf("---------------------------------\n\n");
	}
	//��������ļ���
	char* cellSelectedName = NULL;
	if (cells != NULL) {
		cellSelectedName = createResFileName(cells[cellsCount - 1].actualName, cells[cellsCount - 1].ensureName, DEFAULT_ENSURE_FILE_TYPE);
		printf("����ѡȡ��С�����Ϊ��%s\n\n", cellSelectedName);
		printf("---------------------------------\n\n");
		//��������ѡ���С�����
		printf("��cells_selected_table�в�������\n\n");
		insertCellResultInfoToCellsSelectedTable(cells[cellsCount - 1], connect, cellSelectedName);//��������ѡ���С�������Ϣ
		strcpy(resultCell, cellSelectedName); // ��ֵ
		printf("%s", resultCell);
	}
	#pragma endregion

	#pragma region ��ӡ���ݿ��еı���Ϣ
	//��ӡ�����ݿ��ȡ��������Ϣ
	printf("---------------------------------\n\n");
	printf("��ӡavg_of_strength_table��Ϣ\n\n");
	printTableInfo(connect, "avg_of_strength_table");//ƽ���ź�ǿ�ȱ�
	printf("---------------------------------\n\n");
	printf("��ӡrelated_table��Ϣ\n\n");
	printTableInfo(connect, "related_table");//����Ա�
	printf("---------------------------------\n\n");
	printf("��ӡcells_selected_table��Ϣ\n\n");
	printTableInfo(connect, "cells_selected_table");//С��ѡ���
	#pragma endregion

	#pragma region ͳһ�ͷ������ڴ�
	//ͳһ�ͷ��ڴ�
	mysql_close(connect);
	destroyMap(&mapOriginal);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountAndMax);
	//ͳһ�ͷ�dataSetList���ݼ�
	for (int i = 0; i < dataFileCount; i++) {
		destroyList(dataSetList[i]);//ÿ��������Ϣ
		free(dataSetList[i]);//ÿ��ͷ���
	}
	//ͳһ�ͷ�ssOrPssSetList���ݼ�
	for (int i = 0; i < ssOrPssFileCount; i++) {
		destroyList(ssOrPssSetList[i]);//ÿ��������Ϣ
		free(ssOrPssSetList[i]);//ÿ��ͷ���
	}
	//free(res);
	destroyCells(cells, cellsCount);
	free(dataSetList);
	free(ssOrPssSetList);
	free(cellSelectedName);
	#pragma endregion

	#pragma region  �ͷ���������ģ��
	initialNetBase();//��ʼ�������
	//�����ͻ����׽���
	SOCKET clientSocket;
	int ret = createClientSocketAndConnect(&clientSocket, "127.0.0.1", 8082);
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
	#pragma endregion


	return 0;
}

#pragma region ��������Ϣ����غ���
/*����ƽ���ź�ǿ�ȵ����ݼ�*/
void insertOrderedDataFileInfoToAvgOfStrengthTable(const ResultsMap* orderedDataFileMap, MYSQL* connect) {
	if (orderedDataFileMap == NULL || connect == NULL) {
		printf("���ݿ����Ӳ���ȷ����Ĺ�ϣ��Ϊ��\n");
		return;
	}
	printf("��ǰ�ı�Ϊ��%s\n","avg_of_strength_Table");
	printf("��ͷΪ��%s\t%s\t\n\n","dataFileName", "avgOfStrength");
	for (int i = 0; i < orderedDataFileMap->count; i++) {
		//orderedDataFileMap->fileResults[i].fileName;
		insertInfoToDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength",
			_strdup(orderedDataFileMap->fileResults[i].fileName), orderedDataFileMap->fileResults[i].res);
	}
	printf("\n");
}
/*�����ѡ����Ϣ*/
void insertCellsInfoToRelatedTable(const Cell* cells, MYSQL* connect, int cellsCount) {
	if (cells == NULL || connect == NULL) {
		printf("��ѡ��Ϊ�ջ����ݿ����Ӳ���ȷ\n");
		return;
	}
	printf("��ǰ�ı�Ϊ��%s\n", "related_table");
	printf("��ͷΪ��%s\t%s\t%s\t%s\t\n\n", "dataFileName", "ssOrPssFileName","mountainStrength", "mountainPosition");
	for (int i = 0; i < cellsCount; i++) {
		insertInfoToCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition",
			_strdup(cells[i].actualName), _strdup(cells[i].ensureName), cells[i].singalStrength, cells[i].maxIndex);
	}
	printf("\n");
}
/*�������ѡ���С����Ϣ*/
void insertCellResultInfoToCellsSelectedTable(const Cell cell, MYSQL* connect,const char* resFileName) {
	printf("��ǰ�ı�Ϊ��%s\n", "cells_selected_table");
	printf("��ͷΪ��%s\t%s\t%s\t%s\t%s\t\n\n", "selectedCellResult", "dataFileName", "avgOfStrength", "mountainStrength", "mountainPosition");
	insertInfoToCellResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult",
		_strdup(cell.actualName), _strdup(cell.ensureName), cell.singalStrength, cell.maxIndex, resFileName);
	printf("\n");
}
#pragma endregion

#pragma region ��ӡ��������
/*��ӡ�������ݼ���Ϣ*/
void printDataSet(const LinkedList** dataSetList, E_Type_File fileType, int fileCount) {
	if (fileCount == 0) return;
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(fileType, &fileName, i);
		printf("�ɹ����ļ���%s\t,������Ϊ��\n\n", fileName);
		printList(dataSetList[i]);
		free(fileName);
	}
}
/*��ӡƽ���ź�ǿ�����ݼ�*/
void printAvgRes(const double* res, int fileCount) {
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(data, &fileName, i);
		printf("��ǰ�ļ�����%s\t;����Ϊ%lf\n", fileName, res[i]);
		free(fileName);
	}
}
#pragma endregion

#pragma region ��ؼ������
/*���㵥���ļ���ƽ���ź�ǿ��*/
double caculateSingleFileAvgSignalStrength(const LinkedList* list, const char* fileName) {
	double res = 0, tempI = 0, tempQ = 0;
	if (list == NULL || fileName == NULL) {
		printf("�ļ�������ȷ�����ݲ�����\n");
		return -1;
	}
	Node* temp = list->head;
	while (temp) {
		tempI = temp->i;
		tempQ = temp->q;
		res += sqrt(tempI * tempI + tempQ * tempQ);
		temp = temp->next;
	}
	return res / list->size;
}
/*������ļ���һ�������ݼ���ƽ���ź�ǿ��*/
void caclateMultipleFileAvgSignalStrength(const LinkedList** dataSetList, int fileCount, double** resAvg) {
	*resAvg = malloc(sizeof(double) * fileCount);
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(data, &fileName, i);
		if (*resAvg == NULL) {
			printf("���ڴ洢�Ľ����������ڴ�ʧ�ܣ����������ָ���Ƿ���Ч\n");
		} else {
			(*resAvg)[i] = caculateSingleFileAvgSignalStrength(dataSetList[i], fileName);
		}
		free(fileName);
	}
}
/*��ؼ���ĺ���*/
void caculateRelated(const LinkedList* dataList, const LinkedList* ssOrPssList, double** res) {
	if (dataList == NULL || ssOrPssList == NULL) {
		printf("��������ݼ�Ϊ��\n");
		return;
	}
	int maxIndex = 0;
	double imaginary = 0, real = 0;
	double curRes = 0, maxRes = 0;

	Node* dataCurNode = dataList->tail;
	int windowCount = dataList->size - ssOrPssList->size + 1;
	if (windowCount <= 0) {
		printf("���ڳ��Ȳ��㣬�޷����������\n");
		return;
		//*res = malloc(sizeof(double) * 2);
		//if (res == NULL) {
		//	printf("Ϊ�洢����������ʱ�ڴ�ʧ��\n");
		//	return;
		//} else {
		//	(*res)[0] = 0;
		//	(*res)[1] = -1;
		//	return;
		//}
	}
	for (int i = 0; i < windowCount; i++) {
		imaginary = 0; real = 0;//ÿ�ƶ�һ������ �ͽ�������һ��
		curRes = 0;
		Node* ssOrPssCurNode = ssOrPssList->tail;//��ʼ��������ssOrPss�ڵ� �൱��ȷ���źŽṹ�������еĵ�һ��
		Node* dataCaculateCurNode = dataCurNode; //���л�����ʵ�ʴ��� ֻ��ÿ�ζ�ӦssOrPssCurNode������֮��Ż��ƶ�����һ��
		if (dataCurNode == NULL) break;
		while (dataCaculateCurNode && ssOrPssCurNode) {//��ؼ���
			real +=		 ssOrPssCurNode->i * dataCaculateCurNode->i +
						 ssOrPssCurNode->q * dataCaculateCurNode->q;
			imaginary += ssOrPssCurNode->q * dataCaculateCurNode->i -
						 ssOrPssCurNode->i * dataCaculateCurNode->q;
				
			ssOrPssCurNode = ssOrPssCurNode->pre;
			dataCaculateCurNode = dataCaculateCurNode->pre;
		}
		curRes = sqrt(real * real + imaginary * imaginary);
		if (curRes > maxRes) {
			maxRes = curRes;
			maxIndex = dataCurNode->id;
		}
		dataCurNode = dataCurNode->pre;//ÿ����ǰ�ƶ�һ��
	}
	double* temp = malloc(sizeof(double) * 2);
	if (temp == NULL) {
		printf("��ʱ����Ľ�������ڴ����ʧ��\n");
		return;
	} else {
		*res = temp;
		(*res)[0] = maxRes;
		(*res)[1] = maxIndex;
	}
	//*res = malloc(sizeof(double)*2);
	return;
}
/*����Ĳ���Ϊ ���˺�ָ����Ŀ��ʵ���ļ�����data�ļ� ȷ���źż���SSOrPSS�ļ� �Լ��������ļ�����Ŀ ����һ�����ڼ�¼�Ľṹ������*/
void relatedTest(const ResultsMap* mapFilterd, const LinkedList** ssOrPssSetList, int dataFilteredFileCount, int ssOrPssFileCount, Cell** cells, int* cellsCount, int capacity,E_Type_File fileType) {
	int index = 0;
	if (capacity == 0) {
		printf("��ʼ��������Ϊ0!!!!!��Ĭ������Ϊ1\n");
		capacity = 1;
	}
	for (int i = 0; i < mapFilterd->count; i++) {
		LinkedList* dataList = malloc(sizeof(LinkedList));
		initialLinkedList(dataList);
		loadDataFromTxt(dataList, mapFilterd->fileResults[i].fileName);//һ��list����ָ����һ��data����
		for (int j = 0; j < ssOrPssFileCount; j++) {
			double* res = NULL;		//������������ ��һ�������Ƿ�ֵ �ڶ��������Ƿ�ֵλ��
			char* fileName = NULL;
			//ssOrPssSetList[j];

			caculateRelated(dataList, ssOrPssSetList[j], &res);
			createFileName(fileType, &fileName, j);
			if (index >= capacity) {
				capacity *= 2;
				Cell* temp = realloc(*cells, sizeof(Cell) * capacity);
				if (temp == NULL) {
					printf("С����������ʧ��\n");
					return;
				} else {
					*cells = temp;
				}
			}
			if (*cells == NULL) {
				printf("���ڴ洢��С���ṹ�������ڴ������Ч\n");
			} else {
				if (res == NULL) {
					printf("��ʱ��������ʧ��������ʱ�������\n");
					return;
				} else {
					(*cells)[index].singalStrength = res[0];
					(*cells)[index].maxIndex = (int)res[1];
					(*cells)[index].ensureName = _strdup(fileName);
					(*cells)[index].actualName = _strdup(mapFilterd->fileResults[i].fileName);
					index++;
				}
			}
			free(fileName);
			free(res);

		}
		free(dataList);
	}
	Cell* temp =realloc(*cells, sizeof(Cell) * index);
	if (temp == NULL) {
		printf("С��������������ʧ�ܣ�������ԭʼ��������\n");
	} else {
		//������� ����������ǵ���������
		*cells = temp;
		//*cells = realloc(*cells, sizeof(Cell) * index);
	}
	*cellsCount = index;
}

#pragma endregion

/*�����ݼ�����map��*/
void addDataToMap(ResultsMap* resMap, const LinkedList** dataSetList, E_Type_File fileType, int fileCount) {
	double* resAvg = NULL;
	caclateMultipleFileAvgSignalStrength(dataSetList, fileCount, &resAvg);
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(fileType, &fileName, i);
		addNewFileResult(resMap, fileName, resAvg[i]);
		free(fileName);
	}
	free(resAvg);
}

