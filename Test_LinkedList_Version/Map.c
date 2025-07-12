#include "Map.h"
/*��ʼ��һ��Map*/
void initialResultsMap(ResultsMap* map,int capacity) {
	map->count = 0;
	map->capacity = capacity;
	map->fileResults = malloc(capacity * sizeof(Result));
}
/*���һ���µ��ļ����ݽ���map*/
int addNewFileResult(ResultsMap* map,const char* fileName,double value) {
	if (getValueOfFileName(map, fileName) == -1) {
		if (map->count >= map->capacity) {
			map->capacity *= 2;
			Result* temp = realloc(map->fileResults, map->capacity * sizeof(Result));
			if (temp == NULL) {
				printf("map����ʧ��\n");
				return -1;
			} else {
				map->fileResults = temp;
			}
			//map->fileResults = realloc(map->fileResults, map->capacity * sizeof(Result));
		}
		if (fileName == NULL) {
			printf("���鴫����ļ������ļ���Ϊ��\n");
			return -1;
		} else {
			map->fileResults[map->count].fileName = _strdup(fileName);//ע������ �������ڶ��Ͽ��ٵ�һ�����ڴ�
			map->fileResults[map->count].res = value;
			map->count++;
		}
		return 0;
	}
	return -1;
}
/*��ȡָ���ļ����Ľ��ֵ ˳���ж��Ƿ���ڸ��ļ���*/
double getValueOfFileName(ResultsMap* map,const char* fileName) {
	for (int i = 0; i < map->count; i++) {
		if (strcmp(map->fileResults[i].fileName,fileName)==0) {//�ֵ���Ƚ�
			return map->fileResults[i].res;
		}
	}
	return -1;
}
/*�����ͷ��ڴ�*/
void destroyMap(ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		free(map->fileResults[i].fileName);
	}
	free(map->fileResults);
	map->capacity = 0;
	map->count = 0;
}
/*�������� ��ӡ�������� */
void printMap(const ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		printf("�ļ�����%s\t; �ź�ǿ��ƽ��ֵ��%lf \n", map->fileResults[i].fileName,
												map->fileResults[i].res);
	}
}
/*����*/
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
/*����ָ��λ�õ������ṹ��*/
void swap(ResultsMap* map, int i, int j) {
	Result temp = map->fileResults[i];
	map->fileResults[i].fileName = _strdup(map->fileResults[j].fileName);
	map->fileResults[i].res = map->fileResults[j].res;
	map->fileResults[j].fileName = _strdup(temp.fileName);
	map->fileResults[j].res = temp.res;
}
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

/*
 �������ǽṹ�屾�����õ� (.)
 ��������ָ��ṹ���ָ�룬���ü�ͷ (->)
������һ���ǡ�ֱ�ӷ��ʡ���һ���ǡ���ӷ��ʡ���
*/