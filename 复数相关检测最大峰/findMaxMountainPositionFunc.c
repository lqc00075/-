#include"findMaxMountainPositionFunc.h"
#include"createFileNameFunc.h"
/*直接将数据集加入到结构体中进行寻找计算*/
void findMaxMoutainPositionByStraight(ResultsMap* map, Cell* cells) {
	int index = 0;//用来记录小区数
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

/*计算每一个的峰值 并 添加至小区结构体数组中*/
void caculateMountainToCell(ResultsMap* map, Cell* cells, Complex ss[], char* fileNameOfSS, int* index, int countOfSS) {
	int complexCountsOfActual = 0, complexCountsOfEnsure = countOfSS;
	for (int i = 0; i < map->count; i++) {
		char* fileName = _strdup(map->fileResults[i].fileName);
		Complex* actualData = readTxtFile(fileName, &complexCountsOfActual);//实际信号数据

		double max = 0, cur = 0;
		int maxIndex = 0;
		//卷积流程
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
		printf("文件%s, 确知信号%s, 最大峰值为：%lf，最大峰位置为：%d\n", fileName, fileNameOfSS, max, maxIndex);
		cells[*index].actualName = _strdup(fileName);
		cells[*index].ensureName = _strdup(fileNameOfSS);
		cells[*index].singalStrength = max;
		cells[*index].maxIndex = maxIndex;
		(*index)++;
	}
}

/*寻找组合 即不同data 和 ss文件的组合 然后找出最大峰值 峰值位置在哪里*/
void findMaxMoutainPosition(ResultsMap* map, Cell* cells) {
	int complexCountsOfActual = 0, complexCountsOfEnsure = 0;
	//printf("%d", map->count);
	int index = 0;//用来记录小区数
	for (int i = 0; i < map->count; i++) {
		char* fileName = _strdup(map->fileResults[i].fileName);
		Complex* actualData = readTxtFile(fileName, &complexCountsOfActual);//实际信号数据
		//printf("你好");
		for (int counts = 0; counts < 3; counts++) {
			//确定信号
			char* ensureFileName = createPSSFileName(counts); // 用counts
			//char* ensureFileName = createSSFileName(counts); //少的数据集
			Complex* ensureData = readTxtFile(ensureFileName, &complexCountsOfEnsure);
			double max = 0, cur = 0;
			int maxIndex = 0;
			//卷积流程
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
			printf("文件%s, 确知信号%s, 最大峰值为：%lf，最大峰位置为：%d\n", fileName, ensureFileName, max, maxIndex);
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