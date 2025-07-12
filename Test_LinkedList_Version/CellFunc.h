#pragma once
typedef struct {
	char* ensureName;	   //ȷ���źŵ����� ��SS0 1 2 ��PSS0 1 2
	char* actualName;	   //ʵ���źŵ����� ��data0 1 2 3 4 5 6
	double singalStrength;//ÿ����ϵ��ź�ǿ��ֵ
	int maxIndex;		   //��ֵλ��
}Cell;
/*���� ͳһ�ͷ��ڴ�*/
void destroyCells(Cell* cells, int cellsCount);
/*�������� ��ӡС��ѡ����Ϣ*/
void printCells(const Cell* cells, int cellsCount);
/*�������� ���������ṹ��*/
void swapCell(Cell* cells, int i, int j);
/*��ʼ���ṹ������*/
void initialCells(Cell* cells, int cellsCount);
/*�����С����Ϣ�����������ݽ�������*/
void sortCellsBySingalStrength(Cell* cells);
/*�����ն���ɫ*/
void setColor(int color);