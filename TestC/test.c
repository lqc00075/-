#include<stdio.h>
#include<string.h>
int main() {
	char str[2560];
	printf("请输入你想输入的字符串：\n");
	fgets(str, sizeof(str), stdin);
	str[strcspn(str, "\n")] = '\0';
	printf("你输入的是：%s\n", str);
	return 0;
}