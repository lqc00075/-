#include<stdio.h>
#include<string.h>
int main() {
	char str[2560];
	printf("����������������ַ�����\n");
	fgets(str, sizeof(str), stdin);
	str[strcspn(str, "\n")] = '\0';
	printf("��������ǣ�%s\n", str);
	return 0;
}