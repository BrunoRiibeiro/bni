#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

typedef struct Item_st {
	LinkedList *list;
	char *id;
	size_t qtd;
}Item_st;

typedef struct Node_st {
	Item_st data;
	struct Node_st *next;
}Node_st;

typedef struct SymbolTable {
	Node_st *head;
}SymbolTable;

SymbolTable* create_st(void);
int transcribe_list(Node_st *node, LinkedList *list);
int insert_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList *list);
int add_st(SymbolTable *h, const char *id, unsigned long long int qtd, LinkedList *list);
void free_st(SymbolTable *h);
size_t get_qtd(SymbolTable *h, const char *id);
void print_st(SymbolTable *h);

#endif /* SYMBOL_TABLE_H */
