#include <stdlib.h>
#include <string.h>

typedef struct Item_st {
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

int insert_st(SymbolTable *h, const char *id, unsigned long long int qtd) {
	Node_st *new_node = malloc(sizeof(Node_st));
	if (new_node == NULL) return 0;
	char *iid = (char*)malloc(sizeof(char));
	strcpy(iid, id);
	new_node->data.id = (const char*)iid;
	new_node->data.qtd = qtd;
	new_node->next = h->begin;
	h->begin = new_node;
	return 1;
}

void add_st(SymbolTable *h, const char *id, unsigned long long int qtd) {
	Node_st *aux = h->begin;
	while (aux != NULL) {
		if (strcmp(aux->data.id, id) == 0) {
			aux->data.qtd += qtd;
			return;
		}
		aux = aux->next;
	}
	insert_st(h, id, qtd);
	return;
}

void free_st(SymbolTable *h) {
	while (h->begin != NULL) {
		Node_st *aux = h->begin;
		h->begin = h->begin->next;
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
		aux = aux->next;
	}
}
