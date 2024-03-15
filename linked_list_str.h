#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char Item_str;
typedef struct Node_str {
	Item_str *data;
	struct Node_str *next;
}Node_str;

typedef struct Head_str{
	Node_str *begin;
	Node_str *end;
}Head_str;

int create_list_str(Head_str *h) {
	h->begin = NULL;
	h->end = NULL;
	return 1;
}

int first_insert(Head_str *h, Item_str *e) {
	Node_str *new_node = malloc(sizeof(Node_str));
	if (new_node == NULL) return 0;
	char *item_cp = (char*)malloc(sizeof(char));
	strcpy(item_cp, e);
	new_node->data = (const char*)item_cp;
	new_node->next = NULL;
	h->begin = new_node;
	h->end = new_node;
	return 1;
}

int insert_list_str(Head_str *h, Item_str *e) {
	if (h->begin == NULL) {
		first_insert(h, e);
		return 1;
	}
	Node_str *new_node = malloc(sizeof(Node_str));
	if (new_node == NULL) return 0;
	char *item_cp = (char*)malloc(sizeof(char));
	strcpy(item_cp, e);
	new_node->data = (const char*)item_cp;
	new_node->next = h->end->next;
	h->end->next = new_node;
	h->end = new_node;
	return 1;
}

void free_list_str(Head_str *h) {
	while (h->begin != NULL) {
		Node_str *aux = h->begin;
		h->begin = h->begin->next;
		free(aux);
	}
}

void print_list_str(Head_str *h) {
	Node_str *aux = h->begin;
	while (aux != NULL) {
		printf("%s ", aux->data);
		aux = aux->next;
	}
	printf("\n");
}
