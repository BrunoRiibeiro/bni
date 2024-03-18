#include <stdlib.h>
#include <stdio.h>

typedef char Item;
typedef struct Node {
	Item data;
	struct Node *next;
}Node;

typedef struct LinkedList{
	Node *head;
}LinkedList;

LinkedList* create_list() {
	LinkedList *new_list = malloc(sizeof(LinkedList));
	if (new_list == NULL) return NULL;
	new_list->head = NULL;
	return new_list;
}

int insert_list(LinkedList *h, Item e) {
	Node *new_node = malloc(sizeof(Node));
	if (new_node == NULL) return 0;
	new_node->data = e;
	new_node->next = h->head;
	h->head = new_node;
	return 1;
}

int is_empty_list(LinkedList *h) {
	return h->head == NULL;
}

void free_list(LinkedList *h) {
	while (h->head != NULL) {
		Node *aux = h->head;
		h->head = h->head->next;
		free(aux);
	}
}

void print_list(LinkedList *h) {
	Node *aux = h->head;
	while (aux != NULL) {
		printf("%c", aux->data);
		aux = aux->next;
	}
	printf("\n");
}

int strcmp_list(LinkedList *h, const char *str) {
	LinkedList *aux = h;
	char i = 0;
	while (str[i++] == aux->head->data) {
		aux->head = aux->head->next;
		if (str[i] == '\0' && aux->head == NULL)
			return 0;
	}
	return 1;
}
