#pragma once
#include "Map.h"
//��ֵ
#define THRESHOLD 1000.0
//������ֵ������ļ��� ��Ŀ
#define MAX_COUNTS_OF_THRESHOLD 3

/*ɸѡ������ֵ���ļ���*/
ResultsMap* filterByThreshold(const ResultsMap* map, ResultsMap* resMap, double threshold);

/*ɾѡ������ֵ���ļ���������ź�ǿ�ȵ�ָ����Ŀ*/
ResultsMap* targetCountsMaxAvgMap(const ResultsMap* originalMap, ResultsMap* resMap, int targetCounts);
