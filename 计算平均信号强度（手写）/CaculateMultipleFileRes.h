#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define INITIAL_CAPACITY 10
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
	double i;
	double q;
	int id;
}Complex;
/*读取指定文件 并可以向外部传出一个结构体数组的长度 以及结构体数组*/
Complex* readTxtFile(const char* fileName, int* complexCount);

/*注意fgets()函数 是按行读取 */
void fgetsFuncToLoad(const char* fileName);

/*注意格式匹配 返回值是成功获取的参数数目 同时在传入的装载容器需传地址进入 */
void fscanfFuncToLoad(const char* fileName);

/*创建每一个文件名*/
char* createFileName(int i);

/*计算单个文件平均信号强度*/
double caculateAvgOfSignal(const Complex* complexs, int size);

/*处理多个文件*/
double* allFilesAvgOfSignal(int fileCount);