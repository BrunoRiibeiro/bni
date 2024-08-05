#include "stack.h"

Stack* create_stack(void) {
	Stack *new_stack = malloc(sizeof(Stack));
	new_stack->v = create_list();
	new_stack->size = 0;
	return new_stack;
}

int push(Stack *p, Item_stk e) {
	char *char_to_str = malloc(2);
	if (char_to_str == NULL) return 0;
	char_to_str[0] = e;
	char_to_str[1] = '\0';
	if (insert(p->v, char_to_str)) {
		p->size++;
		free(char_to_str);
		return 1;
	} else {
		free(char_to_str);
		return 0;
	}
}

void free_stack(Stack *p) {
	if (p == NULL) return;
	free_list(p->v);
	free(p->v);
	free(p);
}

void pop(Stack *p) {
	if (remove_first(p->v))
		p->size--;
}

int is_empty_stack(Stack *p) {
	return p->size == 0;
}

char* top(Stack *p) {
	if (is_empty_stack(p)) return 0;
	return p->v->head->data;
}

int amount(Stack *p) {
	return p->size;
}

void stack_to_list(Stack *p, LinkedList *h) {
   while (!is_empty_stack(p) && strcmp(top(p), "("))
	   insert(h, top(p)), pop(p);
}
