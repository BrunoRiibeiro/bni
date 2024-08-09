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

LinkedList* create_list(void); 
int insert(LinkedList *h, Item e); 
void free_list(LinkedList *h); 
int remove_first(LinkedList *h); 
int is_empty_list(LinkedList *h); 
int strcmp_list(LinkedList *h, const char *str); 
void print_list(LinkedList *h); 
int node_count(LinkedList *h);
int search_on(LinkedList *h, const char *s);
char* list_to_str(LinkedList *h);

#endif /* LINKED_LIST_H */
