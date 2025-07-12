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

	#pragma region 测试用
	//int count = 0;
	//Complex* complexs = readSSTxtFile("SS0.txt",&count);
	//printf("%d", count);
	//printfSSComplexs(complexs, count);
	// 测试随机数组创建
	//double* temp = { 0 };
	//temp = createRandomArray(5, 5);
	//printfArr(temp, 5);
	//free(temp);
	// 
	//测试随机创建的确定信号
	//Complex* complexs = malloc(10 * sizeof(Complex));
	//createOneSureSignalSequence(complexs, 20, 5);
	//printfSSComplexs(complexs,10);
	//free(complexs);
	#pragma endregion

	#pragma region 主流程

	ResultsMap map;							//原始数据集
	ResultsMap mapFiltered;					//筛选后
	ResultsMap mapTargetCountsMaxAvg;		//指定最大数目的文件集
	Cell* cells = malloc(12 * sizeof(Cell));	//筛选后的九个预选集

	initialResultsMap(&map, 2);
	initialResultsMap(&mapFiltered, 5);
	initialResultsMap(&mapTargetCountsMaxAvg, 5);
	initialCells(cells);

	dataToMap(&map, FILE_COUNTS);//将数据写入哈希表中
	//dataToMapByStraight(&map);//数据集少的

	printf("未排序前的数据集：\n");
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("排序后的数据集（按平均信号强度升序排序）：\n");
	sortMapByAvg(&map);
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("满足阈值的平均信号强度文件集：当前阈值为%lf。\n", THRESHOLD);
	mapFiltered = *filterByThreshold(&map, &mapFiltered, THRESHOLD);
	printMap(&mapFiltered);
	printf("--------------------------------------------------\n");

	printf("指定数目为：%d， 满足阈值的平均信号强度最大的文件集：\n", MAX_COUNTS_OF_THRESHOLD);
	mapTargetCountsMaxAvg = *targetCountsMaxAvgMap(&mapFiltered, &mapTargetCountsMaxAvg, MAX_COUNTS_OF_THRESHOLD);
	printMap(&mapTargetCountsMaxAvg);
	printf("--------------------------------------------------\n");

	printf("下面进行相关检测\n");
	/*寻找最大组合*/
	if(mapTargetCountsMaxAvg.count == 0){
		printf("无合适小区适合接入\n");
	}else{
		findMaxMoutainPosition(&mapTargetCountsMaxAvg, cells);
		//findMaxMoutainPositionByStraight(&mapTargetCountsMaxAvg, cells);
	}
	printf("--------------------------------------------------\n");
	printf("排序前的各种组合：\n\n");
	printCells(cells);
	printf("--------------------------------------------------\n");
	printf("排序后的各种组合（以最强平均信号强度进行排序）\n\n");
	sortCellsBySingalStrength(cells);
	printCells(cells);

	char* resFileName = createResFileName(cells[8].actualName,cells[8].ensureName);
	printf("最大小区为: %s \n", resFileName);
	#pragma endregion

	#pragma region 统一释放申请的内存
	destroyMap(&map);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountsMaxAvg);

	destroyCells(cells);
	free(resFileName);
	#pragma endregion

	return 0;
}



/*随机生成一个double数组*/
double* createRandomArray(int maxSize, int maxValue) {
	double* arr = malloc(sizeof(double) * maxSize);
	if (arr == NULL) {
		printf("数组初始化分配内存失败\n");
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

/*外部使用时传入一个结构体的指针 同时务必确保传入的maxSize为偶数*/
void createOneSureSignalSequence(Complex* resSequence, int maxSize, int maxValue) {
	double* temp = createRandomArray(maxSize, maxValue);
	for (int i = 0; i < maxSize / 2; i++) {
		resSequence[i].i = temp[2 * i];		//偶数做i路信号
		resSequence[i].q = temp[2 * i + 1]; //奇数做q路信号
	}
}

/*设置字体颜色*/
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

