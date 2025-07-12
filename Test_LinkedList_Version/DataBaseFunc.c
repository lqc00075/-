#include <stdio.h>
#include"DataBaseFunc.h"
/*��ʼ�����ݿⲢ��������*/
void initialMySqlAndConnectDatabase(MYSQL** connect, const char* targetServer, const char* user, const char* password, const char* initialDataBase, unsigned int port, const char* unix_socket, unsigned long clientflag) {
	*connect = mysql_init(NULL);
	if (*connect == NULL) {
		printf("��ʼ��ʧ��\t������Ϣ��%s\n", mysql_error(*connect));
		return;
	}
	if (mysql_real_connect(*connect, targetServer, user, password, initialDataBase, port, unix_socket, clientflag) == NULL) {
		printf("���ݿ�����ʧ��,\t�������ݿ����Ƿ����\t������Ϣ:%s\n", mysql_error(*connect));
		mysql_close(*connect);
		return;
	} else {
		printf("��ǰ������Ϊ:\t\%s\n", targetServer);
		printf("��ǰ���ݿ���:\t%s\n", initialDataBase);
		printf("��ǰ�û�:\t%s\n", user);
		printf("��ǰ�ӿ�Ϊ:\t%u\n", port);
	}	//�������������ַ���
	if (mysql_set_character_set(*connect, "utf8mb4") != 0) {
		printf("�ַ�������ʧ�ܣ���ע�ⲻҪʹ�����ļ��ɣ���ǰ����������\n");
	}
}

/*����ָ�����ֵ����ݿ�*/
void createDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("���ݿ�����Ч�����ݿ����ӽ�������ȷ\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "CREATE DATABASE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, "CREATE DATABASE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("�������ݿ�ʧ�ܣ��������ݿ����Ƿ�����벻Ҫʹ���������ݿ���\t������Ϣ��%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("�������ݿ�ɹ����½����ݿ�Ϊ��%s\t\n", dataBaseName);
			free(action);
			return;
		}
	}
}

/*�л�ָ�����ֵ����ݿ�*/
void changeDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("���ݿ�����Ч�����ݿ����ӽ�������ȷ\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "USE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, "USE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("�л����ݿ�ʧ�ܣ��������ݿ��Ƿ����\t������Ϣ:%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("�л����ݿ�ɹ�����ǰ���ݿ�Ϊ��%s\t\n", dataBaseName);
			free(action);
			return;
		}
	}
}

/*ɾ��ָ�����ݿ�*/
void dropDataBase(MYSQL* connect, const char* dataBaseName) {
	if (connect == NULL || dataBaseName == NULL) {
		printf("���ݿ�����Ч�����ݿ����ӽ�������ȷ\n");
		return;
	} else {
		int actionSize = snprintf(NULL, 0, "DROP DATABASE `%s`", dataBaseName);
		char* action = (char*)malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, "DROP DATABASE `%s`", dataBaseName);
		if (mysql_query(connect, action) != 0) {
			printf("ɾ�����ݿ�ʧ�ܣ��������ݿ����Ƿ�����벻Ҫʹ���������ݿ���\t������Ϣ��%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("ɾ�����ݿ�ɹ�\t\n");
			free(action);
			return;
		}
	}
}

/*����С����ѡ����*/
void createCellsTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || ssOrPssFileNameField == NULL || mounatainStrengthField == NULL || mountainPositionField == NULL) {
		printf("���ݿ����Ӳ���ȷ�������������\n");
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
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		if (mysql_query(connect, action) != 0) {
			printf("������ʧ��\t������Ϣ��%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s�����ɹ�\n\n", tableName);
			printf("��ǰ��ͷΪ��\n\n");
			printf("���\t%-20s %-20s %-20s %-20s\n\n", "ԭʼ�����ļ�", "ssOrPss�ļ�", "��ֵǿ��", "��ֵλ��");
			printf("id\t%-20s %-20s %-20s %-20s\n\n", dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
			free(action);
			return;
		}
	}
}

/*����С��ѡ������*/
void createCellSelectedResultTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || ssOrPssFileNameField == NULL || mounatainStrengthField == NULL || mountainPositionField == NULL) {
		printf("���ݿ����Ӳ���ȷ�������������\n");
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
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, cellSelectedField, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
		if (mysql_query(connect, action) != 0) {
			printf("������ʧ��\t������Ϣ��%s", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s�����ɹ�\n\n", tableName);
			printf("��ǰ��ͷΪ��\n\n");
			printf("���\t%-20s %-20s %-20s %-20s %-20s\n\n", "ѡ���С��", "ԭʼ�����ļ�", "ssOrPss�ļ�", "��ֵǿ��", "��ֵλ��");
			printf("id\t%-20s %-20s %-20s %-20s %-20s\n\n", cellSelectedField, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField);
			free(action);
			return;
		}
	}
}

/*����ԭʼ���ݰ�ƽ���ź�ǿ�������ı�*/
void createDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* avgOfStrengthField) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL) {
		printf("���ݿ����Ӳ���ȷ�������������\n");
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
			printf("�ڴ����ʧ��\n");
			return;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, avgOfStrengthField);
		if (mysql_query(connect, action) != 0) {
			printf("������ʧ��\t������Ϣ��%s\n", mysql_error(connect));
			free(action);
			return;
		} else {
			printf("%s�����ɹ�\n\n", tableName);
			printf("��ǰ��ͷΪ��\n\n");
			printf("���\t%-20s %-20s\n\n", "ԭʼ�����ļ�", "ƽ���ź�ǿ��");
			printf("id\t%-20s %-20s\n\n", dataFileNameField, avgOfStrengthField);
			free(action);
			return;
		}
	}
}

