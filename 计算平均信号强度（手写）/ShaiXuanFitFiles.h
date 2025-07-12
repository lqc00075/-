#pragma once
#include "Map.h"
//阈值
#define THRESHOLD 1000.0
//满足阈值的最大文件集 数目
#define MAX_COUNTS_OF_THRESHOLD 3

/*筛选满足阈值的文件集*/
ResultsMap* filterByThreshold(const ResultsMap* map, ResultsMap* resMap, double threshold);

/*删选满足阈值的文件集的最大信号强度的指定数目*/
ResultsMap* targetCountsMaxAvgMap(const ResultsMap* originalMap, ResultsMap* resMap, int targetCounts);
