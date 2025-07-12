#pragma once
typedef struct {
	char* ensureName;	   //确定信号的名字 即SS0 1 2 或PSS0 1 2
	char* actualName;	   //实际信号的名字 即data0 1 2 3 4 5 6
	double singalStrength;//每个组合的信号强度值
	int maxIndex;		   //峰值位置
}Cell;
/*清理 统一释放内存*/
void destroyCells(Cell* cells, int cellsCount);
/*辅助函数 打印小区选择信息*/
void printCells(const Cell* cells, int cellsCount);
/*辅助函数 交换两个结构体*/
void swapCell(Cell* cells, int i, int j);
/*初始化结构体数组*/
void initialCells(Cell* cells, int cellsCount);
/*将获得小区信息按照最大峰数据进行排序*/
void sortCellsBySingalStrength(Cell* cells);
/*设置终端颜色*/
void setColor(int color);