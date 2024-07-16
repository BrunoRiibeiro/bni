#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

const char* KEYWORDS[] = {"AND", "OR", "NOT"};
#define KEYWORDS_SIZE (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))
char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.
char *buffer = NULL; // Buffer para a função getline() em comentários ignorados.
size_t size_allocated = 0; // Tamanho alocado para a função getline() em comentários ignorados.

void create_enums(FILE *f, SymbolTable *st) {
	Node_st *aux = st->head;
	while (aux) {
		fprintf(f, "enum %s {\n", aux->data.id);
		Node *obj_list = aux->data.list.head;
		while (obj_list) {
			fprintf(f, "\t%s,\n", obj_list->data);
			obj_list = obj_list->next;
		}
		fprintf(f, "};\n");
		aux = aux->next;
	}
}

void hifen_to_underscore(char *s) {
	while (*s != '\0') {
		if (*s == '-') *s = '_';
		s++;
	}
}

void constants_n_objects(FILE *file, SymbolTable *st, Stack *stack, LinkedList *hl, LinkedList *tmplist, char token) {
	unsigned short count = 0;
	while (fscanf(file, "%c", &token) != EOF) { 
		// Ignora os comentários.
		if (token == ';') getline(&buffer, &size_allocated, file);
		// Inicia o tratamento dos dados do domínio.
		if (token == ' ' || token == '\n' || token == '\t' || token == ')') {
			while (strcmp(top(stack), "(")) {
				if (strcmp(top(stack), "-") == 0) insert(tmplist, "_");
				else insert(tmplist, top(stack));
				pop(stack);
			}
			if (obj_sentinel == 1) {
				char *obj = list_to_str(tmplist);
				add_st(st, obj, count, hl);
				free_list(tmplist), free_list(hl), free(obj);
				obj_sentinel = 0;
			} 
			if (token == ')' && strcmp(top(stack), "(") == 0) {
				if (!is_empty_list(tmplist)) {
					insert(hl, list_to_str(tmplist));
					count++;
				}
				if (!is_empty_list(hl)) {
					add_st(st, "obj", count, hl);
					free_list(tmplist), free_list(hl);
				}
				free_list(tmplist);
				free_list(hl);
				break;
			}
		}
		if (node_count(tmplist) == 1 && strcmp(tmplist->head->data, "_") == 0) obj_sentinel = 1;
		if (!obj_sentinel && !is_empty_list(tmplist)) {
			char *obj = list_to_str(tmplist);
			insert(hl, obj);
			free(obj);
			count++;
		}
		if (token != ' ' && token != '\n' && token != '\t')
			push(stack, token);
		free_list(tmplist);
	}
	return;
}

void predicates(FILE *domain_file, FILE *domainc, SymbolTable *st, Stack *parenthesis_stack, char tokend) {
	create_enums(domainc, st);
	print_st(st);
	push(parenthesis_stack, '(');
	// count = quantos '?' em uma linha.
	unsigned short count = 0;
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		// Ignora comentários dentro do bloco.
		if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		// str = nome do predicado.
		char str[100];
		/* caso:
		   tokend = '?' -> soma count.
		   tokend = '-' -> printa [] com a qtd de objetos.
		   tokend = '(' -> add na pilha de parenteses, printando bool com o nome da variavel.
		   tokend = ')' -> desempilha, caso esteja vazia encerra o tratamento do bloco :predicates. 
		   Caso o obj_sentinel = 0, printa [] com a qtd de objetos pradrao. Printa ';'.
		   */
		if (tokend == '?') count++;
		else if (tokend == '-') {
			fscanf(domain_file, " %[^)|^ ]s", str);
			for (int i = 0; i < count; i++)
				fprintf(domainc, "[%ld]", get_qtd(st, str));
			count = 0;
		}
		else if (tokend == '(') {
			push(parenthesis_stack, tokend), fscanf(domain_file, "%[^)|^ ]s", str);
			hifen_to_underscore(str);
			fprintf(domainc, "bool %s", str);
		}
		else if (tokend == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
			else if (!obj_sentinel) {
				for (int i = 0; i < count; i++)
					fprintf(domainc, "[%ld]", get_qtd(st, "obj"));
				count = 0;
			}
			fprintf(domainc, ";\n");
		}
	}
	return;
}

