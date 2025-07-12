#include <stdio.h>
#include"DataBaseFunc.h"
/*初始化数据库并建立连接*/
void initialMySqlAndConnectDatabase(MYSQL** connect, const char* targetServer, const char* user, const char* password, const char* initialDataBase, unsigned int port, const char* unix_socket, unsigned long clientflag) {
	*connect = mysql_init(NULL);
	if (*connect == NULL) {
		printf("初始化失败\t错误信息：%s\n", mysql_error(*connect));
		return;
	}
	if (mysql_real_connect(*connect, targetServer, user, password, initialDataBase, port, unix_socket, clientflag) == NULL) {
		printf("数据库连接失败,\t请检查数据库名是否存在\t错误信息:%s\n", mysql_error(*connect));
		mysql_close(*connect);
		return;
	} else {
		printf("当前服务器为:\t\%s\n", targetServer);
		printf("当前数据库名:\t%s\n", initialDataBase);
		printf("当前用户:\t%s\n", user);
		printf("当前接口为:\t%u\n", port);
	}	//设置中文适配字符集
	if (mysql_set_character_set(*connect, "utf8mb4") != 0) {
		printf("字符集设置失败，请注意不要使用中文即可，当前链接无问题\n");
	}
}

/*创建指定名字的数据库*/
void createDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("数据库名无效或数据库连接建立不正确\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "CREATE DATABASE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, "CREATE DATABASE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("创建数据库失败，请检查数据库名是否合理，请不要使用中文数据库名\t错误信息：%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("创建数据库成功，新建数据库为：%s\t\n", dataBaseName);
			free(action);
			return;
		}
	}
}

/*切换指定名字的数据库*/
void changeDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("数据库名无效或数据库连接建立不正确\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "USE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, "USE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("切换数据库失败，请检查数据库是否存在\t错误信息:%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("切换数据库成功，当前数据库为：%s\t\n", dataBaseName);
			free(action);
			return;
		}
	}
}

/*删除指定数据库*/
void dropDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("数据库名无效或数据库连接建立不正确\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "DROP DATABASE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, "DROP DATABASE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("删除数据库失败，请检查数据库名是否合理，请不要使用中文数据库名\t错误信息：%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("删除数据库成功\t\n");
			free(action);
			return;
		}
	}
}

/*创建小区候选集表*/
void createCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || ssOrPssFileNameField == NULL || mounatainStrengthField == NULL || mountainPositionField == NULL) {
		printf("数据库连接不正确或输入参数有误\n");
		return;
	} else {
		const char* actionTemplate = "CREATE TABLE `%s`("
			"id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, "
			"`%s` VARCHAR(20) NOT NULL, "
			"`%s` VARCHAR(20) NOT NULL, "
			"`%s` DOUBLE,"
			"`%s` INT"
			")";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		if (mysql_query(connect, action) != 0) {
			printf("创建表失败\t错误信息：%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s表创建成功\n\n", tableName);
			printf("当前表头为：\n\n");
			printf("编号\t%-20s %-20s %-20s %-20s\n\n", "原始数据文件", "ssOrPss文件", "峰值强度", "峰值位置");
			printf("id\t%-20s %-20s %-20s %-20s\n\n", dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
			free(action);
			return;
		}
	}
}

/*创建小区选择结果表*/
void createCellSelectedResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || ssOrPssFileNameField == NULL || mounatainStrengthField == NULL || mountainPositionField == NULL) {
		printf("数据库连接不正确或输入参数有误\n");
		return;
	} else {
		const char* actionTemplate = "CREATE TABLE `%s`("
			"id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, "
			"`%s` VARCHAR(255) NOT NULL,"
			"`%s` VARCHAR(255) NOT NULL, "
			"`%s` VARCHAR(255) NOT NULL, "
			"`%s` DOUBLE,"
			"`%s` INT"
			")";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, cellSelectedField, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, cellSelectedField, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		if (mysql_query(connect, action) != 0) {
			printf("创建表失败\t错误信息：%s", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s表创建成功\n\n", tableName);
			printf("当前表头为：\n\n");
			printf("编号\t%-20s %-20s %-20s %-20s %-20s\n\n", "选择的小区", "原始数据文件", "ssOrPss文件", "峰值强度", "峰值位置");
			printf("id\t%-20s %-20s %-20s %-20s %-20s\n\n", cellSelectedField, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
			free(action);
			return;
		}
	}
}

/*创建原始数据按平均信号强度排序后的表*/
void createDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* avgOfStrengthField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL) {
		printf("数据库连接不正确或输入参数有误\n");
		return;
	} else {
		const char* actionTemplate = "CREATE TABLE `%s`("
			"id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, "
			"`%s` VARCHAR(20) NOT NULL, "
			"`%s` DOUBLE"
			")";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, avgOfStrengthField);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存分配失败\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, avgOfStrengthField);
		if (mysql_query(connect, action) != 0) {
			printf("创建表失败\t错误信息：%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s表创建成功\n\n", tableName);
			printf("当前表头为：\n\n");
			printf("编号\t%-20s %-20s\n\n", "原始数据文件", "平均信号强度");
			printf("id\t%-20s %-20s\n\n", dataFileNameField, avgOfStrengthField);
			free(action);
			return;
		}
	}
}

