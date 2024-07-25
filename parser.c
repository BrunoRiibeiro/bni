#include <stdio.h>
#include <ctype.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

const char* KEYWORDS[] = {"and", "or", "not", "forall", "when"};
#define KEYWORDS_SIZE (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))
char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.

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

char *preprocess_file(const char *f) {
	size_t linecap = 0;
	char token, *line = NULL, *filename = malloc(sizeof(f) + 10);
	sprintf(filename, "/tmp%s", strrchr(f, '/'));
	FILE *basefile = fopen(f, "r"), *file = fopen(filename, "w");
	if (basefile == NULL) {
		perror("Error opening base file in preprocessing file step.");
		return NULL;
	} if (file == NULL) {
		perror("Error opening processed file in preprocessing file step.");
		return NULL;
	}
	while (fscanf(basefile, "%c", &token) != EOF)
		if (token == ';') getline(&line, &linecap, basefile);	
		else if (token == '\t') fprintf(file, " ");
		else if (token != '\n') fprintf(file, "%c", tolower(token));
	fclose(basefile), fclose(file), free(line);
	return filename;
}

void cat(const char *fname, FILE *f2) {
	FILE *f1 = fopen(fname, "r");
	char c;
	while (fscanf(f1, "%c", &c) != EOF)
		fprintf(f2, "%c", c);
	fclose(f1);
	return;
}

void constants_n_objects(FILE *file, SymbolTable *st, Stack *stack, LinkedList *hl, LinkedList *tmplist, char token) {
	unsigned short count = 0;
	while (fscanf(file, "%c", &token) != EOF) { 
		if (token == ' ' || token == ')') {
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
		if (token != ' ') push(stack, token);
		free_list(tmplist);
	}
	return;
}

void predicates(FILE *domain_file, FILE *domainc, SymbolTable *st, Stack *parenthesis_stack, char tokend) {
	FILE *tmpfile = fopen("/tmp/tmpfile", "a");
	create_enums(domainc, st);
	push(parenthesis_stack, '(');
	// count = quantos '?' em uma linha.
	unsigned short count = 0, count_p = 0;
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		// str = nome do predicado.
		char str[100];
		/* caso:
		   tokend = '?' -> soma count.
		   tokend = '-' -> printa [] com a qtd de objetos.
		   tokend = '(' -> add na pilha de parenteses, printando bool com o nome da variavel.
		   tokend = ')' -> desempilha, caso esteja vazia encerra o tratamento do bloco :predicates. 
		   Caso o obj_sentinel = 0, printa [] com a qtd de objetos pradrao. Printa ';'.
		   bool check(int tmpstr, ...) {
		       return precon[tmb];
		   }
		*/
		char tmpstr[30][100];
		if (tokend == '?') {
			fscanf(domain_file, " %[^)|^ ]s", tmpstr[count_p]);
			if (count_p > 0) fprintf(tmpfile, ", int %s", tmpstr[count_p]);
			else fprintf(tmpfile, "int %s", tmpstr[count_p]);
			count++, count_p++;
		}
		else if (tokend == '-') {
			char typename[100];
			obj_sentinel = 1;
			fscanf(domain_file, " %[^)|^ ]s", typename);
			for (int i = 0; i < count; i++)
				fprintf(domainc, "[%ld]", get_qtd(st, typename));
			count = 0;
		}
		else if (tokend == '(') {
			push(parenthesis_stack, tokend), fscanf(domain_file, "%[^)|^ ]s", str);
			hifen_to_underscore(str);
			fprintf(domainc, "bool %s", str);
			fprintf(tmpfile, "bool checktrue_%s(", str);
		}
		else if (tokend == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
			if (!obj_sentinel) {
				for (int i = 0; i < count; i++)
					fprintf(domainc, "[%ld]", get_qtd(st, "obj"));
				count = 0;
			}
			fprintf(tmpfile, ") {\n\treturn %s", str);
			for (int i = 0; i < count_p; i++)
				fprintf(tmpfile, "[%s]", tmpstr[i]);
			count_p = 0, obj_sentinel = 0;
			fprintf(domainc, ";\n");
			fprintf(tmpfile, ";\n}\n");
		}
	}
	fclose(tmpfile);
	cat("/tmp/tmpfile", domainc);
	remove("/tmp/tmpfile");
	return;
}

