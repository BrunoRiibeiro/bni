#include "symbol_table.h"

SymbolTable* create_st(void) {
	SymbolTable *new_table = malloc(sizeof(SymbolTable));
	if (new_table == NULL) return NULL;
	new_table->head = NULL;
	return new_table;
}

int transcribe_list(Node_st *node, LinkedList *list) {
	Node *aux = list->head;
	while (aux) {
		char *obj = malloc(strlen(aux->data) + 1);
		if (obj == NULL) return 0;
		strcpy(obj, aux->data);
		insert(&node->data.list, obj);
		aux = aux->next;
	}
	return 1;
}

int insert_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList *list) {
	Node_st *new_node = malloc(sizeof(Node_st));
	if (new_node == NULL) return 0;
	new_node->data.id = malloc(strlen(id) + 1);
	if (new_node->data.id == NULL) {
		free(new_node);
		return 0;
	}
	strcpy(new_node->data.id, id);
	new_node->data.qtd = qtd;
	new_node->next = h->head;
	h->head = new_node;
	new_node->data.list = *create_list();
	if (transcribe_list(new_node, list)) return 1;
	else return 0;
}

int add_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList *list) {
	Node_st *aux = h->head;
	while (aux != NULL) {
		if (strcmp(aux->data.id, id) == 0) {
			aux->data.qtd += qtd;
			if (transcribe_list(aux, list)) return 1;
			else return 0;
		}
		aux = aux->next;
	}
	if (insert_st(h, id, qtd, list)) return 1;
	else return 0;
}

void free_st(SymbolTable *h) {
	while (h->head != NULL) {
		Node_st *aux = h->head;
		h->head = h->head->next;
		free_list(&aux->data.list);
		free(aux);
	}
	free(h);
}

size_t get_qtd(SymbolTable *h, const char *id) {
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
		printf("id: %s, qtd: %ld\n", aux->data.id, aux->data.qtd);
		print_list(&aux->data.list);
		aux = aux->next;
	}
}
