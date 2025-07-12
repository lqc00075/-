#pragma once
/*创建SSx.txt文件名*/
char* createSSFileName(int i);
/*创建PSSx.txt文件名*/
char* createPSSFileName(int i);
/*创建datax.txt文件名*/
char* createDataFileName(int i);
/*创建最终输出结果文件名dataXssY*/
char* createResFileName(const char* dataFileName, const char* ssFileName);