void action(FILE *domain_file, FILE *domainc, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count) {
	LinkedList *ha = create_list();
	push(parenthesis_stack, '(');
	char str[100];
	fscanf(domain_file, "%s", str);
	hifen_to_underscore(str);
	fprintf(domainc, "struct %s {\n", str);
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		if (tokend == ' ' || tokend == '(')
			stack_to_list(domain, ha);
		if (strcmp_list(ha, ":parameters") == 0) {
			while (fscanf(domain_file, "%c", &tokend)) {
				if (tokend == ')') break;
				else if (tokend == '?') {
					char parameters[100];
					fscanf(domain_file, "%[^)|^ ]s", parameters);
					fprintf(domainc, "\tint %s;\n", parameters);
				}
			}
			fprintf(domainc, "};\n");
		} else if (strcmp_list(ha, ":precondition") == 0) {
			fprintf(domainc, "void checktrue_%s(struct %s s) {\n\treturn ", str, str);
			LinkedList *precondition = create_list();
			char sigarg = 0, flag = 0;
			while (fscanf(domain_file, "%c", &tokend)) {
				if ((tokend == ' ' || tokend == '(' || tokend == ')') && (amount(domain) != 2)) {
					stack_to_list(domain, precondition);
					if (!is_empty_list(precondition)) {
						if (precondition->head->data[0] != '?') {
							char sig = 0;
							for (int i = 0; i < KEYWORDS_SIZE; i++) {	
								if (strcmp_list(precondition, KEYWORDS[i]) == 0) {
									if (flag) fprintf(domainc, ", %s(", KEYWORDS[i]);
									else fprintf(domainc, "%s(", KEYWORDS[i]);
									sig = 1, flag = 0;
								}
							}
							if (!sig) {
								char *preid = list_to_str(precondition);
								if (flag) fprintf(domainc, ", checktrue_%s(", preid);
								else fprintf(domainc, "checktrue_%s(", preid);
								free(preid), flag = 0;
							}
						} else {
							//add args dos predicados ?<...>
							char *arg = list_to_str(precondition);
							if (sigarg) fprintf(domainc, ", s.%s", arg+1);
							else fprintf(domainc, "s.%s", arg+1), sigarg = 1;
							free(arg);
						}
					}
				}
				if (tokend == '(') push(parenthesis_stack, tokend);
				else if (tokend == ')') {
					sigarg = 0, flag = 1;
					pop(parenthesis_stack);
					if (amount(parenthesis_stack) == 1) break;
					fprintf(domainc, ")");
				}
				else if (tokend != ' ') push(domain, tokend);
				free_list(precondition);
			}
			fprintf(domainc, ");\n}\n");
			free(precondition);
		} else if (strcmp_list(ha, ":effect") == 0) {
			fprintf(domainc, "void apply_%s(struct %s s) {\n", str, str);
			LinkedList *effect = create_list();
			char isnot = 1, flag = 0;
			while (fscanf(domain_file, "%c", &tokend)) {
				if ((tokend == ' ' || tokend == '(' || tokend == ')') && (amount(domain) != 2)) {
					stack_to_list(domain, effect);
					if (!is_empty_list(effect)) {
						flag = 0;
						if (effect->head->data[0] != '?') {
							if (strcmp_list(effect, "not") == 0) {
								isnot = 0, free_list(effect);
								continue;
							} else if (strcmp_list(effect, "and") == 0) {free_list(effect); continue;}
							char *predicate = list_to_str(effect);
							fprintf(domainc, "\t%s", predicate);
						} else {
							//add args dos predicados ?<...>
							char *arg = list_to_str(effect);
							fprintf(domainc, "[s.%s]", arg+1);
							free(arg);
						}
					}
				}
				if (tokend == '(') push(parenthesis_stack, tokend);
				else if (tokend == ')') {
					pop(parenthesis_stack);
					if (amount(parenthesis_stack) == 1) break;
					if (flag == 0)
						fprintf(domainc, " = %d;\n", isnot), isnot = 1;
					flag = 1;
				} else if (tokend != ' ') push(domain, tokend);
				free_list(effect);
			}
			fprintf(domainc, "}\n");
			free(effect);
		} else if (tokend == '(') push(parenthesis_stack, tokend);
		else if (tokend == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
		}
		free_list(ha);
		if (tokend != ' ') push(domain, tokend);
	}
	free_list(ha), free(ha);
	return;
}

void init(FILE *problem_file, FILE *domain_file, FILE *domainc, Stack *parenthesis_stack, char tokenp) {
	push(parenthesis_stack, '(');
	while (fscanf(problem_file, "%c", &tokenp) && !is_empty_stack(parenthesis_stack)) {
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

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
	}
	char *domain_file_name = argv[1], *problem_file_name = argv[2];
	domain_file_name = preprocess_file(domain_file_name), problem_file_name = preprocess_file(problem_file_name);

	FILE *domain_file = fopen(domain_file_name, "r"), 
		 *problem_file = fopen(problem_file_name, "r"), 
		 *domainc = fopen("pddl.c", "w");
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
	fprintf(domainc, "#include <stdbool.h>\n#include <string.h>\n\n");

	// Problem parser :objects
	while (fscanf(problem_file, "%c", &tokenp)) {
		// Tratamento dos dados.
		if (tokenp == ' ') {
			stack_to_list(problem, hp);
			// Tratamento dos dados do  bloco "objects" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":objects") == 0) {
				constants_n_objects(problem_file, st, problem, hl, tmplist, tokenp);
				// Pausar a leitura do problema depois de terminar o tratamento de dados do "objects".
				break;
			}
			free_list(hp);
		}
		if (tokenp != ' ') push(problem, tokenp);
	}

	// Domain parser
	obj_sentinel = 0;	
	int act_count = 0;
	while (fscanf(domain_file, "%c", &tokend) != EOF) { 
		// Ignora os comentários.
		if (tokend == '(') push(domain, tokend);
		// Inicia o tratamento dos dados do domínio.
		if (tokend == ' ' || tokend == '(' || tokend == ')') {
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
		if (tokenp == '(') push(problem, tokenp);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '(' || tokenp == ')') {
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
	fprintf(domainc, "\treturn 0;\n}\n");


	/* -----------free-n-close------------------- */
	fclose(domain_file), fclose(problem_file), fclose(domainc);
	free_stack(domain), free_stack(problem), free_stack(parenthesis_stack);
	free_list(hp), free_list(hd), free_list(hl), free_list(tmplist);
	free(hp), free(hd), free(hl), free(tmplist);
	free_st(st);
	remove(domain_file_name), remove(problem_file_name);
	return 0;
}