void action(FILE *domain_file, FILE *domainc, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count) {
	FILE *tmpfile = fopen("tmpfile", "a");
	LinkedList *ha = create_list();
	push(parenthesis_stack, '(');
	char str[100];
	fscanf(domain_file, "%s", str);
	hifen_to_underscore(str);
	fprintf(domainc, "struct %s {\n", str);
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		if (tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == '(')
			stack_to_list(domain, ha);
		if (strcmp_list(ha, ":parameters") == 0)
			while (fscanf(domain_file, "%c", &tokend)) {
				if (tokend == ')') break;
				else if (tokend == '?') {
					char parameters[100];
					fscanf(domain_file, "%[^)|^ ]s", parameters);
					fprintf(domainc, "\tunsigned long %s;\n", parameters);
				}
			}
		else if (strcmp_list(ha, ":precondition") == 0) {
			LinkedList *precondition = create_list();
			int dad[100], dadtop = 0;
			int precon_count = 0;
			fprintf(domainc, "\tNtree *precon;\n");
			// 15 = numero de nós que a arvore tem
			fprintf(tmpfile, "\tNtree *precon_%d = create_tree(15);\n", act_count);
			while (fscanf(domain_file, "%c", &tokend)) {
				if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
				if ((tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == '(' || tokend == ')') && (amount(domain) != 2)){
					stack_to_list(domain, precondition);
					if (!is_empty_list(precondition)) {
						if (precondition->head->data[0] != '?') {
							set_uppercase(precondition);
							fprintf(tmpfile, "\tData d_%d_%d;\n", act_count, precon_count);
							fprintf(tmpfile, "\td_%d_%d.index = %d;\n", act_count, precon_count, precon_count);
							char *preid = list_to_str(precondition);
							fprintf(tmpfile, "\tstrcpy(d_%d_%d.id, \"%s\");\n", act_count, precon_count, preid), free(preid);
							char sig = 0;
							for (int i = 0; i < KEYWORDS_SIZE; i++) {	
								if (strcmp_list(precondition, KEYWORDS[i]) == 0) {
									fprintf(tmpfile, "\td_%d_%d.t = %s;\n", act_count, precon_count, KEYWORDS[i]);
									sig = 1;
								}
							}
							if (!sig) fprintf(tmpfile, "\td_%d_%d.t = %s;\n", act_count, precon_count, "PARAMETER");
							fprintf(tmpfile, "\tinsert(*precon_%d->adj, d_%d_%d);\n", act_count, act_count, precon_count);
							if (precon_count > 0)
								fprintf(tmpfile, "\tadd_edge(precon_%d, d_%d_%d, d_%d_%d);\n", act_count, act_count, dad[dadtop-1], act_count, precon_count);
							dad[dadtop++] = precon_count;
							precon_count++;
						} else {
							//add args dos predicados ?<...>
							char *arg = list_to_str(precondition);
							fprintf(tmpfile, "\td_%d_%d.args = %s;\n", act_count, precon_count-1, arg), free(arg);
						}
					}
				}
				if (tokend == '(') push(parenthesis_stack, tokend);
				else if (tokend == ')') {
					dadtop--;
					pop(parenthesis_stack);
					if (amount(parenthesis_stack) == 1) break;
				}
				else if (tokend != ' ' && tokend != '\n' && tokend != '\t')
					push(domain, tokend);
				free_list(precondition);
			}
			free(precondition);
		} else if (strcmp_list(ha, ":effect") == 0) {
			fprintf(domainc, "\tNtree *effect;\n");
			while (fscanf(domain_file, "%c", &tokend)) {
				if (tokend == '(') push(parenthesis_stack, tokend);
				else if (tokend == ')') {
					pop(parenthesis_stack);
					if (amount(parenthesis_stack) == 1) break;
				}
			}
		} else if (tokend == '(') push(parenthesis_stack, tokend);
		else if (tokend == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
		}
		free_list(ha);
		if (tokend != ' ' && tokend != '\n' && tokend != '\t')
			push(domain, tokend);
	}
	fprintf(domainc, "};\n");
	free_list(ha), free(ha);
	fclose(tmpfile);
	return;
}

void init(FILE *problem_file, FILE *domain_file, FILE *domainc, Stack *parenthesis_stack, char tokenp) {
	push(parenthesis_stack, '(');
	while (fscanf(problem_file, "%c", &tokenp) && !is_empty_stack(parenthesis_stack)) {
		// Ignora comentários dentro do bloco.
		if (tokenp == ';') getline(&buffer, &size_allocated, domain_file);
		// str = nome do predicado.
		char str[100];
		if (tokenp == ' ' && amount(parenthesis_stack) == 2) {
			fscanf(problem_file, "%[^)|^ ]s", str);
			hifen_to_underscore(str);
			fprintf(domainc, "[%s]", str);
		}
		else if (tokenp == '(') {
			push(parenthesis_stack, tokenp), fscanf(problem_file, "%[^)|^ ]s", str);
			hifen_to_underscore(str);
			fprintf(domainc, "\t%s", str);
		}
		else if (tokenp == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
			fprintf(domainc, " = true;\n");
		}
	}
	return;
}

