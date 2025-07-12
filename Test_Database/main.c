#include <stdio.h>
#include "DataBaseFunc.h"

int main() {
	MYSQL* connect = NULL;
	//初始化连接
	initialMySqlAndConnectDatabase(&connect, "127.0.0.1", "root", "18598553608lqC", "lqc00075", 0, NULL, 0);
	//切换至指定数据库
	//changeDataBase(connect, "lqc00075");
	//printTableInfo(connect, "cells_selected_table");
	//dropTable(connect, "cells_selected_table");
	//测试 创建小区结果表
	//createCellSelectedResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult");
	//测试 创建平均信号强度表
	//createDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength");
	//测试 创建候选集表
	//createCellsTable(connect,"related_table","dataFileName","ssOrPssFileName","mountainStrength","mountainPosition");
	//测试 向候选集中插入数据
	//insertInfoToCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition","data1.txt"   , "pss1.txt"       , 1566.5            , 5);
	//测试 向小区选择结果表中插入数据
	//insertInfoToCellResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult", "data18.txt", "pss2.txt", 4622.25, 6, "data18pss2");
	//测试 向平均信号强度表中插入数据
	//insertInfoToDataStrengthTable(connect, "avg_of_strength_Table","dataFileName","avgOfStrength","data15.txt",25644.1225);
	//测试 创建指定数据库
	//createDataBase(connect, "123456789");
	//测试 销毁指定数据库
	//dropDataBase(connect, "123456789");
}

