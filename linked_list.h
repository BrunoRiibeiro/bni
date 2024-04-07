#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef char* Item;
typedef struct Node {
	Item data;
	struct Node *next;
}Node;

typedef struct LinkedList {
	Node *head, *tail;
}LinkedList;

LinkedList* create_list(void) {
	LinkedList *new_list = malloc(sizeof(LinkedList));
	if (new_list == NULL) return NULL;
	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

int insert(LinkedList *h, Item e) {
	Node *new_node = malloc(sizeof(Node));
	if (new_node == NULL) return 0;
	new_node->data = malloc(strlen(e) + 1);
	if (new_node->data == NULL) {
		free(new_node);
		return 0;
	}
	strcpy(new_node->data, e);
	if (h->head == NULL)
		h->tail = new_node;
	new_node->next = h->head;
	h->head = new_node;
	return 1;
}

void free_list(LinkedList *h) {
	while (h->head != NULL) {
		Node *aux = h->head;
		h->head = h->head->next;
		free(aux->data);
		free(aux);
	}
}

int remove_first(LinkedList *h) {
	if (h->head == NULL) return 0;
	Node *aux = h->head;
	h->head = h->head->next;
	free(aux->data);
	free(aux);
	return 1;
}

int is_empty_list(LinkedList *h) {
	return h->head == NULL;
}

int strcmp_list(LinkedList *h, const char *str) {
	if (h->head == NULL || str == NULL) return 1;
	Node *aux = h->head;
	while (aux != NULL && *str != '\0') {
		if (aux->data[0] != *str) return 1;
		aux = aux->next;
		str++;
	}
	if (aux == NULL && *str == '\0') return 0;
	else return 1;
}

void print_list(LinkedList *h) {
	Node *aux = h->head;
	while (aux != NULL) {
		printf("%s ", aux->data);
		aux = aux->next;
	}
	printf("\n");
}

#endif /* LINKED_LIST_H */
