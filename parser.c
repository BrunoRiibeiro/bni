#include <stdio.h>
#include <ctype.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

const char* KEYWORDS[] = {"and", "or", "not", "forall", "when"};
#define KEYWORDS_SIZE (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))
char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.

void create_enums(FILE *fc, FILE *fh, SymbolTable *st) {
	Node_st *aux = st->head;
	while (aux) {
		fprintf(fh, "enum %s {\n", aux->data.id);
		fprintf(fc, "const char *%s_names[LENGTH_%s] = {\n", aux->data.id, aux->data.id);
		Node *obj_list = aux->data.list->head;
		while (obj_list) {
			fprintf(fh, "\t%s,\n", obj_list->data);
			fprintf(fc, "\t\"%s\",\n", obj_list->data);
			obj_list = obj_list->next;
		}
		fprintf(fh, "\tLENGTH_%s\n", aux->data.id);
		fprintf(fh, "};\n");
		fprintf(fc, "};\n");
		fprintf(fh, "extern const char *%s_names[LENGTH_%s];\n", aux->data.id, aux->data.id);
		fprintf(fh, "const char *get_%s_names(enum %s e);\n", aux->data.id, aux->data.id);
		fprintf(fc, "const char *get_%s_names(enum %s e) {\n"
					"	if (e >= 0 && e < LENGTH_%s)\n"
					"		return %s_names[e];\n}\n", aux->data.id, aux->data.id, aux->data.id, aux->data.id);
		aux = aux->next;
	}
}

