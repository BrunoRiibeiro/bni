#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char Item_str;
typedef struct Node_str {
	Item_str *data;
	struct Node_str *next;
}Node_str;

typedef struct LinkedList_str{
	Node_str *head;
	Node_str *tail;
}LinkedList_str;

LinkedList_str* create_list_str() {
	LinkedList_str *new_list = malloc(sizeof(LinkedList_str));
	if (new_list == NULL) return 0;
	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

int first_insert(LinkedList_str *h, Item_str *e) {
	Node_str *new_node = malloc(sizeof(Node_str));
	if (new_node == NULL) return 0;
	char *item_cp = (char*)malloc(sizeof(char));
	strcpy(item_cp, e);
	new_node->data = (const char*)item_cp;
	new_node->next = NULL;
	h->head = new_node;
	h->tail = new_node;
	return 1;
}

int insert_list_str(LinkedList_str *h, Item_str *e) {
	if (h->head == NULL) {
		first_insert(h, e);
		return 1;
	}
	Node_str *new_node = malloc(sizeof(Node_str));
	if (new_node == NULL) return 0;
	char *item_cp = (char*)malloc(sizeof(char));
	strcpy(item_cp, e);
	new_node->data = (const char*)item_cp;
	new_node->next = h->tail->next;
	h->tail->next = new_node;
	h->tail = new_node;
	return 1;
}

void free_list_str(LinkedList_str *h) {
	while (h->head != NULL) {
		Node_str *aux = h->head;
		h->head = h->head->next;
		free(aux);
	}
}

void print_list_str(LinkedList_str *h) {
	Node_str *aux = h->head;
	while (aux != NULL) {
		printf("%s ", aux->data);
		aux = aux->next;
	}
	printf("\n");
}
