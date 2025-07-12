#include<stdio.h>
#include<stdlib.h>


typedef struct Node {
	int id;
	double score;
	struct Node* next;
}Node;

typedef struct {
	Node* head;
	int size;
}LinkedList;

void initialLinkedList(LinkedList* list) {
	list->head = NULL;
	list->size = 0;
}

Node* createNode(int id,double score) {
	Node* res = NULL;
	res = malloc(sizeof(Node));
	if (res == NULL) {
		printf("创建节点失败\n");
		return NULL;
	}
	res->id = id;
	res->score = score;
	res->next = NULL;
	return res;
}

void insertNode(LinkedList* list,Node* node) {
	if (list == NULL || node == NULL) {
		printf("插入的数据或表无效\n");
		return;
	}
	if (list->head == NULL) {
		list->head = node;
		return;
	}
	Node* temp = list->head;
	Node* lastTemp = temp;
	while (temp->id < node->id && temp->next!=NULL) {
		lastTemp = temp;
		temp = temp->next;
	}if (temp->id > node->id) {
		if (temp == list->head) {
			node->next = list->head;
			list->head = node;
		} else {
			temp = lastTemp;
			node->next = temp->next;
			temp->next = node;
		}
	} else {
		temp->next = node;
	}
	list->size++;
}

void deteleNode(LinkedList* list, int id) {
	if (list && list->head) {
		Node* temp = list->head;
		Node* pre = NULL;
		while (temp && temp->id != id) {
			pre = temp;
			temp = temp->next;
		}
		if (temp == NULL) {
			printf("未找到id为 %d 的节点\n", id);
			return;
		} else {
			if (pre == NULL) {
				list->head = list->head->next;
			} else {
				pre->next = temp->next;
			}
		}
		free(temp);
		list->size--;
	}
}

void printList(const LinkedList* list) {
	if (list) {
		Node* temp = list->head;
		while (temp) {
			printf("id: %d\tscore：%lf\n", temp->id, temp->score);
			temp = temp->next;
		}
	}
}

int main() {
	LinkedList list;
	initialLinkedList(&list);
	Node* node1 = createNode(1, 95.3);
	Node* node2 = createNode(2, 45.3);
	Node* node3 = createNode(3, 75.3);
	Node* node4 = createNode(4, 85.3);
	Node* node5 = createNode(5, 65.3);

	insertNode(&list, node1);
	insertNode(&list, node2);
	insertNode(&list, node3);
	insertNode(&list, node4);
	insertNode(&list, node5);

	printList(&list);
	printf("***********************\n");

	deteleNode(&list, 3);
	printList(&list);

	printf("***********************\n");
	Node* node6 = createNode(3, 65.3);
	insertNode(&list, node6);
	printList(&list);
	return 0;
}