#include "CaculateMultipleFileRes.h"
#include "Map.h"
#include"ShaiXuanFitFiles.h"
#define FILE_COUNTS 7
void bubbleSort(int arr[], int len);

int main() {

	//fscanfFuncToLoad("data0.txt");
	// �����Ƿ���ȷ��ȡ
	//int count = 0;
	//Complex* complexs = readTxtFile("data0.txt", &count);
	//for (int i = 0; i < count; i++) {
	//	printf("��%d�����ݣ�I���ݣ�%lf,Q���ݣ�%lf\n",i, complexs[i].i, complexs[i].q);
	//}
	// ���Լ������Ƿ���ȷ����
	//double ans = caculateAvgOfSingle(complexs, count);
	//printf("%lf", ans);
	// ƴ������
	//char* testFileName = createFileName(2);
	//printf("%s", testFileName);
	// ���ļ�����
	//double* res = allFilesAvgOfSingle(7);
	//for (int i = 0; i < 7; i++) {
	//	printf("%lf\n", res[i]);
	//}
	ResultsMap map;						//ԭʼ���ݼ�
	ResultsMap mapFiltered;				//ɸѡ��
	ResultsMap mapTargetCountsMaxAvg;	//ָ�������Ŀ���ļ���
	initialResultsMap(&map,2);
	initialResultsMap(&mapFiltered, 5);
	initialResultsMap(&mapTargetCountsMaxAvg, 5);
	//addNewFileResult(&map, "data0.txt", 1605.84);
	//addNewFileResult(&map, "data1.txt", 697.32);
	//for (int i = 0; i < map.count; i++) {
	//	printf("%s��%lf\n", map.fileResults[i].fileName, map.fileResults[i].res);
	//}
	dataToMap(&map, FILE_COUNTS);
	printf("δ����ǰ�����ݼ���\n");
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("���������ݼ�����ƽ���ź�ǿ���������򣩣�\n");
	sortMapByAvg(&map);
	printMap(&map);
	printf("--------------------------------------------------\n");

	printf("������ֵ��ƽ���ź�ǿ���ļ�������ǰ��ֵΪ%lf��\n",THRESHOLD);
	mapFiltered = *filterByThreshold(&map,&mapFiltered,THRESHOLD);
	printMap(&mapFiltered);
	printf("--------------------------------------------------\n");

	printf("ָ����ĿΪ��%d�� ������ֵ��ƽ���ź�ǿ�������ļ�����\n", MAX_COUNTS_OF_THRESHOLD);
	mapTargetCountsMaxAvg = *targetCountsMaxAvgMap(&mapFiltered,&mapTargetCountsMaxAvg, MAX_COUNTS_OF_THRESHOLD);
	printMap(&mapTargetCountsMaxAvg);
	printf("--------------------------------------------------\n");
	//����ð��
	//int arr[6] = { 1,2,5,11,1,2 };
	//bubbleSort(arr, 6);
	//for (int i = 0; i < 6; i++) {
	//	printf("%d ", arr[i]);
	//}
	destroyMap(&map);
	destroyMap(&mapFiltered);
	destroyMap(&mapTargetCountsMaxAvg);
	return 0;
}


/*����ð������*/
void bubbleSort(int arr[], int len) {
	for (int i = len - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}