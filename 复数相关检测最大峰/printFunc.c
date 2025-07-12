/*打印各文件数据看看*/
#include "printFunc.h"
void printfSSComplexs(const Complex* complexs, int count) {
	for (int i = 0; i < count; i++) {
		printf("I数据：%lf  ; Q数据：%lf\n", complexs[i].i, complexs[i].q);
	}
}

/*打印函数*/
void printfArr(const double* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%lf ", arr[i]);
	}
}