#ifndef NTREE_H
#define NTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum type {
	AND,
	OR,
	NOT,
	PARAMETER
}type;

typedef struct Data {
	type t;
	char id[100];
	char *args;
	int index;
}Data;

typedef struct Node {
	Data d;
	struct Node *next;
}Node;

typedef struct Ntree {
	Node **adj;
	int n;
}Ntree;

Ntree *create_tree(int n);
Node *insert(Node *list, Data d);
void add_edge(Ntree *t, Data d1, Data d2);
void free_tree(Ntree *t);
void print_tree(Ntree *t);

#endif /* NTREE_H */