/*����һ�����ݵ�related_table��ر���*/
int insertInfoToCellsTable(MYSQL* connect, const char* tableName,
	const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition) {
	if (connect == NULL || dataFileName == NULL || ssOrPssFileName == NULL) {
		printf("���ݿ�����ʧ�ܻ����������Ч���ļ�������\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s,%s,%s) VALUES"
			"('%s','%s',%lf,%d)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ�����ʧ��\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		if (mysql_query(connect, action) != 0) {
			printf("���ݿ�����ʧ��\t������ϢΪ��%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("�ɹ�����һ����Ϣ\t");
			printf("�����¼Ϊ��");
			printf("\t%s\t%s\t%lf\t%d\n", dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
			free(action);
			return 0;
		}
	}
}

/*��avg_of_strength_Tableǿ�ȱ��в���һ������*/
int insertInfoToDataStrengthTable(MYSQL* connect, const char* tableName, const char* dataFileNameField, const char* mounatainStrengthField, const char* dataFileName, double mountainStrength) {
	if (connect == NULL || tableName == NULL || dataFileNameField == NULL || mounatainStrengthField == NULL || dataFileName == NULL) {
		printf("���ݿ�����ʧ�ܻ����������Ч���ļ�������\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s) VALUES"
			"('%s',%lf)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, dataFileNameField, mounatainStrengthField, dataFileName, mountainStrength);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ�����ʧ��\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, dataFileNameField, mounatainStrengthField, dataFileName, mountainStrength);
		if (mysql_query(connect, action) != 0) {
			printf("���ݿ�����ʧ��\t������ϢΪ��%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("�ɹ�����һ����Ϣ\t");
			printf("�����¼Ϊ��");
			printf("\t%s\t%lf\n", dataFileName, mountainStrength);
			free(action);
			return 0;
		}
	}
}

/*��С��ѡ��������cells_selected_table����*/
int insertInfoToCellResultTable(MYSQL* connect, const char* tableName,
	const char* dataFileNameField, const char* ssOrPssFileNameField, const char* mounatainStrengthField, const char* mountainPositionField, const char* cellSelectedFiled,
	const char* dataFileName, const char* ssOrPssFileName, double mountainStrength, int mountainPosition, const char* cellSelectedName) {
	if (connect == NULL || dataFileName == NULL || ssOrPssFileName == NULL) {
		printf("���ݿ�����ʧ�ܻ����������Ч���ļ�������\n");
		return -1;
	} else {
		const char* actionTemplate = "INSERT INTO %s (%s,%s,%s,%s,%s) VALUES"
			"('%s','%s','%s',%lf,%d)"
			"";
		int actionSize = snprintf(NULL, 0, actionTemplate, tableName, cellSelectedFiled, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		char* action = malloc(actionSize + 1);
		if (action == NULL) {
			printf("�ڴ�����ʧ��\n");
			return -1;
		}
		snprintf(action, actionSize + 1, actionTemplate, tableName, cellSelectedFiled, dataFileNameField, ssOrPssFileNameField, mounatainStrengthField, mountainPositionField,
			cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
		if (mysql_query(connect, action) != 0) {
			printf("���ݿ�����ʧ��\t������ϢΪ��%s\n", mysql_error(connect));
			free(action);
			return -1;
		} else {
			printf("�ɹ�����һ����Ϣ\t");
			printf("�����¼Ϊ��");
			printf("%-20s\t%s\t%s\t%lf\t%d\n", cellSelectedName, dataFileName, ssOrPssFileName, mountainStrength, mountainPosition);
			free(action);
			return 0;
		}
	}
}

/*��ӡָ�����ı�����*/
void printTableInfo(MYSQL* connect, const char* tableName) {
	if (connect == NULL || tableName == NULL) {
		printf("���ݿ�����ʧ�ܻ������������Ч\n");
		return;
	}
	int actionSize = snprintf(NULL, 0, "SELECT * FROM `%s`", tableName);
	char* action = malloc(actionSize + 1);
	if (action == NULL) {
		printf("��ʼ���ڴ�ʧ��\n");
		return;
	}
	snprintf(action, actionSize + 1, "SELECT * FROM `%s`", tableName);
	if (mysql_query(connect, action) != 0) {
		printf("���ݿ����ʧ��\t������Ϣ%s\n", mysql_error(connect));
		return;
	} else {
		MYSQL_RES* dataOfDataBase = mysql_store_result(connect);//��ȡ���ݼ�ָ�� ָ��һ��Ƭ���ݽ��
		int fieldCount = mysql_num_fields(dataOfDataBase);      //�ֶ���
		printf("��ǰ��Ϊ��%s\n", tableName);
		printf("��ͷΪ��\n");
		MYSQL_FIELD* fields = mysql_fetch_fields(dataOfDataBase);//�ֶνṹ������ָ��
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

/*ɾ��ָ��������*/
void dropTable(MYSQL* connect, const char* tableName) {
	if (connect == NULL || tableName == NULL) {
		printf("���ݿ�����ʧ�ܻ�ָ���������Ч\n");
		return;
	}
	int actionSize = snprintf(NULL, 0, "DROP TABLE `%s`", tableName);
	char* action = malloc(actionSize + 1);
	if (action == NULL) {
		printf("�ڴ����ʧ��\n");
		return;
	}
	snprintf(action, actionSize + 1, "DROP TABLE `%s`", tableName);
	if (mysql_query(connect, action) != 0) {
		printf("���ݿ�����ʧ��\t������Ϣ:%s\n", mysql_error(connect));
		return;
	} else {
		printf("ɾ����%s�ɹ�\n", tableName);
		free(action);
		return;
	}
}