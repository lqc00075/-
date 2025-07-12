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
//平均信号强度指定最大文件数目
#define FILE_COUNTS 3
#define INITIAL_CAPACITY_CELLS 1
#define DEFAULT_ENSURE_FILE_TYPE pss

#pragma region 函数声明
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

	#pragma region 测试 文件是否读取无误 计算平均信号强度是否正确
	//LinkedList* list = malloc(sizeof(LinkedList));
	//initialLinkedList(list);
	//测试加载数据
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

	#pragma region 初始化 初始化MySql连接 数据集的建立 即指针数组初始化 以及记录哈希表的初始化 还有最终存储候选集的初始化

	

	MYSQL* connect = NULL;
	initialMySqlAndConnectDatabase(&connect, "127.0.0.1", "root", "18598553608lqC", "mysql", 0, NULL, 0);
	dropDataBase(connect, "lqc00075");
	createDataBase(connect, "lqc00075");
	changeDataBase(connect, "lqc00075");
	int dataFileCount = 0;
	int ssOrPssFileCount = 0;
	int cellsCount = 0;
	//double* res = NULL;
	LinkedList** dataSetList = NULL;							//data数据集
	LinkedList** ssOrPssSetList = NULL;							//ssOrPss数据集
	ResultsMap mapOriginal;										//初始数据集 存储所有的文件名及结果
	ResultsMap mapFiltered;										//筛选满足阈值的数据集
	ResultsMap mapTargetCountAndMax;							//筛选满足阈值同时满足数目的数据集
	Cell* cells = malloc(sizeof(Cell) * INITIAL_CAPACITY_CELLS);//这里设置的动态内存是为了代码的扩展性 如果以后会使用非常非常大的数据 这里就可以扩容

	initialResultsMap(&mapOriginal, 2);
	initialResultsMap(&mapFiltered, 2);
	initialResultsMap(&mapTargetCountAndMax, 2);
	initialCells(cells, cellsCount);
	
	#pragma endregion

	#pragma region 统一创建数据库中需要的表
	createDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength");//创平均信号强度表即avg_of_strength_Table
	createCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition");//创候选集表即related_table
	createCellSelectedResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult");//创建小区选择表
	#pragma endregion

	#pragma region 加载同一类型的文件至一个指针数组中存储备用 以及 将获取的指针数组经过计算后的平均信号强度 存储至原始文件数据哈希表中
	loadAllSamePrefixTxtFile(&dataSetList, &dataFileCount, data);//这里加载的是data数据集 注意这里的文件类型要和添加时一致 不然没有意义
	loadAllSamePrefixTxtFile(&ssOrPssSetList, &ssOrPssFileCount, DEFAULT_ENSURE_FILE_TYPE);//这里加载的是ssOrPss数据集
	addDataToMap(&mapOriginal, dataSetList, data, dataFileCount);//这里加载的是data数据集 注意这里的文件类型要和加载时一致 不然没有意义 这里传入的是数组指针 在内部进行计算后 存储的平均信号强度及文件名
	#pragma endregion

	#pragma region 人机交互 打印初始平均信号强度文件集 打印排序后的平均信号强度集 插入排序后的结果至数据库中 打印满足阈值的平均信号强度集 打印满足阈值同时指定最大数目的文件集
	printf("未排序前的数据：\n\n");
	printMap(&mapOriginal);
	printf("---------------------------------\n\n");
	printf("排序后的数据 ：\n\n");
	sortMapByAvg(&mapOriginal);
	printMap(&mapOriginal);

	printf("---------------------------------\n\n");
	printf("向avg_of_strength_Table中插入数据\n\n");
	insertOrderedDataFileInfoToAvgOfStrengthTable(&mapOriginal, connect);//插入数据data排序后的数据

	printf("---------------------------------\n\n");
	printf("满足指定阈值要求的文件集(阈值为：%lf)：\n\n", THRESHOLD);
	mapFiltered = *filterByThreshold(&mapOriginal, &mapFiltered, THRESHOLD);
	printMap(&mapFiltered);

	printf("---------------------------------\n\n");
	printf("满足指定阈值同时指定的最大数目为%d的文件集：\n\n", FILE_COUNTS);
	mapTargetCountAndMax = *targetCountsMaxAvgMap(&mapFiltered, &mapTargetCountAndMax, FILE_COUNTS);
	printMap(&mapTargetCountAndMax);

	printf("---------------------------------\n\n");
	#pragma endregion

	#pragma region 测试 数据集是否正确读取
	//for (int i = 0; i < fileCount; i++) {
	//	printList(dataSetList[i]);
	//}
	// 测试是否正确读取
	//printDataSet(dataSetList, data, fileCount);
	//printDataSet(ssOrPssSetList, ss, ssOrPssFileCount);
	// 测试计算结果
	//caclateMultipleFileAvgSignalStrength(dataSetList, fileCount, &res);
	//printAvgRes(res, fileCount);
	#pragma endregion

	#pragma region 用于进行相关检测计算和记录结果
	if (mapTargetCountAndMax.count == 0) {
		printf("没有满足阈值频点信息\n\n");
	} else {
		printf("\n");
		relatedTest(&mapTargetCountAndMax, ssOrPssSetList, dataFileCount, ssOrPssFileCount, &cells, &cellsCount, INITIAL_CAPACITY_CELLS, DEFAULT_ENSURE_FILE_TYPE);
		printf("候选集个数%d\n", cellsCount);
		printf("候选集组合(未排序)：\n\n");
		printCells(cells, cellsCount);
		printf("\n");
		printf("候选集组合(已排序,按照峰值进行排序)：\n\n");
		sortCellsBySingalStrength(cells);
		printf("---------------------------------\n\n");
		printCells(cells, cellsCount);
		printf("\n");
		printf("向related_table中插入数据\n\n");
		printf("---------------------------------\n\n");
		insertCellsInfoToRelatedTable(cells, connect, cellsCount); //插入候选集信息 即相关峰信息
		printf("---------------------------------\n\n");
	}
	//创建结果文件名
	char* cellSelectedName = NULL;
	if (cells != NULL) {
		cellSelectedName = createResFileName(cells[cellsCount - 1].actualName, cells[cellsCount - 1].ensureName, DEFAULT_ENSURE_FILE_TYPE);
		printf("最终选取的小区组合为：%s\n\n", cellSelectedName);
		printf("---------------------------------\n\n");
		//插入最终选择的小区结果
		printf("向cells_selected_table中插入数据\n\n");
		insertCellResultInfoToCellsSelectedTable(cells[cellsCount - 1], connect, cellSelectedName);//插入最终选择的小区结果信息
		strcpy(resultCell, cellSelectedName); // 赋值
		printf("%s", resultCell);
	}
	#pragma endregion

	#pragma region 打印数据库中的表信息
	//打印从数据库读取的内容信息
	printf("---------------------------------\n\n");
	printf("打印avg_of_strength_table信息\n\n");
	printTableInfo(connect, "avg_of_strength_table");//平均信号强度表
	printf("---------------------------------\n\n");
	printf("打印related_table信息\n\n");
	printTableInfo(connect, "related_table");//相关性表
	printf("---------------------------------\n\n");
	printf("打印cells_selected_table信息\n\n");
	printTableInfo(connect, "cells_selected_table");//小区选择表
	#pragma endregion

	#pragma region 统一释放申请内存
	//统一释放内存
	mysql_close(connect);
	destroyMap(&mapOriginal);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountAndMax);
	//统一释放dataSetList数据集
	for (int i = 0; i < dataFileCount; i++) {
		destroyList(dataSetList[i]);//每个链表信息
		free(dataSetList[i]);//每个头结点
	}
	//统一释放ssOrPssSetList数据集
	for (int i = 0; i < ssOrPssFileCount; i++) {
		destroyList(ssOrPssSetList[i]);//每个链表信息
		free(ssOrPssSetList[i]);//每个头结点
	}
	//free(res);
	destroyCells(cells, cellsCount);
	free(dataSetList);
	free(ssOrPssSetList);
	free(cellSelectedName);
	#pragma endregion

	#pragma region  和服务器交互模块
	initialNetBase();//初始化网络库
	//创建客户端套接字
	SOCKET clientSocket;
	int ret = createClientSocketAndConnect(&clientSocket, "127.0.0.1", 8082);
	if (ret != 0) {
		printf("与服务器连接失败\n");
		return -1;
	}
	HANDLE sendInfoThread = CreateThread(NULL, 0, sendInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (sendInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}
	HANDLE recvInfoThread = CreateThread(NULL, 0, receiveInfoThreadFunc, (LPVOID)clientSocket, 0, NULL);
	if (recvInfoThread == NULL) {
		printf("sendInfoThread创建失败\n");
		return -1;
	}
	WaitForSingleObject(sendInfoThread, INFINITE);
	WaitForSingleObject(recvInfoThread, INFINITE);
	closesocket(clientSocket);
	WSACleanup();
	#pragma endregion


	return 0;
}

