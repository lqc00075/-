#pragma once
#include <mysql.h>
/*销毁指定数据库*/
void dropDataBase(MYSQL* connect, const char* dataBaseName);
/*切换至指定数据库*/
void changeDataBase(MYSQL* connect, const char* dataBaseName);
/*创建指定名数据库*/
void createDataBase(MYSQL* connect, const char* dataBaseName);
/*创建平均信号强度表*/
void createDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField);
/*创建候选集信息表*/
void createCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField);
/*创建小区最终选择结果表*/
void createCellSelectedResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedField);
/*初始化MySql并与初始化数据库建立连接*/
void initialMySqlAndConnectDatabase(MYSQL** connect, const char* targetServer, const char* user, const char* password, const char* initialDataBase, unsigned int port, const char* unix_socket, unsigned long clientflag);
/*向候选集信息表插入信息*/
int insertInfoToCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition);
/*向最终结果表插入信息*/
int insertInfoToCellResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedFiled,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition, const char* cellSelectedName);
/*向平均信号强度表插入信息*/
int insertInfoToDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField, const char* dataFileName, double mountainStrength);
/*打印表中信息*/
void printTableInfo(MYSQL* connect, const char* tableName);
/*删除指定表*/
void dropTable(MYSQL* connect, const char* tableName);