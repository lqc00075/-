/*��ӡ���ļ����ݿ���*/
#include "printFunc.h"
void printfSSComplexs(const Complex* complexs, int count) {
	for (int i = 0; i < count; i++) {
		printf("I���ݣ�%lf  ; Q���ݣ�%lf\n", complexs[i].i, complexs[i].q);
	}
}

/*��ӡ����*/
void printfArr(const double* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%lf ", arr[i]);
	}
}