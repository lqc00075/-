#include "CaculateMultipleFileRes.h"
/*读取指定文件 并可以向外部传出一个结构体数组的长度 以及结构体数组*/
Complex* readTxtFile(const char* fileName, int* complexCount) {
	*complexCount = 0;
	FILE* file = fopen(fileName, "r");
	if (!file) {
		perror("文件打开失败");
		return NULL;
	}
	Complex* res = NULL;
	int capacity = INITIAL_CAPACITY;
	int count = 0;
	res = (Complex*)malloc(capacity * sizeof(Complex));
	if (res == NULL) {
		printf("初始化内存失败");
		fclose(file);
		return NULL;
	}
	double tempI = 0, tempQ = 0;
	while (fscanf(file, "%lf %lf", &tempI, &tempQ) == 2) {
		//扩容
		if (count >= capacity) {
			capacity *= 2;
			//放置申请内存失败
			Complex* temp = (Complex*)realloc(res, capacity * sizeof(Complex));
			if (temp == NULL) {
				printf("内存分配失败\n");
				*complexCount = 0;
				free(res);//一定记得释放 申请的内存 不然 会出很严重的问题
				fclose(file);
				return NULL;
			}
			res = temp;
		}
		res[count].i = tempI;
		res[count].q = tempQ;
		res[count].id = count;
		count++;
	}
	/*保护手段*/
		// 检查是否因IO错误退出循环
	if (ferror(file)) {
		fprintf(stderr, "读取文件时发生错误\n");
		free(res);
		fclose(file); // 关闭文件
		return NULL;
	}

	// 如果没有读取到任何数据
	if (count == 0) {
		free(res);
		fclose(file);
		return NULL;
	}
	// 安全地收缩内存
	Complex* final_res = (Complex*)realloc(res, count * sizeof(Complex));
	if (final_res == NULL) {
		fprintf(stderr, "内存收缩失败，但返回原始数据\n");
		// 如果收缩失败，final_res 是 NULL，但原始的 res 指针仍然有效
		// 我们可以选择返回原始的 res 指针，它只是比实际需要的大一点
		*complexCount = count;
		fclose(file);
		return res;
	}

	*complexCount = count;
	fclose(file);
	return final_res; // 返回收缩后的内存指针
}

/*创建每一个文件名*/
char* createFileName(int i) {
	int fileNameSize = snprintf(NULL, 0, "data%d.txt", i);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "data%d.txt", i);
	return fileName;
}

/*注意fgets()函数 是按行读取 */
void fgetsFuncToLoad(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file) {
		char temp[64];
		while (fgets(temp, sizeof(temp), file) != NULL) {
			printf("每一行的数据为：%s", temp);
		}
	} else {
		printf("文件打开失败\n");
		return;
	}
	fclose(file);
}

/*注意格式匹配 返回值是成功获取的参数数目 同时在传入的装载容器需传地址进入 */
void fscanfFuncToLoad(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file) {
		double tempI = 0, tempQ = 0;
		while (fscanf(file, "%lf %lf", &tempI, &tempQ) == 2) {
			printf("I数据：%lf;Q数据：%lf\n", tempI, tempQ);
		}
	} else {
		printf("文件读取失败\n");
		return;
	}
	fclose(file);
}

/*计算单个文件平均信号强度*/
double caculateAvgOfSignal(const Complex* complexs, int size) {
	double ans = 0;
	double tempRes = 0;
	for (int i = 0; i < size; i++) {
		tempRes = sqrt(complexs[i].i * complexs[i].i
					 + complexs[i].q * complexs[i].q);
		ans += tempRes;
	}
	return ans / size;
}

/*处理多个文件*/
double* allFilesAvgOfSignal(int fileCount) {
	double* res = NULL;
	res = (double*)malloc(fileCount * sizeof(double));
	for (int i = 0; i < fileCount; i++) {
		int singleFileElements = 0;
		char* fileName = createFileName(i);
		Complex* complexs = readTxtFile(fileName, &singleFileElements);
		res[i] = caculateAvgOfSignal(complexs, singleFileElements);

		free(fileName);
		free(complexs);
	}
	return res;
}