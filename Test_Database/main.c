#include <stdio.h>
#include "DataBaseFunc.h"

int main() {
	MYSQL* connect = NULL;
	//��ʼ������
	initialMySqlAndConnectDatabase(&connect, "127.0.0.1", "root", "18598553608lqC", "lqc00075", 0, NULL, 0);
	//�л���ָ�����ݿ�
	//changeDataBase(connect, "lqc00075");
	//printTableInfo(connect, "cells_selected_table");
	//dropTable(connect, "cells_selected_table");
	//���� ����С�������
	//createCellSelectedResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult");
	//���� ����ƽ���ź�ǿ�ȱ�
	//createDataStrengthTable(connect, "avg_of_strength_Table", "dataFileName", "avgOfStrength");
	//���� ������ѡ����
	//createCellsTable(connect,"related_table","dataFileName","ssOrPssFileName","mountainStrength","mountainPosition");
	//���� ���ѡ���в�������
	//insertInfoToCellsTable(connect, "related_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition","data1.txt"   , "pss1.txt"       , 1566.5            , 5);
	//���� ��С��ѡ�������в�������
	//insertInfoToCellResultTable(connect, "cells_selected_table", "dataFileName", "ssOrPssFileName", "mountainStrength", "mountainPosition", "selectedCellResult", "data18.txt", "pss2.txt", 4622.25, 6, "data18pss2");
	//���� ��ƽ���ź�ǿ�ȱ��в�������
	//insertInfoToDataStrengthTable(connect, "avg_of_strength_Table","dataFileName","avgOfStrength","data15.txt",25644.1225);
	//���� ����ָ�����ݿ�
	//createDataBase(connect, "123456789");
	//���� ����ָ�����ݿ�
	//dropDataBase(connect, "123456789");
}

