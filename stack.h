#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include "linked_list.h"

typedef char Item_stk;
typedef struct Stack {
	LinkedList *v;
	int size;
}Stack;

Stack* create_stack(void);
int push(Stack *p, Item_stk e);
void free_stack(Stack *p);
void pop(Stack *p);
int is_empty_stack(Stack *p);
char* top(Stack *p);
int amount(Stack *p);

#endif /* STACK_H */
