#pragma once
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CaculateMultipleFileRes.h"
typedef struct {
	char* fileName;
	double res;
}Result;
typedef struct {
	Result* fileResults;
	int count;
	int capacity;

}ResultsMap;
/*初识化哈希表，分配初始内存*/
void initialResultsMap(ResultsMap* map, int capacity);
/*销毁 释放内存 C语言中必须自己手动释放*/
void destroyMap(ResultsMap* map);
/*辅助函数 打印哈希表内容*/
void printMap(const ResultsMap* map);
/*排序函数 将哈希表按平均信号强度进行升序排序*/
void sortMapByAvg(ResultsMap* map);
/*辅助函数 交换哈希表中的两个结构体的值*/
void swap(ResultsMap* map, int i, int j);
/*增加 为哈希表增添新元素*/
int addNewFileResult(ResultsMap* map, const char* fileName, double value);
/*获取指定文件名的平均信号强度值 不过只有在判断存不存在的时候用了一下 作用中规中矩*/
double getValueOfFileName(ResultsMap* map, const char* fileName);


