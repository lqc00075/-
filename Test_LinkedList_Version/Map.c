#include "Map.h"
/*初始化一个Map*/
void initialResultsMap(ResultsMap* map,int capacity) {
	map->count = 0;
	map->capacity = capacity;
	map->fileResults = malloc(capacity * sizeof(Result));
}
/*添加一个新的文件数据进入map*/
int addNewFileResult(ResultsMap* map,const char* fileName,double value) {
	if (getValueOfFileName(map, fileName) == -1) {
		if (map->count >= map->capacity) {
			map->capacity *= 2;
			Result* temp = realloc(map->fileResults, map->capacity * sizeof(Result));
			if (temp == NULL) {
				printf("map扩容失败\n");
				return -1;
			} else {
				map->fileResults = temp;
			}
			//map->fileResults = realloc(map->fileResults, map->capacity * sizeof(Result));
		}
		if (fileName == NULL) {
			printf("请检查传入的文件名，文件名为空\n");
			return -1;
		} else {
			map->fileResults[map->count].fileName = _strdup(fileName);//注意这里 是重新在堆上开辟的一段新内存
			map->fileResults[map->count].res = value;
			map->count++;
		}
		return 0;
	}
	return -1;
}
/*获取指定文件名的结果值 顺便判断是否存在该文件名*/
double getValueOfFileName(ResultsMap* map,const char* fileName) {
	for (int i = 0; i < map->count; i++) {
		if (strcmp(map->fileResults[i].fileName,fileName)==0) {//字典序比较
			return map->fileResults[i].res;
		}
	}
	return -1;
}
/*清理！释放内存*/
void destroyMap(ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		free(map->fileResults[i].fileName);
	}
	free(map->fileResults);
	map->capacity = 0;
	map->count = 0;
}
/*辅助函数 打印出来看看 */
void printMap(const ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		printf("文件名：%s\t; 信号强度平均值：%lf \n", map->fileResults[i].fileName,
												map->fileResults[i].res);
	}
}
/*排序*/
void sortMapByAvg(ResultsMap* map) {
	int count = map->count;
	for (int i = count - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (map->fileResults[j].res > map->fileResults[j + 1].res) {
				swap(map, j, j + 1);
			}
		}
	}
}
/*交换指定位置的两个结构体*/
void swap(ResultsMap* map, int i, int j) {
	Result temp = map->fileResults[i];
	map->fileResults[i].fileName = _strdup(map->fileResults[j].fileName);
	map->fileResults[i].res = map->fileResults[j].res;
	map->fileResults[j].fileName = _strdup(temp.fileName);
	map->fileResults[j].res = temp.res;
}
/*获取指定最大阈值的指定文件集*/
ResultsMap* targetCountsMaxAvgMap(const ResultsMap* originalMap, ResultsMap* resMap, int targetCounts) {
	int count = 0;
	for (int i = originalMap->count - 1; i >= 0; i--) {
		addNewFileResult(resMap, originalMap->fileResults[i].fileName, originalMap->fileResults[i].res);
		count++;
		if (count >= targetCounts) {
			break;
		}
	}
	return resMap;
}
/*筛选满足阈值要求的所有文件集*/
ResultsMap* filterByThreshold(const ResultsMap* map, ResultsMap* resMap, double threshold) {
	for (int i = 0; i < map->count; i++) {
		if (map->fileResults[i].res >= threshold) {
			addNewFileResult(resMap, map->fileResults[i].fileName, map->fileResults[i].res);
		}
	}
	printf("满足阈值要求的文件集共有：%d个\n", resMap->count);
	return resMap;
}

/*
 如果左边是结构体本身，就用点 (.)
 如果左边是指向结构体的指针，就用箭头 (->)
它们俩一个是“直接访问”，一个是“间接访问”。
*/