#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
/*初始化链表*/
void initialLinkedList(LinkedList* list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
/*创建一个结点*/
Node* createNode(double valueI, double valueQ, int i) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		printf("节点创建失败\n");
		return NULL;
	}
	node->i = valueI;
	node->q = valueQ;
	node->id = i;
	node->pre = NULL;
	node->next = NULL;
	return node;
}
/*头插法*/
void addHeadList(LinkedList* list, Node* node) {
	if (list && node) {
		if (list->size == 0) {
			list->head = node;
			list->tail = node;
			node->next = NULL;
			node->pre = NULL;
		} else {
			node->pre = NULL;
			node->next = list->head;
			list->head->pre = node;
			list->head = node;
		}
		list->size++;
	}
}
/*插入指定位置*/
void insertList(LinkedList* list, Node* node, int index) {
	if (!list || !node) return;
	if (index < 0 || index > list->size) return; // 越界保护
	if (list->size == 0) {
		list->head = node;
		list->tail = node;
	} else if (index == 0) {//头插
		addHeadList(list, node);
		return;
	} else if (index == list->size) {//尾插
		node->pre = list->tail;
		list->tail->next = node;
		node->next = NULL;
		list->tail = node;
	} else {
		Node* temp = list->head;
		for (int i = 0; i < index - 1; i++) {
			temp = temp->next;
		}
		node->pre = temp;
		node->next = temp->next;
		temp->next->pre = node;
		temp->next = node;

	}
	list->size++;
}
/*删除节点*/
int deleteNode(LinkedList* list, Node* node) {
	if (list && node) {
		if (node == list->head) {
			list->head = list->head->next;
			if (list->head) {//判单节点情况
				list->head->pre = NULL;
			} else {
				list->tail = NULL;
			}
			free(node);
			list->size--;
			return 0;
		} else if (node == list->tail) {
			list->tail = list->tail->pre;
			if (list->tail) {
				list->tail->next = NULL;
			} else {
				list->head = NULL;  // 链表变为空
			}
			free(node);
			list->size--;
			return 0;
		} else {
			Node* temp = list->head;
			while (temp != node && temp != NULL) {
				temp = temp->next;
			}
			if (temp == node) {
				temp->pre->next = temp->next;
				temp->next->pre = temp->pre;
				free(temp);
				list->size--;
				return 0;
			}
		}
	}
	return -1;
}
/*获取节点位置*/
void getNodePositionOfList(LinkedList* list, Node* node) {
	if (list) {
		Node* temp = list->head;
		while (temp) {
			if (temp == node) {
				printf("该节点的位置在：%d", list->size - temp->id - 1);
				return;
			}
			temp = temp->next;
		}
		printf("未找到指定节点\n");
		return;
	}
	return;
}
/*获取链表大小*/
int size(LinkedList* list) {
	return list->size;
}
/*销毁链表 */
void destroyList(LinkedList* list) {
	if (list) {
		Node* cur = list->head;
		while (cur) {
			Node* next = cur->next;//务必先记录下一个节点
			free(cur);			   // 不能使用释放后的内存
			cur = next;
		}
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	//free(list);
	return;
}
/*辅助函数打印链表内容*/
void printList(const LinkedList* list) {
	if (!list) {
		printf("链表为空\n");
		return;
	}
	if (list) {
		Node* temp = list->head;
		while (temp) {
			printf("第 %d 个节点的I数据为\t%lf\t,Q数据为：%lf\n", temp->id, temp->i, temp->q);
			temp = temp->next;
		}
	}
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	return;
}

