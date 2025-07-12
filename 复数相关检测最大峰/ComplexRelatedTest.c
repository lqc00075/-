#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include "CaculateMultipleFileRes.h"
#include "Map.h"
#include "ShaiXuanFitFiles.h"
#include "CellFunc.h"
#include "createFileNameFunc.h"
#include "printFunc.h"
#include "findMaxMountainPositionFunc.h"
#define  FILE_COUNTS 48
#define  INITIAL_CAPACITY 10
#define  RANDOM 1.3456218

void setColor(int color);
void createOneSureSignalSequence(Complex* resSequence, int maxSize, int maxValue);
double* createRandomArray(int maxSize, int maxValue);


int main() {

	#pragma region ������
	//int count = 0;
	//Complex* complexs = readSSTxtFile("SS0.txt",&count);
	//printf("%d", count);
	//printfSSComplexs(complexs, count);
	// ����������鴴��
	//double* temp = { 0 };
	//temp = createRandomArray(5, 5);
	//printfArr(temp, 5);
	//free(temp);
	// 
	//�������������ȷ���ź�
	//Complex* complexs = malloc(10 * sizeof(Complex));
	//createOneSureSignalSequence(complexs, 20, 5);
	//printfSSComplexs(complexs,10);
	//free(complexs);
	#pragma endregion

	#pragma region ������

	ResultsMap map;							//ԭʼ���ݼ�
	ResultsMap mapFiltered;					//ɸѡ��
	ResultsMap mapTargetCountsMaxAvg;		//ָ�������Ŀ���ļ���
	Cell* cells = malloc(12 * sizeof(Cell));	//ɸѡ��ľŸ�Ԥѡ��

	initialResultsMap(&map, 2);
	initialResultsMap(&mapFiltered, 5);
	initialResultsMap(&mapTargetCountsMaxAvg, 5);
	initialCells(cells);

	dataToMap(&map, FILE_COUNTS);//������д���ϣ����
	//dataToMapByStraight(&map);//���ݼ��ٵ�

	printf("δ����ǰ�����ݼ���\n");
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("���������ݼ�����ƽ���ź�ǿ���������򣩣�\n");
	sortMapByAvg(&map);
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("������ֵ��ƽ���ź�ǿ���ļ�������ǰ��ֵΪ%lf��\n", THRESHOLD);
	mapFiltered = *filterByThreshold(&map, &mapFiltered, THRESHOLD);
	printMap(&mapFiltered);
	printf("--------------------------------------------------\n");

	printf("ָ����ĿΪ��%d�� ������ֵ��ƽ���ź�ǿ�������ļ�����\n", MAX_COUNTS_OF_THRESHOLD);
	mapTargetCountsMaxAvg = *targetCountsMaxAvgMap(&mapFiltered, &mapTargetCountsMaxAvg, MAX_COUNTS_OF_THRESHOLD);
	printMap(&mapTargetCountsMaxAvg);
	printf("--------------------------------------------------\n");

	printf("���������ؼ��\n");
	/*Ѱ��������*/
	if(mapTargetCountsMaxAvg.count == 0){
		printf("�޺���С���ʺϽ���\n");
	}else{
		findMaxMoutainPosition(&mapTargetCountsMaxAvg, cells);
		//findMaxMoutainPositionByStraight(&mapTargetCountsMaxAvg, cells);
	}
	printf("--------------------------------------------------\n");
	printf("����ǰ�ĸ�����ϣ�\n\n");
	printCells(cells);
	printf("--------------------------------------------------\n");
	printf("�����ĸ�����ϣ�����ǿƽ���ź�ǿ�Ƚ�������\n\n");
	sortCellsBySingalStrength(cells);
	printCells(cells);

	char* resFileName = createResFileName(cells[8].actualName,cells[8].ensureName);
	printf("���С��Ϊ: %s \n", resFileName);
	#pragma endregion

	#pragma region ͳһ�ͷ�������ڴ�
	destroyMap(&map);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountsMaxAvg);

	destroyCells(cells);
	free(resFileName);
	#pragma endregion

	return 0;
}



/*�������һ��double����*/
double* createRandomArray(int maxSize, int maxValue) {
	double* arr = malloc(sizeof(double) * maxSize);
	if (arr == NULL) {
		printf("�����ʼ�������ڴ�ʧ��\n");
		return NULL;
	}
	srand(time(NULL));
	int value = 0;
	for (int i = 0; i < maxSize; i++) {
		value = (rand() % maxValue + 1) % 7;
		arr[i] = (rand() % 2) == 0 ? value * RANDOM : value * RANDOM * -1;
	}
	return arr;
}

/*�ⲿʹ��ʱ����һ���ṹ���ָ�� ͬʱ���ȷ�������maxSizeΪż��*/
void createOneSureSignalSequence(Complex* resSequence, int maxSize, int maxValue) {
	double* temp = createRandomArray(maxSize, maxValue);
	for (int i = 0; i < maxSize / 2; i++) {
		resSequence[i].i = temp[2 * i];		//ż����i·�ź�
		resSequence[i].q = temp[2 * i + 1]; //������q·�ź�
	}
}

/*����������ɫ*/
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