#pragma region 向表插入信息的相关函数
/*插入平均信号强度的数据集*/
void insertOrderedDataFileInfoToAvgOfStrengthTable(const ResultsMap* orderedDataFileMap, MYSQL* connect) {
	if (orderedDataFileMap == NULL || connect == NULL) {
		printf("数据库连接不正确或传入的哈希表为空\n");
		return;
	}
	printf("当前的表为：%s\n","avg_of_strength_Table");
	printf("表头为：%s\t%s\t\n\n","dataFileName", "avgOfStrength");
	for (int i = 0; i < orderedDataFileMap->count; i++) {
		//orderedDataFileMap->fileResults[i].fileName;
		insertInfoToDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength",
			_strdup(orderedDataFileMap->fileResults[i].fileName), orderedDataFileMap->fileResults[i].res);
	}
	printf("\n");
}
/*插入候选集信息*/
void insertCellsInfoToRelatedTable(const Cell* cells, MYSQL* connect, int cellsCount) {
	if (cells == NULL || connect == NULL) {
		printf("候选集为空或数据库连接不正确\n");
		return;
	}
	printf("当前的表为：%s\n", "related_table");
	printf("表头为：%s\t%s\t%s\t%s\t\n\n", "dataFileName", "ssOrPssFileName","mountainStrength", "mountainPosition");
	for (int i = 0; i < cellsCount; i++) {
		insertInfoToCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition",
			_strdup(cells[i].actualName), _strdup(cells[i].ensureName), cells[i].singalStrength, cells[i].maxIndex);
	}
	printf("\n");
}
/*插入最后选择的小区信息*/
void insertCellResultInfoToCellsSelectedTable(const Cell cell, MYSQL* connect,const char* resFileName) {
	printf("当前的表为：%s\n", "cells_selected_table");
	printf("表头为：%s\t%s\t%s\t%s\t%s\t\n\n", "selectedCellResult", "dataFileName", "avgOfStrength", "mountainStrength", "mountainPosition");
	insertInfoToCellResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult",
		_strdup(cell.actualName), _strdup(cell.ensureName), cell.singalStrength, cell.maxIndex, resFileName);
	printf("\n");
}
#pragma endregion

