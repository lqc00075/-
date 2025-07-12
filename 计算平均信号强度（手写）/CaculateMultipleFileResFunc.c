#include "CaculateMultipleFileRes.h"
/*��ȡָ���ļ� ���������ⲿ����һ���ṹ������ĳ��� �Լ��ṹ������*/
Complex* readTxtFile(const char* fileName, int* complexCount) {
	*complexCount = 0;
	FILE* file = fopen(fileName, "r");
	if (!file) {
		perror("�ļ���ʧ��");
		return NULL;
	}
	Complex* res = NULL;
	int capacity = INITIAL_CAPACITY;
	int count = 0;
	res = (Complex*)malloc(capacity * sizeof(Complex));
	if (res == NULL) {
		printf("��ʼ���ڴ�ʧ��");
		fclose(file);
		return NULL;
	}
	double tempI = 0, tempQ = 0;
	while (fscanf(file, "%lf %lf", &tempI, &tempQ) == 2) {
		//����
		if (count >= capacity) {
			capacity *= 2;
			//���������ڴ�ʧ��
			Complex* temp = (Complex*)realloc(res, capacity * sizeof(Complex));
			if (temp == NULL) {
				printf("�ڴ����ʧ��\n");
				*complexCount = 0;
				free(res);//һ���ǵ��ͷ� ������ڴ� ��Ȼ ��������ص�����
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
	/*�����ֶ�*/
		// ����Ƿ���IO�����˳�ѭ��
	if (ferror(file)) {
		fprintf(stderr, "��ȡ�ļ�ʱ��������\n");
		free(res);
		fclose(file); // �ر��ļ�
		return NULL;
	}

	// ���û�ж�ȡ���κ�����
	if (count == 0) {
		free(res);
		fclose(file);
		return NULL;
	}
	// ��ȫ�������ڴ�
	Complex* final_res = (Complex*)realloc(res, count * sizeof(Complex));
	if (final_res == NULL) {
		fprintf(stderr, "�ڴ�����ʧ�ܣ�������ԭʼ����\n");
		// �������ʧ�ܣ�final_res �� NULL����ԭʼ�� res ָ����Ȼ��Ч
		// ���ǿ���ѡ�񷵻�ԭʼ�� res ָ�룬��ֻ�Ǳ�ʵ����Ҫ�Ĵ�һ��
		*complexCount = count;
		fclose(file);
		return res;
	}

	*complexCount = count;
	fclose(file);
	return final_res; // ������������ڴ�ָ��
}

/*����ÿһ���ļ���*/
char* createFileName(int i) {
	int fileNameSize = snprintf(NULL, 0, "data%d.txt", i);
	char* fileName = (char*)malloc(fileNameSize + 1);
	snprintf(fileName, fileNameSize + 1, "data%d.txt", i);
	return fileName;
}

/*ע��fgets()���� �ǰ��ж�ȡ */
void fgetsFuncToLoad(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file) {
		char temp[64];
		while (fgets(temp, sizeof(temp), file) != NULL) {
			printf("ÿһ�е�����Ϊ��%s", temp);
		}
	} else {
		printf("�ļ���ʧ��\n");
		return;
	}
	fclose(file);
}

/*ע���ʽƥ�� ����ֵ�ǳɹ���ȡ�Ĳ�����Ŀ ͬʱ�ڴ����װ�������贫��ַ���� */
void fscanfFuncToLoad(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file) {
		double tempI = 0, tempQ = 0;
		while (fscanf(file, "%lf %lf", &tempI, &tempQ) == 2) {
			printf("I���ݣ�%lf;Q���ݣ�%lf\n", tempI, tempQ);
		}
	} else {
		printf("�ļ���ȡʧ��\n");
		return;
	}
	fclose(file);
}

/*���㵥���ļ�ƽ���ź�ǿ��*/
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

/*�������ļ�*/
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