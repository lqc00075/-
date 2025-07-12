#include"Map.h"
//#include"ShaiXuanFitFiles.h"

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
