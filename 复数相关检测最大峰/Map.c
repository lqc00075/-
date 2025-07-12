#include "Map.h"
#include "CaculateMultipleFileRes.h"
/*初始化一个Map*/
void initialResultsMap(ResultsMap* map,int capacity) {
	map->count = 0;
	map->capacity = capacity;
	map->fileResults = malloc(capacity * sizeof(Result));
}

/*添加一个新的文件数据进入map*/
int addNewFileResult(ResultsMap* map,const char* fileName,double value) {
	if (getValueOfFileName(map, fileName) == -1) {
		if (map->count >= map->capacity) {
			map->capacity *= 2;
			map->fileResults = realloc(map->fileResults, map->capacity * sizeof(Result));
		}
		map->fileResults[map->count].fileName = _strdup(fileName);//注意这里 是重新在堆上开辟的一段新内存
		map->fileResults[map->count].res = value;
		map->count++;
		return 0;
	}
	return -1;
}

/*获取指定文件名的结果值 顺便判断是否存在该文件名*/
double getValueOfFileName(ResultsMap* map,const char* fileName) {
	for (int i = 0; i < map->count; i++) {
		if (strcmp(map->fileResults[i].fileName,fileName)==0) {//字典序比较
			return map->fileResults[i].res;
		}
	}
	return -1;
}

/*清理！释放内存*/
void destroyMap(ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		free(map->fileResults[i].fileName);
	}
	free(map->fileResults);
	map->capacity = 0;
	map->count = 0;
}

/*将数据写进map表中*/
void dataToMap(ResultsMap* map, int fileCount) {
	for (int i = 0; i < fileCount; i++) {
		int count = 0;
		char* fileName = createFileName(i);
		Complex* complexs = readTxtFile(fileName, &count);
		double ans = caculateAvgOfSignal(complexs, count);
		addNewFileResult(map, fileName, ans);
		free(fileName);
	}
}

