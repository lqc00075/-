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
/*��ʶ����ϣ�������ʼ�ڴ�*/
void initialResultsMap(ResultsMap* map, int capacity);
/*���� �ͷ��ڴ� C�����б����Լ��ֶ��ͷ�*/
void destroyMap(ResultsMap* map);
/*�������� ��ӡ��ϣ������*/
void printMap(const ResultsMap* map);
/*������ ����ϣ��ƽ���ź�ǿ�Ƚ�����������*/
void sortMapByAvg(ResultsMap* map);
/*�������� ������ϣ���е������ṹ���ֵ*/
void swap(ResultsMap* map, int i, int j);
/*���� Ϊ��ϣ��������Ԫ��*/
int addNewFileResult(ResultsMap* map, const char* fileName, double value);
/*��ȡָ���ļ�����ƽ���ź�ǿ��ֵ ����ֻ�����жϴ治���ڵ�ʱ������һ�� �����й��о�*/
double getValueOfFileName(ResultsMap* map, const char* fileName);