void cat(const char *fname, FILE *f2) {
	FILE *f1 = fopen(fname, "r");
	char c;
	while (fscanf(f1, "%c", &c) != EOF)
		fprintf(f2, "%c", c);
	fclose(f1);
	return;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
	}
	char *domain_file_name = argv[1], *problem_file_name = argv[2];

	FILE *domain_file = fopen(domain_file_name, "r"), *problem_file = fopen(problem_file_name, "r"), *domainc = fopen("pddl.c", "w");
	if (domain_file == NULL) {
		perror("Error opening domain file");
		return 1;
	} if (problem_file == NULL) {
		perror("Erro opening problem file");
		return 1;
	} if (domainc == NULL ) {
		perror("Erro opening problem file");
		return 1;
	}

	Stack *domain = create_stack(), *problem = create_stack(), *parenthesis_stack = create_stack();
	char tokend, tokenp;
	LinkedList *hd = create_list(), *hp = create_list(), *hl = create_list(), *tmplist = create_list();
	SymbolTable *st = create_st();
	fprintf(domainc, "#include <stdbool.h>\n#include <string.h>\n#include \"ntree.h\"\n\n");

	// Problem parser :objects
	while (fscanf(problem_file, "%c", &tokenp)) {
		// Ignorar os comentário.
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			stack_to_list(problem, hp);
			// Tratamento dos dados do  bloco "objects" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":objects") == 0) {
				constants_n_objects(problem_file, st, problem, hl, tmplist, tokenp);
				// Pausar a leitura do problema depois de terminar o tratamento de dados do "objects".
				break;
			}
			free_list(hp);
		}
		if (tokenp != ' ' && tokenp != '\t' && tokenp != '\n')
			push(problem, tokenp);
	}

	// Domain parser
	obj_sentinel = 0;	
	int act_count = 0;
	while (fscanf(domain_file, "%c", &tokend) != EOF) { 
		// Ignora os comentários.
		if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		if (tokend == '(') push(domain, tokend);
		// Inicia o tratamento dos dados do domínio.
		if (tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == '(' || tokend == ')') {
			stack_to_list(domain, hd);
			if (strcmp_list(hd, ":constants") == 0)
				constants_n_objects(domain_file, st, domain, hl, tmplist, tokend);
			else if (strcmp_list(hd, ":predicates") == 0)
				predicates(domain_file, domainc, st, parenthesis_stack, tokend);
			else if (strcmp_list(hd, ":action") == 0)
				action(domain_file, domainc, domain, parenthesis_stack, tokend, act_count++);
			free_list(hd);
		}
		else push(domain, tokend); 
		if (tokend == ')' && strcmp(top(domain), "(") == 0)
			pop(domain);
	}

	// Problem parser :init
	fprintf(domainc, "int main(void) {\n");
	while (fscanf(problem_file, "%c", &tokenp) != EOF) {
		// Ignorar os comentário.
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		if (tokenp == '(') push(problem, tokenp);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t' || tokenp == '(' || tokenp == ')') {
			// Transfere os tokens da pilha para a lista, para assim serem lidos como string (strcmp_list).
			stack_to_list(problem, hp);
			// Tratamento dos dados do  bloco ":init" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":init") == 0)
				init(problem_file, domain_file, domainc, parenthesis_stack, tokenp);
			free_list(hp);
		}
		else push(problem, tokenp);
		if (tokenp == ')' && strcmp(top(problem), "(") == 0)
			pop(problem);
	}
	cat("tmpfile", domainc);
	remove("tmpfile");
	fprintf(domainc, "\treturn 0;\n}\n");


	/* -----------free-n-close------------------- */
	fclose(domain_file), fclose(problem_file), fclose(domainc);
	free_stack(domain), free_stack(problem), free_stack(parenthesis_stack);
	free_list(hp), free_list(hd), free_list(hl), free_list(tmplist);
	free(hp), free(hd), free(hl), free(tmplist);
	free_st(st);
	free(buffer);
	return 0;
}
