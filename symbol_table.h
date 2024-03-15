#include <stdlib.h>
#include <string.h>
#include "linked_list_str.h"

typedef struct Item_st {
	Head_str list;
	const char *id;
	unsigned long long int qtd;
}Item_st;

typedef struct Node_st {
	Item_st data;
	struct Node_st *next;
}Node_st;

typedef struct SymbolTable {
	Node_st *begin;
}SymbolTable;

int create_st(SymbolTable *h) {
	h->begin = NULL;
	return 1;
}

void transcribe_list(Node_st *node, Head_str *list) {
	Node_str *aux = list->begin;
	while (aux) {
		char *obj = (char*)malloc(sizeof(char));
		strcpy(obj, aux->data);
		insert_list_str(&node->data.list, (const char*)obj);
		aux = aux->next;
	}
}

int insert_st(SymbolTable *h, const char *id, unsigned long long int qtd, Head_str *list) {
	Node_st *new_node = malloc(sizeof(Node_st));
	if (new_node == NULL) return 0;
	char *iid = (char*)malloc(sizeof(char));
	strcpy(iid, id);
	new_node->data.id = (const char*)iid;
	new_node->data.qtd = qtd;
	new_node->next = h->begin;
	h->begin = new_node;
	create_list_str(&new_node->data.list);
	transcribe_list(new_node, list);
	return 1;
}

int add_st(SymbolTable *h, const char *id, unsigned long long int qtd, Head_str *list) {
	Node_st *aux = h->begin;
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
	while (h->begin != NULL) {
		Node_st *aux = h->begin;
		h->begin = h->begin->next;
		free_list_str(&aux->data.list);
		free(aux);
	}
}

unsigned long long int get_qtd(SymbolTable *h, const char *id) {
	Node_st *aux = h->begin;
	while (aux != NULL) {
		if (strcmp(aux->data.id, id) == 0)
			return aux->data.qtd;
		aux = aux->next;
	}
	return 0;
}

void print_st(SymbolTable *h) {
	Node_st *aux = h->begin;
	while (aux != NULL) {
		printf("id: %s, qtd: %lld\n", aux->data.id, aux->data.qtd);
		print_list_str(&aux->data.list);
		aux = aux->next;
	}
}
