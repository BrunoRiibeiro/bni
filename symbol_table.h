#include <stdlib.h>
#include <string.h>
#include "linked_list_str.h"

typedef struct Item_st {
	LinkedList_str list;
	const char *id;
	unsigned long long int qtd;
}Item_st;

typedef struct Node_st {
	Item_st data;
	struct Node_st *next;
}Node_st;

typedef struct SymbolTable {
	Node_st *head;
}SymbolTable;

SymbolTable* create_st(void) {
	SymbolTable *new_table = malloc(sizeof(SymbolTable));
	if (new_table == NULL) return NULL;
	new_table->head = NULL;
	return new_table;
}

void transcribe_list(Node_st *node, LinkedList_str *list) {
	Node_str *aux = list->head;
	while (aux) {
		char *obj = (char*)malloc(sizeof(char));
		strcpy(obj, aux->data);
		insert_list_str(&node->data.list, (const char*)obj);
		aux = aux->next;
	}
}

int insert_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList_str *list) {
	Node_st *new_node = malloc(sizeof(Node_st));
	if (new_node == NULL) return 0;
	char *iid = (char*)malloc(sizeof(char));
	strcpy(iid, id);
	new_node->data.id = (const char*)iid;
	new_node->data.qtd = qtd;
	new_node->next = h->head;
	h->head = new_node;
	new_node->data.list = *create_list_str();
	transcribe_list(new_node, list);
	return 1;
}

int add_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList_str *list) {
	Node_st *aux = h->head;
	while (aux != NULL) {
		if (strcmp(aux->data.id, id) == 0) {
			aux->data.qtd += qtd;
			transcribe_list(aux, list);
			return 1;
		}
		aux = aux->next;
	}
	insert_st(h, id, qtd, list);
	return 0;
}

void free_st(SymbolTable *h) {
	while (h->head != NULL) {
		Node_st *aux = h->head;
		h->head = h->head->next;
		free_list_str(&aux->data.list);
		free(aux);
	}
}

unsigned long long int get_qtd(SymbolTable *h, const char *id) {
	Node_st *aux = h->head;
	while (aux != NULL) {
		if (strcmp(aux->data.id, id) == 0)
			return aux->data.qtd;
		aux = aux->next;
	}
	return 0;
}

void print_st(SymbolTable *h) {
	Node_st *aux = h->head;
	while (aux != NULL) {
		printf("id: %s, qtd: %lld\n", aux->data.id, aux->data.qtd);
		print_list_str(&aux->data.list);
		aux = aux->next;
	}
}
