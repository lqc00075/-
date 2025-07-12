#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
/*
ofstream - 用于写入文件
ifstream - 用于从文件中读取
fstream - 用于读取和写入
*/
using namespace std;
vector<vector<int>> LoadTxtFile(string fileName);
void WriteTxtFile(vector<vector<int>> data, string fileName);
void bubbleSort(int arr[], int len);
int main() {
	//vector<vector<int>> res = {};
	//res = LoadTxtFile("test.txt");
	//for (int i = 0; i < res.size(); i++) {
	//	for (int j = 0; j < res[i].size(); j++) {
	//		cout << res[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//WriteTxtFile(res, "lqc.txt");
	#pragma region 知识点一 文件打开的三种方式 写 追加 仅读
	//申明一个流对象
	//fstream myfile;
	/*
	ios::in - 打开要读取的文件（ifstream 的默认值）
	ios::out - 打开要写入的文件（ofstream 的默认值）
	ios::binary - 以二进制模式打开文件
	ios::app- 打开文件以在末尾附加新信息
	ios::ate - 打开并将控件移动到文件末尾
	ios::trunc - 删除现有文件中的数据
	ios::nocreate - 仅在文件已存在时打开文件
	ios::noreplace - 仅在文件不存在时打开文件
	*/
	// 打开指定文件 后面可以加打开方式
	//myfile.open("test.txt", ios::out);//清空后写入
	//if (myfile.is_open()) {
	//	myfile << "你好，这是第一行我写入的内容" << endl;
	//	myfile << "你好，这是第二行" << endl;
	//	myfile << 66 << 88 << " " << endl;
	//	myfile.close();//使用完一定记得关闭
	//}
	//myfile.open("test.txt", ios::app);//追加内容
	//if (myfile.is_open()) {
	//	for (int i = 0; i < 15; i++) {
	//		myfile << "这是第：" << i << "行内容" << endl;
	//	}
	//	myfile.close();//使用完一定记得关闭
	//}
	//myfile.open("test.txt", ios::in);//读
	//if (myfile.is_open()) {
	//	//string line;
	//	//while (getline(myfile, line)) {//读取方式一：整行读取
	//	//	cout << line << endl;
	//	//}
	//	//读取方式二 逐个字符读取
	//	char c = get();
	//	myfile.close();
	//}
	//注意事项：
	// 一定先声明一个文件流对象
	// 再通过is_open的方式 判断是否流文件写入成功
	// 每行读 使用getline(文件流对象,承载的字符串)
	// 千万记住 使用完文件 必须关闭文件
	#pragma endregion

	#pragma region 知识点二 关于文件的几种状态
	/*
	1、is_open()	检查文件是否成功打开		
	bool: 打开成功为 true	
	stream.is_open()

	2、good()	检查流状态是否正常	
	bool: 状态正常为 true	
	stream.good()

	3、eof()	检查是否到达文件末尾		
	bool: 到达文件尾为 true	
	stream.eof()

	4、fail()	检查是否发生非致命错误（如类型转换失败）
	bool: 非致命错误为 true	
	stream.fail()

	5、bad()	检查是否发生致命错误（如无法读取文件）	
	bool: 致命错误为 true	
	stream.bad()

	*/
	#pragma endregion

	#pragma region 知识点三 关于文件的几种写入 读出API
	
	//fstream file("test.txt", ios::app);
	//方式一： <<写入   >>读出
	// 注意这里的 读入写出一定要与接收的变量类型要匹配
	//string name;
	//int age;
	//if (file.is_open()) {
	//	file << "LQC00075" << endl;
	//	file << 18 << endl;
	//	file.close();
	//}
	//fstream read("test.txt", ios::in);
	//if(read.is_open()){
	//	read >> name >> age;
	//	read.close();
	//}
	//cout << name << " " << age << endl;

	//方式二 get()
	//fstream readFile("test.txt", ios::in);
	//cout << readFile.is_open() << endl;
	//if (readFile.is_open()) {
	//	char temp = readFile.get();
	//	while (temp != EOF) {
	//		cout << temp;
	//		temp = readFile.get();
	//	}
	//	readFile.close();
	//}
	//方式三 getline()
	//if (readFile.is_open()) {
	//	string line;
	//	while (getline(readFile, line)) {
	//		cout << line << endl;
	//	}
	//}
	#pragma endregion

	#pragma region  知识点四 读取CSV文件
	////读文件
	//fstream csvFileStream("test.csv");
	//vector<vector<string>> allData;
	//string line;
	//if(csvFileStream.is_open()) {
	//	while (getline(csvFileStream, line)) {
	//		vector<string> row;
	//		stringstream ss(line);
	//		string cell;
	//		while (getline(ss, cell, ',')) {
	//			row.push_back(cell);
	//		}
	//		allData.push_back(row);
	//	}
	//	csvFileStream.close();
	//}
	//for (int i = 0; i < allData.size(); i++) {
	//	for (int j = 0; j < allData[i].size(); j++) {
	//		cout << allData[i][j] << "\t";
	//	}
	//	cout << endl;
	//}
	//存文件
	//vector<vector<string>> data = {
	//{"姓名", "年龄", "城市", "加入日期"},
	//{"张三", "28", "北京", "2023-06-15"},
	//{"李四", "32", "上海", "2022-11-30"},
	//{"王五", "25", "深圳", "2023-02-18"}
	//};
	//fstream addDataToFile("test.csv", ios::out);
	//if (addDataToFile.is_open()) {
	//	//处理每一行
	//	for (int row = 0; row < data.size(); row++) {
	//		//处理某一行 的 每一列
	//		for (int col = 0; col < data[row].size(); col++) {
	//			//
	//			if (data[row][col].find(',') != string::npos ||
	//				data[row][col].find('\n') != string::npos || 
	//				data[row][col].find('"' != string::npos)) {
	//				//添加起始引号
	//				addDataToFile << "\"";
	//				for (char c : data[row][col]) {
	//					if (c == '"') addDataToFile << "\"\"";
	//					else addDataToFile << c;
	//				}
	//				//添加结束引号
	//				addDataToFile << "\"";
	//			} else {
	//				addDataToFile << data[row][col];
	//			}
	//			//添加逗号分隔符
	//			if (col < data[row].size() - 1) {
	//				addDataToFile << ",";
	//			}
	//		}
	//		addDataToFile << "\n";//行尾换行
	//	}
	//	addDataToFile.close();
	//}
	#pragma endregion

	int arr[8] = { 6,52,3,4,9,55,15,34 };
	bubbleSort(arr, 8);
	for (int i = 0; i < 8; i++) {
		cout << arr[i] << endl;
	}

}

