#pragma once
/*���ṹ��*/
typedef struct Node {
	double i;
	double q;
	int id;
	struct Node* pre;  //ǰ�����
	struct Node* next; //��̽��
}Node;
/*����ṹ��*/
typedef struct {
	Node* head;
	Node* tail;
	int size;
}LinkedList;
/*��ʼ������ ָ��size==0 ͷ��� β�ڵ�Ϊ��*/
void initialLinkedList(LinkedList* list);
/*����ÿһ���½ڵ�*/
Node* createNode(double valueI, double valueQ, int i);
/*ͷ�巨 ����ڵ�*/
void addHeadList(LinkedList* list, Node* node);
/*ָ��λ�ò���ڵ�*/
void insertList(LinkedList* list, Node* node, int index);
/*ɾ��ָ���ڵ�*/
int deleteNode(LinkedList* list, Node* node);
/*�õ�ָ���ڵ��λ��*/
void getNodePositionOfList(LinkedList* list, Node* node);
/*��ȡ�����С*/
int size(LinkedList* list);
/*��������*/
void destroyList(LinkedList* list);
/*��ӡ��������*/
void printList(const LinkedList* list);