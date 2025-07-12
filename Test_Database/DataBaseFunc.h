#pragma once
#include <mysql.h>
/*����ָ�����ݿ�*/
void dropDataBase(MYSQL* connect, const char* dataBaseName);
/*�л���ָ�����ݿ�*/
void changeDataBase(MYSQL* connect, const char* dataBaseName);
/*����ָ�������ݿ�*/
void createDataBase(MYSQL* connect, const char* dataBaseName);
/*����ƽ���ź�ǿ�ȱ�*/
void createDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField);
/*������ѡ����Ϣ��*/
void createCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField);
/*����С������ѡ������*/
void createCellSelectedResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedField);
/*��ʼ��MySql�����ʼ�����ݿ⽨������*/
void initialMySqlAndConnectDatabase(MYSQL** connect, const char* targetServer, const char* user, const char* password, const char* initialDataBase, unsigned int port, const char* unix_socket, unsigned long clientflag);
/*���ѡ����Ϣ�������Ϣ*/
int insertInfoToCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition);
/*�����ս���������Ϣ*/
int insertInfoToCellResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedFiled,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition, const char* cellSelectedName);
/*��ƽ���ź�ǿ�ȱ������Ϣ*/
int insertInfoToDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField, const char* dataFileName, double mountainStrength);
/*��ӡ������Ϣ*/
void printTableInfo(MYSQL* connect, const char* tableName);
/*ɾ��ָ����*/
void dropTable(MYSQL* connect, const char* tableName);