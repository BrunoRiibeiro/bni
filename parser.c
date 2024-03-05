#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "stack.h"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
    }
	char *domain_file_name = argv[1];
	char *problem_file_name = argv[2];
	
	FILE *domain_file = fopen(domain_file_name, "r");
	if (domain_file == NULL) {
		perror("Error opening domain file");
		return 1;
	}

	Stack p, p_aux;
	create_stack(&p, 1000);
	create_stack(&p_aux, 100);
	Item token;
	Head h;
	create_list(&h);
	while (fscanf(domain_file, "%c", &token) != EOF) {
		if (token == ' ' || token == '\n' || token == '\t') {
			while (top(&p) != '(') {
				insert_list(&h, top(&p));
				pop(&p);
			}
			if (strcmp_list(&h, ":predicates") == 0) {
				push(&p_aux, '(');
				char count = 0;
				while (fscanf(domain_file, "%c", &token) && !is_empty(&p_aux)) {
					char str[100], buff[100];
					if (token == '?') count++;
					if (token == '(') {
						push(&p_aux, token);
						fscanf(domain_file, "%s", str);
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
	fclose(domain_file);
	return 0;
}
