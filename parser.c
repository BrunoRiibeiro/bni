#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "stack.h"
//void write_on_file(char *str) {
//	FILE *f;
//	f = fopen("domain.c", "w");
//	fprintf(f, str);
//	fclose(f);
//}

//int main() {
//	system("touch domain.c");
//	char token[100];
//	while (scanf("%s", token) != EOF) {
//		if (strcmp(token, "(:predicates") == 0) {
//			char buff[100], taux[100];
//			scanf("\t%*c%s", token);
//			int acc = 0;
//			while (scanf("%s", taux) && strcmp(taux, "-") != 0) acc++;
//			sprintf(buff, "printf 'int %s' >> domain.c", token);
//			system(buff);
//			for (int i = 0; i < acc; i++)
//				system("printf '[]' >> domain.c");
//			system("echo ';' >> domain.c");
//		}
//	}
//	return 0;
//}


int main() {
	Stack p, p_aux;
	create_stack(&p, 1000);
	create_stack(&p_aux, 100);
	Item token;
	Head h;
	create_list(&h);
	while (scanf("%c", &token) != EOF) {
		if (token == ' ' || token == '\n' || token == '\t') {
			while (top(&p) != '(') {
				insert_list(&h, top(&p));
				pop(&p);
			}
			if (strcmp_list(&h, ":predicates") == 0) {
				push(&p_aux, '(');
				char count = 0;
				while (scanf("%c", &token) && !is_empty(&p_aux)) {
					char str[100], buff[100];
					if (token == '?') count++;
					if (token == '(') {
						push(&p_aux, token);
						scanf("%s", str);
					}
					else if (token == ')') {
						pop(&p_aux);
						if (!is_empty(&p_aux)) {
							sprintf(buff, "printf 'int %s' >> domain.c", str);
							system(buff);
							for (int i = 0; i < count; i++)
								system("printf '[]' >> domain.c");
							system("echo ';' >> domain.c");
							count = 0;
						}
					}
				}
			}
			free_list(&h);
		}
		push(&p, token);
	}
	return 0;
}