vector<vector<int>> LoadTxtFile(string fileName) {
	ifstream file(fileName); // 使用ifstream只读模式
	vector<vector<int>> res;

	if (!file.is_open()) {
		cerr << "错误：无法打开文件 '" << fileName << "'，请检查文件路径和权限" << endl;
		return res; // 返回空结果
	}

	string line;
	int lineNum = 1; // 用于错误定位

	try {
		while (getline(file, line)) {
			// 跳过空行
			if (line.empty()) {
				lineNum++;
				continue;
			}
			vector<int> row;
			istringstream ss(line);
			string cell;
			// 使用流操作符代替getline分割
			int value;
			while (ss >> value) { 
				row.push_back(value);
			}

			// 检查流是否完全成功
			if (!ss.eof()) {
				// 清除可能的错误状态
				ss.clear();
				// 提取剩余内容用于错误报告
				string remaining;
				ss >> remaining;

				// 处理可能的额外空格
				if (!remaining.empty()) {
					throw runtime_error("第 " + to_string(lineNum) +
						" 行包含非数字内容: '" + remaining + "'");
				}
			}

			if (!row.empty()) {
				res.push_back(row);
			}

			lineNum++;
		}
	}
	catch (const exception& e) {
		cerr << "数据处理错误: " << e.what() << endl;
		// 清空部分结果以避免使用损坏数据
		res.clear();
	}

	return res;
}

void WriteTxtFile(vector<vector<int>> data,string fileName) {
	fstream file(fileName, ios::out);
	if (!file.is_open()) {
		cout << "文件创建失败" << endl;
	} else {
		for (int i = 0; i < data.size(); i++) {
			for (int j = 0; j < data[i].size(); j++) {
				file << data[i][j] << " ";
			}
			file << endl;
		}
	}
	file.close();
}

void bubbleSort(int arr[],int len) {
	for (int i = len - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}