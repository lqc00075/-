#pragma once
#include"Map.h"
#include"CellFunc.h"
/*Ѱ����������λ�� ֱ�ӽ����ݸ�ֵ�������cells*/
void findMaxMoutainPosition(ResultsMap* map, Cell* cells);
/*ͨ��ֱ�ӽṹ�������ȡ���ݵķ�ʽ������ط�Ѱ��*/
void findMaxMoutainPositionByStraight(ResultsMap* map, Cell* cells);
/*������ط�*/
void caculateMountainToCell(ResultsMap* map, Cell* cells, Complex ss[], char* fileNameOfSS, int* index, int countOfSS);
