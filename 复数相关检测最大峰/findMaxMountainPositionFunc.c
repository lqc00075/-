#include"findMaxMountainPositionFunc.h"
#include"createFileNameFunc.h"
/*ֱ�ӽ����ݼ����뵽�ṹ���н���Ѱ�Ҽ���*/
void findMaxMoutainPositionByStraight(ResultsMap* map, Cell* cells) {
	int index = 0;//������¼С����
	Complex ss0[] = {
{ 6.60693, -4.51093, 0 },
{ 6.58546, -4.49181, 1 },
{ 6.52117, -4.43459, 2 },
{ 6.41446, -4.33976, 3 },
{ 6.26597, -4.20811, 4 }
	};
	int count = sizeof(ss0) / sizeof(ss0[0]);
	caculateMountainToCell(map, cells, ss0, "SS0.txt", &index, count);

	Complex ss1[] = {
	{ -6.82357, 4.176, 0 },
	{ -6.80806, 4.15888, 1 },
	{ -6.7616, 4.10763, 2 },
	{ -6.68441, 4.02256, 3 },
	{ -6.57683, 3.90419, 4 }
	};
	count = sizeof(ss1) / sizeof(ss1[0]);
	caculateMountainToCell(map, cells, ss1, "SS1.txt", &index, count);

	Complex ss2[] = {
	{ -6.82357, -4.176, 0 },
	{ -6.80806, -4.15888, 1 },
	{ -6.7616,  -4.10763, 2 },
	{ -6.68441, -4.02256, 3 },
	{ -6.57683, -3.90419, 4 }
	};
	count = sizeof(ss2) / sizeof(ss2[0]);
	caculateMountainToCell(map, cells, ss2, "SS2.txt", &index, count);
}

/*����ÿһ���ķ�ֵ �� �����С���ṹ��������*/
void caculateMountainToCell(ResultsMap* map, Cell* cells, Complex ss[], char* fileNameOfSS, int* index, int countOfSS) {
	int complexCountsOfActual = 0, complexCountsOfEnsure = countOfSS;
	for (int i = 0; i < map->count; i++) {
		char* fileName = _strdup(map->fileResults[i].fileName);
		Complex* actualData = readTxtFile(fileName, &complexCountsOfActual);//ʵ���ź�����

		double max = 0, cur = 0;
		int maxIndex = 0;
		//�������
		for (int j = 0; j <= (complexCountsOfActual - complexCountsOfEnsure); j++) {
			double real = 0, imaginary = 0;
			for (int k = 0; k < complexCountsOfEnsure; k++) {
				real += ss[k].i * actualData[k + j].i +
						ss[k].q * actualData[k + j].q;
				imaginary += ss[k].q * actualData[k + j].i -
							 ss[k].i * actualData[k + j].q;
			}
			cur = sqrt(real * real + imaginary * imaginary);
			if (max < cur) {
				max = cur;
				maxIndex = j;
			}
		}
		printf("�ļ�%s, ȷ֪�ź�%s, ����ֵΪ��%lf������λ��Ϊ��%d\n", fileName, fileNameOfSS, max, maxIndex);
		cells[*index].actualName = _strdup(fileName);
		cells[*index].ensureName = _strdup(fileNameOfSS);
		cells[*index].singalStrength = max;
		cells[*index].maxIndex = maxIndex;
		(*index)++;
	}
}

/*Ѱ����� ����ͬdata �� ss�ļ������ Ȼ���ҳ�����ֵ ��ֵλ��������*/
void findMaxMoutainPosition(ResultsMap* map, Cell* cells) {
	int complexCountsOfActual = 0, complexCountsOfEnsure = 0;
	//printf("%d", map->count);
	int index = 0;//������¼С����
	for (int i = 0; i < map->count; i++) {
		char* fileName = _strdup(map->fileResults[i].fileName);
		Complex* actualData = readTxtFile(fileName, &complexCountsOfActual);//ʵ���ź�����
		//printf("���");
		for (int counts = 0; counts < 3; counts++) {
			//ȷ���ź�
			char* ensureFileName = createPSSFileName(counts); // ��counts
			//char* ensureFileName = createSSFileName(counts); //�ٵ����ݼ�
			Complex* ensureData = readTxtFile(ensureFileName, &complexCountsOfEnsure);
			double max = 0, cur = 0;
			int maxIndex = 0;
			//�������
			for (int j = 0; j <= (complexCountsOfActual - complexCountsOfEnsure); j++) {
				double real = 0, imaginary = 0;
				for (int k = 0; k < complexCountsOfEnsure; k++) {
					real	  += ensureData[k].i * actualData[k + j].i +
								 ensureData[k].q * actualData[k + j].q;
					imaginary += ensureData[k].q * actualData[k + j].i -
							   	 ensureData[k].i * actualData[k + j].q;
				}	
				cur = sqrt(real * real + imaginary * imaginary);
				if (max < cur) {
					max = cur;
					maxIndex = j;
				}
			}
			printf("�ļ�%s, ȷ֪�ź�%s, ����ֵΪ��%lf������λ��Ϊ��%d\n", fileName, ensureFileName, max, maxIndex);
			cells[index].actualName = _strdup(fileName);
			cells[index].ensureName = _strdup(ensureFileName);
			cells[index].singalStrength = max;
			cells[index].maxIndex = maxIndex;
			index++;
			free(ensureData);
		}
		free(fileName);
		free(actualData);
	}
}