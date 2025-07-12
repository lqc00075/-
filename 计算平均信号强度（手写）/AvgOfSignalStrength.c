#include "CaculateMultipleFileRes.h"
#include "Map.h"
#include"ShaiXuanFitFiles.h"
#define FILE_COUNTS 7
void bubbleSort(int arr[], int len);

int main() {

	//fscanfFuncToLoad("data0.txt");
	// 测试是否正确读取
	//int count = 0;
	//Complex* complexs = readTxtFile("data0.txt", &count);
	//for (int i = 0; i < count; i++) {
	//	printf("第%d组数据：I数据：%lf,Q数据：%lf\n",i, complexs[i].i, complexs[i].q);
	//}
	// 测试计算结果是否正确返回
	//double ans = caculateAvgOfSingle(complexs, count);
	//printf("%lf", ans);
	// 拼接名字
	//char* testFileName = createFileName(2);
	//printf("%s", testFileName);
	// 多文件操作
	//double* res = allFilesAvgOfSingle(7);
	//for (int i = 0; i < 7; i++) {
	//	printf("%lf\n", res[i]);
	//}
	ResultsMap map;						//原始数据集
	ResultsMap mapFiltered;				//筛选后
	ResultsMap mapTargetCountsMaxAvg;	//指定最大数目的文件集
	initialResultsMap(&map,2);
	initialResultsMap(&mapFiltered, 5);
	initialResultsMap(&mapTargetCountsMaxAvg, 5);
	//addNewFileResult(&map, "data0.txt", 1605.84);
	//addNewFileResult(&map, "data1.txt", 697.32);
	//for (int i = 0; i < map.count; i++) {
	//	printf("%s：%lf\n", map.fileResults[i].fileName, map.fileResults[i].res);
	//}
	dataToMap(&map, FILE_COUNTS);
	printf("未排序前的数据集：\n");
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("排序后的数据集（按平均信号强度升序排序）：\n");
	sortMapByAvg(&map);
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("满足阈值的平均信号强度文件集：当前阈值为%lf。\n",THRESHOLD);
	mapFiltered = *filterByThreshold(&map,&mapFiltered,THRESHOLD);
	printMap(&mapFiltered);
	printf("--------------------------------------------------\n");

	printf("指定数目为：%d， 满足阈值的平均信号强度最大的文件集：\n", MAX_COUNTS_OF_THRESHOLD);
	mapTargetCountsMaxAvg = *targetCountsMaxAvgMap(&mapFiltered,&mapTargetCountsMaxAvg, MAX_COUNTS_OF_THRESHOLD);
	printMap(&mapTargetCountsMaxAvg);
	printf("--------------------------------------------------\n");
	//测试冒泡
	//int arr[6] = { 1,2,5,11,1,2 };
	//bubbleSort(arr, 6);
	//for (int i = 0; i < 6; i++) {
	//	printf("%d ", arr[i]);
	//}
	destroyMap(&map);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountsMaxAvg);
	return 0;
}


/*测试冒泡排序*/
void bubbleSort(int arr[], int len) {
	for (int i = len - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}