char *preprocess_file(const char *f) {
	size_t linecap = 0;
	char token, *line = NULL, *filename = malloc(strlen(f) + 10);
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
		else if (token == '-') fprintf(file, "_");
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

void constants_n_objects(FILE *file, SymbolTable *st, Stack *stack, LinkedList *hl, char token) {
	LinkedList *tmplist = create_list(); 
	unsigned short count = 0;
	while (fscanf(file, "%c", &token) != EOF) { 
		if (token == ' ' || token == ')') {
			while (strcmp(top(stack), "(")) {
				insert(tmplist, top(stack));
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
					char *obj = list_to_str(tmplist);
					insert(hl, obj);
					free(obj);
					count++;
				}
				if (!is_empty_list(hl)) {
					add_st(st, "obj", count, hl);
					free_list(tmplist), free_list(hl);
				}
				free_list(tmplist), free_list(hl);
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
	free(tmplist);
	return;
}

void predicates(FILE *domain_file, FILE *domainc, FILE *domainh, SymbolTable *st, Stack *parenthesis_stack, char tokend) {
	FILE *tmpfilec = fopen("/tmp/tmpfilec", "a"), *tmpfileh = fopen("/tmp/tmpfileh", "a");
	create_enums(domainc, domainh, st);
	push(parenthesis_stack, '(');
	// count = quantos '?' em uma linha.
	unsigned short count = 0, count_p = 0;
	char str[100], tmpstr[30][100];
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		// str = nome do predicado.
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
		if (tokend == '?') {
			fscanf(domain_file, " %[^)|^ ]s", tmpstr[count_p]);
			if (count_p > 0) fprintf(tmpfilec, ", int %s", tmpstr[count_p]), fprintf(tmpfileh, ", int %s", tmpstr[count_p]);
			else fprintf(tmpfilec, "int %s", tmpstr[count_p]), fprintf(tmpfileh, "int %s", tmpstr[count_p]);
			count++, count_p++;
		}
		else if (tokend == '_') {
			char typename[100];
			obj_sentinel = 1;
			fscanf(domain_file, " %[^)|^ ]s", typename);
			for (int i = 0; i < count; i++)
				fprintf(domainc, "[%ld]", get_qtd(st, typename));
			count = 0;
		}
		else if (tokend == '(') {
			push(parenthesis_stack, tokend), fscanf(domain_file, "%[^)|^ ]s", str);
			fprintf(domainc, "bool %s", str);
			fprintf(tmpfileh, "bool checktrue_%s(", str);
			fprintf(tmpfilec, "bool checktrue_%s(", str);
		}
		else if (tokend == ')') {
			pop(parenthesis_stack);
			if (is_empty_stack(parenthesis_stack)) break;
			if (!obj_sentinel) {
				for (int i = 0; i < count; i++)
					fprintf(domainc, "[%ld]", get_qtd(st, "obj"));
				count = 0;
			}
			fprintf(tmpfilec, ") {\n\treturn %s", str);
			fprintf(tmpfileh, ");\n");
			for (int i = 0; i < count_p; i++)
				fprintf(tmpfilec, "[%s]", tmpstr[i]);
			count_p = 0, obj_sentinel = 0;
			fprintf(domainc, ";\n");
			fprintf(tmpfilec, ";\n}\n");
		}
	}
	fclose(tmpfilec), fclose(tmpfileh);
	cat("/tmp/tmpfilec", domainc), cat("/tmp/tmpfileh", domainh);
	remove("/tmp/tmpfilec"), remove("/tmp/tmpfileh");
	return;
}

void action(FILE *domain_file, FILE *domainc, FILE *domainh, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count) {
	LinkedList *ha = create_list();
	FILE *tmpfile = fopen("/tmp/tmpfile", "w");
	push(parenthesis_stack, '(');
	char act_name[100];
	fscanf(domain_file, "%s", act_name);
	fprintf(domainh, "void check_show_%s(void);\n", act_name);
	fprintf(tmpfile, "void check_show_%s(void) {\n\tstruct %s s;\n", act_name, act_name);
	fprintf(domainh, "struct %s {\n", act_name);
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		if (tokend == ' ' || tokend == '(')
			stack_to_list(domain, ha);
		if (strcmp_list(ha, ":parameters") == 0) {
			LinkedList *parameters_read = create_list();
			LinkedList *parameters = create_list();
			unsigned short int par_count = 0;
			while (fscanf(domain_file, "%c", &tokend)) {
				if ((tokend == ' ' || tokend == '(' || tokend == ')')) {
					stack_to_list(domain, parameters_read);
					if (!is_empty_list(parameters_read)) {
						if (parameters_read->head->data[0] == '_') {
							char type[100];
							fscanf(domain_file, "%[^)|^ ]s", type);
							while (!is_empty_list(parameters)) {
								fprintf(domainh, "\tenum %s %s;\n", type, parameters->head->data);
								for (int i = 0; i < par_count; i++) fprintf(tmpfile, "\t");
								fprintf(tmpfile, "\tfor (int i%d = 0; i%d < LENGTH_%s; i%d++) {\n", par_count, par_count, type, par_count);
								for (int i = 0; i < par_count; i++) fprintf(tmpfile, "\t");
								fprintf(tmpfile, "\t\ts.%s = i%d;\n", parameters->head->data, par_count);
								par_count++, remove_first(parameters);
							}
						}
						else {
							char *par = list_to_str(parameters_read);
							insert(parameters, par);
							free(par);
						}
					}
					free_list(parameters_read);
					if (tokend == ')') {
						while (!is_empty_list(parameters)) {
							fprintf(domainh, "\tenum obj %s;\n", parameters->head->data);
							for (int i = 0; i < par_count; i++) fprintf(tmpfile, "\t");
							fprintf(tmpfile, "\tfor (int i%d = 0; i%d < LENGTH_obj; i%d++) {\n", par_count, par_count, par_count);
							for (int i = 0; i < par_count; i++) fprintf(tmpfile, "\t");
							fprintf(tmpfile, "\t\ts.%s = i%d;\n", parameters->head->data, par_count);
							par_count++, remove_first(parameters);
						}
						break;
					}
				}
				else if (tokend != ' ' && tokend != '?') push(domain, tokend);
			}
			fprintf(domainh, "};\n");
			for (int i = 0; i < par_count; i++) fprintf(tmpfile, "\t");
			fprintf(tmpfile, "\tif (checktrue_%s(s)) show_%s();\n", act_name, act_name);
			for (int i = 0; i < par_count; i++) {
				for (int ii = par_count; ii > i; ii--) fprintf(tmpfile, "\t");
				fprintf(tmpfile, "}\n");
			}
			fprintf(tmpfile, "}\n");
			fclose(tmpfile);
			free_list(parameters_read), free_list(parameters);
			free(parameters_read), free(parameters);
		} else if (strcmp_list(ha, ":precondition") == 0) {
			fprintf(domainc, "bool checktrue_%s(struct %s s) {\n\treturn ", act_name, act_name);
			fprintf(domainh, "bool checktrue_%s(struct %s s);\n", act_name, act_name);
			LinkedList *precondition = create_list();
			Stack *operators = create_stack();
			char sigarg = 0, flag = 0, dummyflag = 0;
			while (fscanf(domain_file, "%c", &tokend)) {
				if ((tokend == ' ' || tokend == '(' || tokend == ')') && (amount(domain) != 2)) {
					stack_to_list(domain, precondition);
					if (!is_empty_list(precondition)) {
						if (precondition->head->data[0] != '?') {
							char sig = 0, i;
							for (i = 0; i < KEYWORDS_SIZE; i++) {
								if (strcmp_list(precondition, KEYWORDS[i]) == 0) {
									if (flag)
										if (i == 2) fprintf(domainc, " %s !(", KEYWORDS[top(operators)[0]]);
										else fprintf(domainc, " %s (", KEYWORDS[top(operators)[0]]);
									else
										if (i == 2) fprintf(domainc, "!(");
										else fprintf(domainc, "(");
									sig = 1, flag = 0;
									break;
								}
							}
							if (!sig && !dummyflag) {
								char *preid = list_to_str(precondition);
								if (flag) fprintf(domainc, " %s checktrue_%s(", KEYWORDS[top(operators)[0]], preid);
								else fprintf(domainc, "checktrue_%s(", preid);
								free(preid), flag = 0, dummyflag = 1;
							} else if (!sig && dummyflag) {
								char *arg = list_to_str(precondition);
								if (sigarg) fprintf(domainc, ", %s", arg);
								else fprintf(domainc, "%s", arg), sigarg = 1;
								free(arg);
							}
							push(operators, i);
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
					sigarg = 0, flag = 1, dummyflag = 0;
					pop(parenthesis_stack);
					pop(operators);
					if (amount(parenthesis_stack) == 1) break;
					fprintf(domainc, ")");
				}
				else if (tokend != ' ') push(domain, tokend);
				free_list(precondition);
			}
			fprintf(domainc, ");\n}\n");
			free(precondition), free_stack(operators);
		} else if (strcmp_list(ha, ":effect") == 0) {
			fprintf(domainc, "void apply_%s(struct %s s) {\n", act_name, act_name);
			fprintf(domainh, "void apply_%s(struct %s s);\n", act_name, act_name);
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
							free(predicate);
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
	cat("/tmp/tmpfile", domainc);
	remove("/tmp/tmpfile");
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
			fprintf(domainc, "[%s]", str);
		}
		else if (tokenp == '(') {
			push(parenthesis_stack, tokenp), fscanf(problem_file, "%[^)|^ ]s", str);
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

//void create_show_actions(FILE *domainc, FILE *domainh) {
//	fprintf(domainc, "void show_actions(void) {\n");
//	fprintf(domainh, "void show_actions(void);\n");
//	return;
//}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
	}
	char *domain_file_name = argv[1], *problem_file_name = argv[2];
	domain_file_name = preprocess_file(domain_file_name), problem_file_name = preprocess_file(problem_file_name);

	FILE *domain_file = fopen(domain_file_name, "r"), 
		 *problem_file = fopen(problem_file_name, "r"), 
		 *domainc = fopen("pddl.c", "w"),
		 *domainh = fopen("pddl.h", "w");
	if (domain_file == NULL) {
		perror("Error opening domain file");
		return 1;
	} if (problem_file == NULL) {
		perror("Erro opening problem file");
		return 1;
	} if (domainc == NULL ) {
		perror("Erro opening pddl.c");
		return 1;
	} if (domainh == NULL ) {
		perror("Erro opening pddl.h");
		return 1;
	}
	Stack *domain = create_stack(), *problem = create_stack(), *parenthesis_stack = create_stack();
	char tokend, tokenp;
	LinkedList *hd = create_list(), *hp = create_list(), *hl = create_list();
	SymbolTable *st = create_st();
	fprintf(domainc, "#include \"pddl.h\"\n\n");
	fprintf(domainh, "#ifndef PDDL_H\n#define PDDL_H\n#include <stdbool.h>\n#include <string.h>\n#define and &&\n#define or ||\n\n");

	// Problem parser :objects
	while (fscanf(problem_file, "%c", &tokenp)) {
		// Tratamento dos dados.
		if (tokenp == ' ') {
			stack_to_list(problem, hp);
			// Tratamento dos dados do  bloco "objects" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":objects") == 0) {
				constants_n_objects(problem_file, st, problem, hl, tokenp);
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
				constants_n_objects(domain_file, st, domain, hl, tokend);
			else if (strcmp_list(hd, ":predicates") == 0)
				predicates(domain_file, domainc, domainh, st, parenthesis_stack, tokend);
			else if (strcmp_list(hd, ":action") == 0)
				action(domain_file, domainc, domainh, domain, parenthesis_stack, tokend, act_count++);
			free_list(hd);
		}
		else push(domain, tokend); 
		if (tokend == ')' && strcmp(top(domain), "(") == 0)
			pop(domain);
	}

	// Problem parser :init
	fprintf(domainc, "void initialize(void) {\n");
	fprintf(domainh, "void initialize(void);\n");
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
	fprintf(domainc, "\treturn;\n}\n");

	/* FUNCOES REPL */
	//create_show_actions(domainc, domainh);

	fprintf(domainh, "#endif /* PDDL_H */\n");


	/* -----------free-n-close------------------- */
	fclose(domain_file), fclose(problem_file), fclose(domainc), fclose(domainh);
	free_stack(domain), free_stack(problem), free_stack(parenthesis_stack);
	free_list(hp), free_list(hd), free_list(hl);
	free(hp), free(hd), free(hl);
	free_st(st);
	remove(domain_file_name), remove(problem_file_name);
	free(domain_file_name), free(problem_file_name);
	return 0;
}
