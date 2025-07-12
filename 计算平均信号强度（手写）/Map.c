#include "Map.h"
#include "CaculateMultipleFileRes.h"
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
			map->fileResults = realloc(map->fileResults, map->capacity * sizeof(Result));
		}
		map->fileResults[map->count].fileName = _strdup(fileName);//ע������ �������ڶ��Ͽ��ٵ�һ�����ڴ�
		map->fileResults[map->count].res = value;
		map->count++;
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


/*������д��map����*/
void dataToMap(ResultsMap* map, int fileCount) {
	for (int i = 0; i < fileCount; i++) {
		int count = 0;
		char* fileName = createFileName(i);
		Complex* complexs = readTxtFile(fileName, &count);
		double ans = caculateAvgOfSignal(complexs, count);
		addNewFileResult(map, fileName, ans);
	}
}

/*�������� ��ӡ�������� */
void printMap(const ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		printf("�ļ�����%s; �ź�ǿ��ƽ��ֵ��%lf \n", map->fileResults[i].fileName,
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

/*
 �������ǽṹ�屾�����õ� (.)
 ��������ָ��ṹ���ָ�룬���ü�ͷ (->)
������һ���ǡ�ֱ�ӷ��ʡ���һ���ǡ���ӷ��ʡ���
*/