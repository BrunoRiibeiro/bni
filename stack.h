typedef char Item;
typedef struct Stack {
	Item *v;
	int size, top;
}Stack;

int create_stack(Stack *p, int s) {
	p->size = s;
	p->v = malloc(s * sizeof(Item));
	p->top = 0;
	return 1;
}

int push(Stack *p, Item e) {
	if (p->top == p->size) return 0;
	p->v[p->top++] = e;
	return 1;
}

void pop(Stack *p) {
	p->top--;
}

Item top(Stack *p) {
	return p->v[p->top-1];
}

int is_empty(Stack *p) {
	return p->top == 0;
}
