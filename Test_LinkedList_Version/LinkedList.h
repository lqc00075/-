#pragma once
/*结点结构体*/
typedef struct Node {
	double i;
	double q;
	int id;
	struct Node* pre;  //前驱结点
	struct Node* next; //后继结点
}Node;
/*链表结构体*/
typedef struct {
	Node* head;
	Node* tail;
	int size;
}LinkedList;
/*初始化链表 指定size==0 头结点 尾节点为空*/
void initialLinkedList(LinkedList* list);
/*创建每一个新节点*/
Node* createNode(double valueI, double valueQ, int i);
/*头插法 插入节点*/
void addHeadList(LinkedList* list, Node* node);
/*指定位置插入节点*/
void insertList(LinkedList* list, Node* node, int index);
/*删除指定节点*/
int deleteNode(LinkedList* list, Node* node);
/*得到指定节点的位置*/
void getNodePositionOfList(LinkedList* list, Node* node);
/*获取链表大小*/
int size(LinkedList* list);
/*清理链表*/
void destroyList(LinkedList* list);
/*打印链表内容*/
void printList(const LinkedList* list);