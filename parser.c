#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
    }
	char *domain_file_name = argv[1], *problem_file_name = argv[2];
	
	FILE *domain_file = fopen(domain_file_name, "r"), *problem_file = fopen(problem_file_name, "r");
	if (domain_file == NULL) {
		perror("Error opening domain file");
		return 1;
	} if (problem_file == NULL) {
		perror("Erro opening problem file");
		return 1;
	}

	Stack domain, problem, p_aux; // stacks for tokenize domain and problem files p_aux is used to parenthesis match in :predicates.
	create_stack(&domain, 1000), create_stack(&problem, 1000), create_stack(&p_aux, 100);
	Item tokend, tokenp; // token variables for domain and problem files.
	Head hd, hp; // linked lists for string reading in domain and problem files.
	create_list(&hd), create_list(&hp);
	SymbolTable st;
	create_st(&st);
	char *buffer = NULL; // buffer for getline() function on comments ignoring.
	size_t size_allocated = 0; // size for getline() function on comments ignoring.

	// Problem parser

	while (fscanf(problem_file, "%c", &tokenp)) {
		// ignorar os comentário
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		// tratamento dos dados
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			while (top(&problem) != '(')
				insert_list(&hp, top(&problem)), pop(&problem);
			// Leitura e tratamento dos dados do "objects" até encontrar ")"
			if (strcmp_list(&hp, ":objects") == 0) {
				for ( ; ; ) {
					char count = 0, obj[100];
					while (fscanf(problem_file, "%s", obj) && obj[0] != '-' && obj[0] != ')')
						count++;
					if (obj[0] == ')') break;
					// Ler até encontrar ")" ou espaco ou quebra de linha ou tab
					fscanf(problem_file, " %[^)|^ |^\n|^\t]s", obj);
					// Adiciona ou aumenta a quantidade de repeticoes de um tipo de objeto a tabela de simbolos
					add_st(&st, obj, count);
					if (fscanf(problem_file, "%c", &tokenp) && tokenp == ')') break;
				}
				print_st(&st);
				// Pausar a leitura do problema depois de terminar o tratamento de dados do "objects"
				break;
			}
			free_list(&hp);
		}
		push(&problem, tokenp);
	}
	free_list(&hp);

	// Domain parser
	
	while (fscanf(domain_file, "%c", &tokend) != EOF) {
		if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		if (tokend == ' ' || tokend == '\n' || tokend == '\t') {
			while (top(&domain) != '(')
				insert_list(&hd, top(&domain)),	pop(&domain);
			if (strcmp_list(&hd, ":predicates") == 0) {
				push(&p_aux, '(');
				char count = 0;
				while (fscanf(domain_file, "%c", &tokend) && !is_empty(&p_aux)) {
					if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
					char str[100], buff[100];
					if (tokend == '?') count++;
					if (tokend == '(')
						push(&p_aux, tokend), fscanf(domain_file, "%[^)|^ ]s", str);
					else if (tokend == ')') {
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
			free_list(&hd);
		}
		push(&domain, tokend);
	}
	fclose(domain_file), fclose(problem_file);
	return 0;
}
