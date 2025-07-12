#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
/*��ʼ������*/
void initialLinkedList(LinkedList* list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
/*����һ�����*/
Node* createNode(double valueI, double valueQ, int i) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		printf("�ڵ㴴��ʧ��\n");
		return NULL;
	}
	node->i = valueI;
	node->q = valueQ;
	node->id = i;
	node->pre = NULL;
	node->next = NULL;
	return node;
}
/*ͷ�巨*/
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
/*����ָ��λ��*/
void insertList(LinkedList* list, Node* node, int index) {
	if (!list || !node) return;
	if (index < 0 || index > list->size) return; // Խ�籣��
	if (list->size == 0) {
		list->head = node;
		list->tail = node;
	} else if (index == 0) {//ͷ��
		addHeadList(list, node);
		return;
	} else if (index == list->size) {//β��
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
/*ɾ���ڵ�*/
int deleteNode(LinkedList* list, Node* node) {
	if (list && node) {
		if (node == list->head) {
			list->head = list->head->next;
			if (list->head) {//�е��ڵ����
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
				list->head = NULL;  // �����Ϊ��
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
/*��ȡ�ڵ�λ��*/
void getNodePositionOfList(LinkedList* list, Node* node) {
	if (list) {
		Node* temp = list->head;
		while (temp) {
			if (temp == node) {
				printf("�ýڵ��λ���ڣ�%d", list->size - temp->id - 1);
				return;
			}
			temp = temp->next;
		}
		printf("δ�ҵ�ָ���ڵ�\n");
		return;
	}
	return;
}
/*��ȡ�����С*/
int size(LinkedList* list) {
	return list->size;
}
/*�������� */
void destroyList(LinkedList* list) {
	if (list) {
		Node* cur = list->head;
		while (cur) {
			Node* next = cur->next;//����ȼ�¼��һ���ڵ�
			free(cur);			   // ����ʹ���ͷź���ڴ�
			cur = next;
		}
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	//free(list);
	return;
}
/*����������ӡ��������*/
void printList(const LinkedList* list) {
	if (!list) {
		printf("����Ϊ��\n");
		return;
	}
	if (list) {
		Node* temp = list->head;
		while (temp) {
			printf("�� %d ���ڵ��I����Ϊ\t%lf\t,Q����Ϊ��%lf\n", temp->id, temp->i, temp->q);
			temp = temp->next;
		}
	}
	printf("----------------------------------------------------------------------------------------------------------------------\n");
	return;
}