/*插入一行数据到related_table相关表中*/
int insertInfoToCellsTable(MYSQL* connect, const char* tableName,
	const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition) {
	if (connect == NULL || dataFileName == NULL || ssOrPssFileName == NULL) {
		printf("数据库连接失败或插入数据无效或文件名不对\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s,%s,%s) VALUES"
			"('%s','%s',%lf,%d)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存申请失败\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		if (mysql_query(connect, action) != 0) {
			printf("数据库连接失败\t错误信息为：%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("成功插入一条信息\t");
			printf("插入记录为：");
			printf("\t%s\t%s\t%lf\t%d\n", dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
			free(action);
			return 0;
		}
	}
}

/*向avg_of_strength_Table强度表中插入一组数据*/
int insertInfoToDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField, const char* dataFileName, double mountainStrength) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || mounatainStrengthField == NULL || dataFileName == NULL) {
		printf("数据库连接失败或插入数据无效或文件名不对\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s) VALUES"
			"('%s',%lf)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, mounatainStrengthField, dataFileName, mountainStrength);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存申请失败\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, mounatainStrengthField, dataFileName, mountainStrength);
		if (mysql_query(connect, action) != 0) {
			printf("数据库连接失败\t错误信息为：%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("成功插入一条信息\t");
			printf("插入记录为：");
			printf("\t%s\t%lf\n", dataFileName, mountainStrength);
			free(action);
			return 0;
		}
	}
}

/*将小区选择结果插入cells_selected_table表中*/
int insertInfoToCellResultTable(MYSQL* connect, const char* tableName,
	const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedFiled,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition, const char* cellSelectedName) {
	if (connect == NULL || dataFileName == NULL || ssOrPssFileName == NULL) {
		printf("数据库连接失败或插入数据无效或文件名不对\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s,%s,%s,%s) VALUES"
			"('%s','%s','%s',%lf,%d)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, cellSelectedFiled, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("内存申请失败\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, cellSelectedFiled, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		if (mysql_query(connect, action) != 0) {
			printf("数据库连接失败\t错误信息为：%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("成功插入一条信息\t");
			printf("插入记录为：");
			printf("%-20s\t%s\t%s\t%lf\t%d\n", cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
			free(action);
			return 0;
		}
	}
}

/*打印指定名的表内容*/
void printTableInfo(MYSQL* connect, const char* tableName) {
	if (connect == NULL || tableName == NULL) {
		printf("数据库连接失败或输出数据名无效\n");
		return;
	}
	int actionSize = snprintf(NULL, 0, "SELECT * FROM `%s`", tableName);
	char* action = malloc(actionSize + 1);
	if (action == NULL) {
		printf("初始化内存失败\n");
		return;
	}
	snprintf(action, actionSize + 1, "SELECT * FROM `%s`", tableName);
	if (mysql_query(connect, action) != 0) {
		printf("数据库操作失败\t错误信息%s\n", mysql_error(connect));
		return;
	} else {
		MYSQL_RES* dataOfDataBase = mysql_store_result(connect);//获取数据集指针 指向一整片数据结果
		int fieldCount = mysql_num_fields(dataOfDataBase);      //字段数
		printf("当前表为：%s\n", tableName);
		printf("表头为：\n");
		MYSQL_FIELD* fields = mysql_fetch_fields(dataOfDataBase);//字段结构体数组指针
		for (int i = 0; i < fieldCount; i++) {
			printf("%-5s\t\t", fields[i].name);
		}
		printf("\n");
		MYSQL_ROW everyRow;
		while ((everyRow = mysql_fetch_row(dataOfDataBase))) {
			for (int i = 0; i < fieldCount; i++) {
				printf("%-5s\t\t", everyRow[i]);
			}
			printf("\n");
		}
		printf("\n");
		free(action);
		mysql_free_result(dataOfDataBase);
	}

}

/*删除指定表内容*/
void dropTable(MYSQL* connect, const char* tableName) {
	if (connect == NULL || tableName == NULL) {
		printf("数据库连接失败或指定表格名无效\n");
		return;
	}
	int actionSize = snprintf(NULL, 0, "DROP TABLE `%s`", tableName);
	char* action = malloc(actionSize + 1);
	if (action == NULL) {
		printf("内存分配失败\n");
		return;
	}
	snprintf(action, actionSize + 1, "DROP TABLE `%s`", tableName);
	if (mysql_query(connect, action) != 0) {
		printf("数据库连接失败\t错误信息:%s\n", mysql_error(connect));
		return;
	} else {
		printf("删除表：%s成功\n", tableName);
		free(action);
		return;
	}
}