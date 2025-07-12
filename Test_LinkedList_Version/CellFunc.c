#include "CellFunc.h"
#include<stdio.h>
#include <Windows.h>
/*��ʼ��Cells*/
void initialCells(Cell* cells,int cellsCount) {
	for (int i = 0; i < cellsCount; i++) {
		cells[i].actualName = NULL;
		cells[i].ensureName = NULL;
	}
}
/*�ͷ�������ڴ�*/
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
/*�������� ��ӡ*/
void printCells(const Cell* cells,int cellsCount) {
	for (int i = 0; i < cellsCount; i++) {
		setColor(10); // ��ɫ
		printf("��ǰʵ�������ļ�����data�ļ�����%s\t", cells[i].actualName);
		printf("��ǰȷ���ź��ļ�����ss��pss�ļ�����%s\n", cells[i].ensureName);
		setColor(7);
		setColor(12); // ��ɫ
		printf("��ǰ���ƽ���ź�ǿ�ȣ�%lf\t\t", cells[i].singalStrength);
		setColor(7); // Ĭ��
		printf("��ǰ��ֵλ��Ϊ   ��%d\n", cells[i].maxIndex);
	}
}
/*����Ͻ������� ��С����*/
void sortCellsBySingalStrength(Cell* cells) {
	for (int i = 8; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (cells[j].singalStrength > cells[j + 1].singalStrength) {
				swapCell(cells, j, j + 1);
			}
		}
	}
}
/*�������� �����ڽ���Cell*/
void swapCell(Cell* cells, int i, int j) {
	Cell temp = cells[i];
	cells[i] = cells[j];
	cells[j] = temp;
}
/*����������ɫ*/
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}