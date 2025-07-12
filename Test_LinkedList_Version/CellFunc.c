#include "CellFunc.h"
#include<stdio.h>
#include <Windows.h>
/*初始化Cells*/
void initialCells(Cell* cells,int cellsCount) {
	for (int i = 0; i < cellsCount; i++) {
		cells[i].actualName = NULL;
		cells[i].ensureName = NULL;
	}
}
/*释放申请的内存*/
void destroyCells(Cell* cells, int cellsCount) {
	for (int i = 0; i < cellsCount; i++) {
		if (cells[i].actualName) {
			free(cells[i].actualName);
		}
		if (cells[i].ensureName) {
			free(cells[i].ensureName);
		}
	}
	//free(cells);
}
/*辅助函数 打印*/
void printCells(const Cell* cells,int cellsCount) {
	for (int i = 0; i < cellsCount; i++) {
		setColor(10); // 绿色
		printf("当前实际数据文件（即data文件）：%s\t", cells[i].actualName);
		printf("当前确定信号文件（即ss或pss文件）：%s\n", cells[i].ensureName);
		setColor(7);
		setColor(12); // 红色
		printf("当前最大平均信号强度：%lf\t\t", cells[i].singalStrength);
		setColor(7); // 默认
		printf("当前峰值位置为   ：%d\n", cells[i].maxIndex);
	}
}
/*将组合进行排序 从小到大*/
void sortCellsBySingalStrength(Cell* cells) {
	for (int i = 8; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (cells[j].singalStrength > cells[j + 1].singalStrength) {
				swapCell(cells, j, j + 1);
			}
		}
	}
}
/*辅助函数 帮助于交换Cell*/
void swapCell(Cell* cells, int i, int j) {
	Cell temp = cells[i];
	cells[i] = cells[j];
	cells[j] = temp;
}
/*设置字体颜色*/
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}