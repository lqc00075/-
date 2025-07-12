#pragma once
#include"Map.h"
#include"CellFunc.h"
/*寻找最大峰数及位置 直接将数据赋值给传入的cells*/
void findMaxMoutainPosition(ResultsMap* map, Cell* cells);
/*通过直接结构体数组读取数据的方式进行相关峰寻找*/
void findMaxMoutainPositionByStraight(ResultsMap* map, Cell* cells);
/*计算相关峰*/
void caculateMountainToCell(ResultsMap* map, Cell* cells, Complex ss[], char* fileNameOfSS, int* index, int countOfSS);
