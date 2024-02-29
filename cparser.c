#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "stack.h"

int main() {
	Stack p;
	create_stack(&p, 1000);
	Item token;
	Head h;
	create_list(&h);
	while (scanf("%c", &token) != EOF) {
		if (token == ' ' || token == '\n' || token == '\t') {
			while (top(&p) != '(') {
				insert_list(&h, top(&p));
				pop(&p);
			}
			if (strcmp_list(&h, ":objects") == 0) {
				for ( ; ; ) { 
					char count = 0, obj[100];
					while (scanf("%s", obj) && obj[0] != '-' && obj[0] != ')')
						count++;
					if (obj[0] == ')') break;
					scanf(" %[^)|^ |^\n|^\t]s", obj); 
					printf("%s - %d\n", obj, count);
					if (scanf("%c", &token) && token == ')') break;
				}
			}
			free_list(&h);
		}
		push(&p, token);
	}
	return 0;
}