#pragma region 打印辅助函数
/*打印整个数据集信息*/
void printDataSet(const LinkedList** dataSetList, E_Type_File fileType, int fileCount) {
	if (fileCount == 0) return;
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(fileType, &fileName, i);
		printf("成功打开文件：%s\t,其数据为：\n\n", fileName);
		printList(dataSetList[i]);
		free(fileName);
	}
}
/*打印平均信号强度数据集*/
void printAvgRes(const double* res, int fileCount) {
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(data, &fileName, i);
		printf("当前文件名：%s\t;数据为%lf\n", fileName, res[i]);
		free(fileName);
	}
}
#pragma endregion

#pragma region 相关计算过程
/*计算单个文件的平均信号强度*/
double caculateSingleFileAvgSignalStrength(const LinkedList* list, const char* fileName) {
	double res = 0, tempI = 0, tempQ = 0;
	if (list == NULL || fileName == NULL) {
		printf("文件名不正确或数据不存在\n");
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
/*处理多文件即一整个数据集的平均信号强度*/
void caclateMultipleFileAvgSignalStrength(const LinkedList** dataSetList, int fileCount, double** resAvg) {
	*resAvg = malloc(sizeof(double) * fileCount);
	for (int i = 0; i < fileCount; i++) {
		char* fileName = NULL;
		createFileName(data, &fileName, i);
		if (*resAvg == NULL) {
			printf("用于存储的结果数组分配内存失败，请检查这里的指针是否有效\n");
		} else {
			(*resAvg)[i] = caculateSingleFileAvgSignalStrength(dataSetList[i], fileName);
		}
		free(fileName);
	}
}
/*相关计算的核心*/
void caculateRelated(const LinkedList* dataList, const LinkedList* ssOrPssList, double** res) {
	if (dataList == NULL || ssOrPssList == NULL) {
		printf("传入的数据集为空\n");
		return;
	}
	int maxIndex = 0;
	double imaginary = 0, real = 0;
	double curRes = 0, maxRes = 0;

	Node* dataCurNode = dataList->tail;
	int windowCount = dataList->size - ssOrPssList->size + 1;
	if (windowCount <= 0) {
		printf("窗口长度不足，无法计算相关性\n");
		return;
		//*res = malloc(sizeof(double) * 2);
		//if (res == NULL) {
		//	printf("为存储结果分配的临时内存失败\n");
		//	return;
		//} else {
		//	(*res)[0] = 0;
		//	(*res)[1] = -1;
		//	return;
		//}
	}
	for (int i = 0; i < windowCount; i++) {
		imaginary = 0; real = 0;//每移动一个窗口 就进行重置一次
		curRes = 0;
		Node* ssOrPssCurNode = ssOrPssList->tail;//初始参与计算的ssOrPss节点 相当于确定信号结构体数组中的第一个
		Node* dataCaculateCurNode = dataCurNode; //进行滑动的实际窗口 只有每次对应ssOrPssCurNode滑动完之后才会移动到下一个
		if (dataCurNode == NULL) break;
		while (dataCaculateCurNode && ssOrPssCurNode) {//相关计算
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
		dataCurNode = dataCurNode->pre;//每次往前移动一个
	}
	double* temp = malloc(sizeof(double) * 2);
	if (temp == NULL) {
		printf("临时分配的结果数组内存分配失败\n");
		return;
	} else {
		*res = temp;
		(*res)[0] = maxRes;
		(*res)[1] = maxIndex;
	}
	//*res = malloc(sizeof(double)*2);
	return;
}
/*传入的参数为 过滤后指定数目的实际文件集即data文件 确定信号集即SSOrPSS文件 以及这两个文件的数目 还有一个用于记录的结构体数组*/
void relatedTest(const ResultsMap* mapFilterd, const LinkedList** ssOrPssSetList, int dataFilteredFileCount, int ssOrPssFileCount, Cell** cells, int* cellsCount, int capacity,E_Type_File fileType) {
	int index = 0;
	if (capacity == 0) {
		printf("初始容量不能为0!!!!!以默认容量为1\n");
		capacity = 1;
	}
	for (int i = 0; i < mapFilterd->count; i++) {
		LinkedList* dataList = malloc(sizeof(LinkedList));
		initialLinkedList(dataList);
		loadDataFromTxt(dataList, mapFilterd->fileResults[i].fileName);//一个list就是指定的一组data数据
		for (int j = 0; j < ssOrPssFileCount; j++) {
			double* res = NULL;		//会有两个数据 第一个数据是峰值 第二个数据是峰值位置
			char* fileName = NULL;
			//ssOrPssSetList[j];

			caculateRelated(dataList, ssOrPssSetList[j], &res);
			createFileName(fileType, &fileName, j);
			if (index >= capacity) {
				capacity *= 2;
				Cell* temp = realloc(*cells, sizeof(Cell) * capacity);
				if (temp == NULL) {
					printf("小区数组扩容失败\n");
					return;
				} else {
					*cells = temp;
				}
			}
			if (*cells == NULL) {
				printf("用于存储的小区结构体数组内存分配无效\n");
			} else {
				if (res == NULL) {
					printf("临时分配数组失败请检查临时结果数组\n");
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
		printf("小区数据数组缩容失败，将保留原始容量数组\n");
	} else {
		//做个标记 哪里有问题记得来看这里
		*cells = temp;
		//*cells = realloc(*cells, sizeof(Cell) * index);
	}
	*cellsCount = index;
}

#pragma endregion

/*将数据加入至map中*/
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

