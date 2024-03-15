#include <stdlib.h>
#include <stdio.h>

typedef char Item;
typedef struct Node {
	Item data;
	struct Node *next;
}Node;

typedef struct Head{
	Node *begin;
}Head;

int create_list(Head *h) {
	h->begin = NULL;
	return 1;
}

int insert_list(Head *h, Item e) {
	Node *new_node = malloc(sizeof(Node));
	if (new_node == NULL) return 0;
	new_node->data = e;
	new_node->next = h->begin;
	h->begin = new_node;
	return 1;
}

void free_list(Head *h) {
	while (h->begin != NULL) {
		Node *aux = h->begin;
		h->begin = h->begin->next;
		free(aux);
	}
}

void print_list(Head *h) {
	Node *aux = h->begin;
	while (aux != NULL) {
		printf("%c", aux->data);
		aux = aux->next;
	}
	printf("\n");
}

int strcmp_list(Head *h, const char *str) {
	Head *aux = h;
	int i = 0;
	while (str[i++] == aux->begin->data) {
		aux->begin = aux->begin->next;
		if (str[i] == '\0' && aux->begin == NULL)
			return 0;
	}
	return 1;
}
