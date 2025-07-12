#include"Map.h"
//#include"ShaiXuanFitFiles.h"

/*��ȡָ�������ֵ��ָ���ļ���*/
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

/*ɸѡ������ֵҪ��������ļ���*/
ResultsMap* filterByThreshold(const ResultsMap* map, ResultsMap* resMap, double threshold) {
	for (int i = 0; i < map->count; i++) {
		if (map->fileResults[i].res >= threshold) {
			addNewFileResult(resMap, map->fileResults[i].fileName, map->fileResults[i].res);
		}
	}
	printf("������ֵҪ����ļ������У�%d��\n", resMap->count);
	return resMap;
}