/*将数据写入map表中，通过直接将数据集通过结构体数组的方式*/
void dataToMapByStraight(ResultsMap* map) {
	#pragma region 数据集
	Complex data0[] = {
{ 2344.03, -300.208, 0 },
{ 2026.82, -1881.33, 1 },
{ 81.9305, -1387.33, 2 },
{ 178.285, 66.7237, 3 },
{ 1959.36, -31.8907, 4 },
{ 1947.36, -984.243, 5 },
{ -119.448, -1928.44, 6 },
{ -1513.83, -2390.84, 7 },
{ -1099.79, -978.291, 8 },
{ -586.912, 811.322, 9 },
{ -1158.87, 118.141, 10 },
{ -1073.86, -1279.44, 11 },
{ 647.457, -752.142, 12 },
{ 1459.28, -163.773, 13 },
{ 248.337, -1137.24, 14 },
{ -265.381, -1747.46, 15 },
{ 633.795, -856.707, 16 },
{ 294.094, 332.051, 17 },
{ -1460.02, 1068.21, 18 },
{ -2115.45, 1303.24, 19 }
	};
	int count = sizeof(data0) / sizeof(data0[0]);
	double ans = caculateAvgOfSignal(data0, count);
	addNewFileResult(map, "data0.txt", ans);

	Complex data1[] = {
	{ 46.7541, -454.467, 0 },
	{ 178.106, -308.972, 1 },
	{ 542.829, -538.584, 2 },
	{ 179.846, -925.819, 3 },
	{ -471.863, -562.213, 4 },
	{ -117.084, 136.875, 5 },
	{ 701.631, 197.731, 6 },
	{ 511.758, -18.85, 7 },
	{ -350.833, 207.395, 8 },
	{ -687.392, 433.717, 9 },
	{ -621.976, 256.809, 10 },
	{ -665.298, 263.491, 11 },
	{ -549.75, 703.638, 12 },
	{ -361.502, 1063.06, 13 },
	{ -625.105, 1088.99, 14 },
	{ -957.146, 714.314, 15 },
	{ -752.114, -96.1876, 16 },
	{ -419.337, -543.218, 17 },
	{ -297.605, 28.3574, 18 },
	{ -17.7673, 451.509, 19 }
	};
	count = sizeof(data1) / sizeof(data1[0]);
	ans = caculateAvgOfSignal(data1, count);
	addNewFileResult(map, "data1.txt", ans);

	Complex data2[] = {
	{ 678.583, 108.645, 0 },
	{ 866.068, 1498.54, 1 },
	{ 1125.81, 1859.3, 2 },
	{ 1967.78, 1089.63, 3 },
	{ 2137.19, 1320.76, 4 },
	{ 1055.35, 3020.26, 5 },
	{ -322.395, 3773.72, 6 },
	{ -1194.93, 1996.58, 7 },
	{ -1458.41, -554.509, 8 },
	{ -977.569, -1114.58, 9 },
	{ -52.955, 312.686, 10 },
	{ -210.594, 1092.28, 11 },
	{ -1855.77, -231.598, 12 },
	{ -2392.36, -2189.13, 13 },
	{ -654.694, -2855.25, 14 },
	{ 329.71, -2165.06, 15 },
	{ -622.458, -1136.39, 16 },
	{ -743.64, -262.827, 17 },
	{ -61.4908, 521.746, 18 },
	{ -1059.8, 854.333, 19 }
	};
	count = sizeof(data2) / sizeof(data2[0]);
	ans = caculateAvgOfSignal(data2, count);
	addNewFileResult(map, "data2.txt", ans);

	Complex data3[] = {
	{ 36.2822, -61.0299, 0 },
	{ 204.687, -5.5716, 1 },
	{ 60.2785, 2.80446, 2 },
	{ -31.3148, 14.4248, 3 },
	{ 51.0753, 37.4057, 4 },
	{ 36.3684, -41.7917, 5 },
	{ -34.3634, 14.6564, 6 },
	{ 8.91134, 124.755, 7 },
	{ 64.44, 21.3867, 8 },
	{ 42.9066, -15.9834, 9 },
	{ 58.4008, 65.1818, 10 },
	{ 154.735, -57.1058, 11 },
	{ 125.902, -105.287, 12 },
	{ -137.299, 134.182, 13 },
	{ -299.509, 106.369, 14 },
	{ -93.6629, -183.672, 15 },
	{ 121.83, -48.6273, 16 },
	{ 23.1019, 256.723, 17 },
	{ -80.5094, 56.0246, 18 },
	{ 74.2872, -207.988, 19 }
	};
	count = sizeof(data3) / sizeof(data3[0]);
	ans = caculateAvgOfSignal(data3, count);
	addNewFileResult(map, "data3.txt", ans);

	Complex data4[] = {
	{ 600.141, -1301.23, 0 },
	{ 262.375, -1027.6, 1 },
	{ 229.746, 195.259, 2 },
	{ 481.065, 304.116, 3 },
	{ 742.444, -1325.41, 4 },
	{ 416.107, -2304.84, 5 },
	{ -181.559, -827.761, 6 },
	{ 67.1187, 957.394, 7 },
	{ 819.258, 566.462, 8 },
	{ 611.037, -414.568, 9 },
	{ -242.545, 132.265, 10 },
	{ -231.642, 641.942, 11 },
	{ 338.832, -350.686, 12 },
	{ 207.956, -1036.31, 13 },
	{ -234.127, -535.09, 14 },
	{ -38.9427, -734.389, 15 },
	{ 408.604, -1829.05, 16 },
	{ 389.407, -1696.42, 17 },
	{ -128.961, -193.355, 18 },
	{ -856.788, 781.636, 19 }
	};
	count = sizeof(data4) / sizeof(data4[0]);
	ans = caculateAvgOfSignal(data4, count);
	addNewFileResult(map, "data4.txt", ans);

	Complex data5[] = {
	{ -358.932, 50.9223, 0 },
	{ -348.66, 164.939, 1 },
	{ -22.0194, 96.5533, 2 },
	{ 122.967, -170.055, 3 },
	{ -9.90675, -227.378, 4 },
	{ 57.4612, 50.6199, 5 },
	{ 313.125, 249.621, 6 },
	{ 312.442, 167.333, 7 },
	{ 98.2779, 67.8263, 8 },
	{ 30.0308, 86.7665, 9 },
	{ 117.326, 96.3517, 10 },
	{ 175.959, 90.5649, 11 },
	{ 159.913, 148.911, 12 },
	{ 82.011, 182.619, 13 },
	{ -10.9985, 87.9025, 14 },
	{ -62.948, -61.8956, 15 },
	{ -126.482, -159.383, 16 },
	{ -180.59, -207.851, 17 },
	{ -20.2585, -223.667, 18 },
	{ 292.406, -129.28, 19 }
	};
	count = sizeof(data5) / sizeof(data5[0]);
	ans = caculateAvgOfSignal(data5, count);
	addNewFileResult(map, "data5.txt", ans);

	Complex data6[] = {
	{ -1400.47, 1270.56, 0 },
	{ -2197.63, -203.817, 1 },
	{ 1096.52, -2029.55, 2 },
	{ 2370.17, -3102.32, 3 },
	{ -568.664, -360.252, 4 },
	{ -2051.73, 3491.86, 5 },
	{ -639.55, 2700.95, 6 },
	{ 132.939, -983.61, 7 },
	{ 519.76, -2008.08, 8 },
	{ 1157.58, -1151.37, 9 },
	{ -684.234, -1441.04, 10 },
	{ -3161.1, -1503.5, 11 },
	{ -912.487, -337.952, 12 },
	{ 3343.09, 433.09, 13 },
	{ 2786.92, 1196.44, 14 },
	{ -639.931, 2261.62, 15 },
	{ -937.837, 705.862, 16 },
	{ 859.353, -3407.82, 17 },
	{ 985.885, -4578.72, 18 },
	{ 341.253, -472.443, 19 }
	};
	count = sizeof(data6) / sizeof(data6[0]);
	ans = caculateAvgOfSignal(data6, count);
	addNewFileResult(map, "data6.txt", ans);


	#pragma endregion
}

/*辅助函数 打印出来看看 */
void printMap(const ResultsMap* map) {
	for (int i = 0; i < map->count; i++) {
		printf("文件名：%s\t; 信号强度平均值：%lf \n", map->fileResults[i].fileName,
												map->fileResults[i].res);
	}
}

/*排序*/
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

/*交换指定位置的两个结构体*/
void swap(ResultsMap* map, int i, int j) {
	Result temp = map->fileResults[i];
	map->fileResults[i].fileName = _strdup(map->fileResults[j].fileName);
	map->fileResults[i].res = map->fileResults[j].res;
	map->fileResults[j].fileName = _strdup(temp.fileName);
	map->fileResults[j].res = temp.res;
}

/*
 如果左边是结构体本身，就用点 (.)
 如果左边是指向结构体的指针，就用箭头 (->)
它们俩一个是“直接访问”，一个是“间接访问”。
*/