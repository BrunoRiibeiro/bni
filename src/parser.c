#include <stdio.h>
#include <ctype.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

const char* KEYWORDS[] = {"and", "or", "not", "forall", "when"};
#define KEYWORDS_SIZE (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))
char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.

void create_forall_effect(FILE *toread, FILE *towrite) {
	char token, par_count = 0, types_count = 0, sig = 0, flag = 0, isnot = 1, ii = 0;
	Stack *parenthesis = create_stack(), *tokens = create_stack();
	LinkedList *word = create_list(), *types_list = create_list();
	// get forall types
	while (fscanf(toread, "%c", &token)) { 
		if (token == ' ' || token == '(' || token == ')') {
			stack_to_list(tokens, word);
			if (!is_empty_list(word)) {
				if (word->head->data[0] == '?') {
					char *type = list_to_str(word);
					insert(types_list, type+1);
					par_count++, free(type);
				}
				if (strcmp_list(word, "_") == 0) sig = 1;
				else if (sig == 1) {
					char *type = list_to_str(word);
					for (; ii < par_count; ii++) {
						for (int i = 0; i < ii; i++) fprintf(towrite, "\t");
						fprintf(towrite, "\tfor (int i%d = 0; i%d < LENGTH_%s; i%d++) {\n", ii, ii, type, ii);
					}
					free(type), sig = 0, types_count++;
				}
			}
		} else push(tokens, token);
		if (token == ')') {
			for (; ii < par_count; ii++) {
				for (int i = 0; i < ii; i++) fprintf(towrite, "\t");
				fprintf(towrite, "\tfor (int i%d = 0; i%d < LENGTH_obj; i%d++) {\n", ii, ii, ii);
			}
			break;
		}
		free_list(word);
	}
	free_list(word);
	while (fscanf(toread, "%c", &token)) { 
		if (token == ' ' || token == '(' || token == ')') {
			stack_to_list(tokens, word);
			if (!is_empty_list(word)) {
				flag = 0;
				if (word->head->data[0] != '?') {
					if (strcmp_list(word, "not") == 0) {
						isnot = 0, free_list(word);
						continue;
					} else if (strcmp_list(word, "forall") == 0) { 
						create_forall_effect(toread, towrite); 
						flag = 1, free_list(word);
						continue;
					} else if (strcmp_list(word, "and") == 0) {free_list(word); continue;}
					char *predicate = list_to_str(word);
					for (int i = 0; i < ii; i++) fprintf(towrite, "\t");
					fprintf(towrite, "\t%s", predicate);
					free(predicate);
				} else {
					//add args dos predicados ?<...>
					char *arg = list_to_str(word);
					int index = ii - 1 - search_on(types_list, arg+1);
					if (index == -1) fprintf(towrite, "[s.%s]", arg+1);
					else fprintf(towrite, "[i%d]", index);
					free(arg);
				}
			}
		} else push(tokens, token);
		if (token == '(') push(parenthesis, token);
		else if (token == ')') {
			pop(parenthesis);
			if (is_empty_stack(parenthesis)) break;
			if (flag == 0)
				fprintf(towrite, " = %d;\n", isnot), isnot = 1;
			flag = 1;
		}
		free_list(word);
	}
	for (int i = 0; i < ii; i++) {
		for (int iii = i; iii < ii; iii++) fprintf(towrite, "\t");
		fprintf(towrite, "}\n");
	}
	free_stack(parenthesis), free_stack(tokens);
	free(word);
}

void create_enums(FILE *fc, FILE *fh, SymbolTable *st) {
	Node_st *aux = st->head;
	while (aux) {
		fprintf(fh, "enum %s {\n", aux->data.id);
		fprintf(fc, "%sMap %s_map[LENGTH_%s] = {\n", aux->data.id, aux->data.id, aux->data.id);
		Node *obj_list = aux->data.list->head;
		while (obj_list) {
			fprintf(fh, "\t%s,\n", obj_list->data);
			fprintf(fc, "\t{\"%s\", %s},\n", obj_list->data, obj_list->data);
			obj_list = obj_list->next;
		}
		fprintf(fh, "\tLENGTH_%s\n", aux->data.id);
		fprintf(fh, "};\n");
		fprintf(fh, "typedef struct %sMap {\n"
				"	const char *str;\n"
				"	enum %s value;\n}%sMap;\n", aux->data.id, aux->data.id, aux->data.id);
		fprintf(fc, "};\n");
		fprintf(fh, "const char *get_%s_names(enum %s e);\n", aux->data.id, aux->data.id);
		fprintf(fc, "const char *get_%s_names(enum %s e) {\n"
				"	if (e >= 0 && e < LENGTH_%s)\n"
				"		return %s_map[e].str;\n"
				"	return NULL;\n}\n", aux->data.id, aux->data.id, aux->data.id, aux->data.id);
		fprintf(fc, "enum %s get_%s_enum(const char *s) {\n"
				"	for (int i = 0; %s_map[i].str != NULL; i++)\n"
				"		if (strcmp(s, %s_map[i].str) == 0)\n"
				"			return %s_map[i].value;\n"
				"	return LENGTH_%s;\n}\n", aux->data.id, aux->data.id, aux->data.id, aux->data.id, aux->data.id, aux->data.id);
		aux = aux->next;
	}
}

char *preprocess_file(const char *f) {
	size_t linecap = 0;
	char token, *line = NULL, *filename = malloc(strlen(f) + 10);
	const char *basename = strrchr(f, '/');
	if (basename == NULL) basename = f;
	else basename++;
	snprintf(filename, (strlen(f)+10), "/tmp/%s", basename);
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
		else if (token == '\t' || token == '\n') fprintf(file, " ");
		else if (token == '-') fprintf(file, "_");
		else fprintf(file, "%c", tolower(token));
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

void action(FILE *domain_file, FILE *domainc, FILE *domainh, FILE *tmpshow, FILE *tmpapply, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count) {
	LinkedList *ha = create_list();
	FILE *tmpfile_check_show = fopen("/tmp/tmpfile_check_show", "w");
	push(parenthesis_stack, '(');
	char act_name[100];
	fscanf(domain_file, "%s", act_name);
	fprintf(domainh, "void check_show_%s(void);\n", act_name);
	fprintf(tmpshow, "\tcheck_show_%s();\n", act_name);
	fprintf(tmpfile_check_show, "void check_show_%s(void) {\n\tstruct %s s;\n", act_name, act_name);
	fprintf(domainh, "struct %s {\n", act_name);
	fprintf(tmpapply, "\tif (strcmp(basename, \"%s\") == 0) {\n", act_name);
	fprintf(tmpapply, "\t\tstruct %s tcurts;\n", act_name);
	fprintf(tmpapply, "\t\tchar *token;\n");
	while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
		if (tokend == ' ' || tokend == '(')
			stack_to_list(domain, ha);
		if (strcmp_list(ha, ":parameters") == 0) {
			LinkedList *parameters_read = create_list();
			LinkedList *parameters = create_list();
			LinkedList *get_type_printf = create_list();
			LinkedList *apply_act = create_list();
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
								for (int i = 0; i < par_count; i++) fprintf(tmpfile_check_show, "\t");
								fprintf(tmpfile_check_show, "\tfor (int i%d = 0; i%d < LENGTH_%s; i%d++) {\n", par_count, par_count, type, par_count);
								for (int i = 0; i < par_count; i++) fprintf(tmpfile_check_show, "\t");
								fprintf(tmpfile_check_show, "\t\ts.%s = i%d;\n", parameters->head->data, par_count);
								size_t toalloc = strlen(parameters->head->data) + strlen(type) + 80;
								char *gtp = malloc(strlen(type) + 20), *stringao = malloc(toalloc);
								snprintf(gtp, (strlen(type)+20), ", get_%s_names(i%hd)", type, par_count);
								snprintf(stringao, toalloc, "\t\ttoken = strsep(&s, \",\");\n\t\ttcurts.%s = get_%s_enum(strsep(&token, \")\"));\n", parameters->head->data, type);
								insert(get_type_printf, gtp);
								insert(apply_act, stringao);
								par_count++, remove_first(parameters), free(gtp), free(stringao);
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
							for (int i = 0; i < par_count; i++) fprintf(tmpfile_check_show, "\t");
							fprintf(tmpfile_check_show, "\tfor (int i%d = 0; i%d < LENGTH_obj; i%d++) {\n", par_count, par_count, par_count);
							for (int i = 0; i < par_count; i++) fprintf(tmpfile_check_show, "\t");
							fprintf(tmpfile_check_show, "\t\ts.%s = i%d;\n", parameters->head->data, par_count);
							fprintf(tmpapply, "\t\ttoken = strsep(&s, \",\");\n");
							fprintf(tmpapply, "\t\ttcurts.%s = get_obj_enum(strsep(&token, \")\"));\n", parameters->head->data);
							size_t toalloc = strlen(parameters->head->data) + 85;
							char gtp[25], *stringao = malloc(toalloc);
							snprintf(gtp, 25, ", get_obj_names(i%hd)", par_count);
							snprintf(stringao, toalloc, "\t\ttoken = strsep(&s, \",\");\n\t\ttcurts.%s = get_obj_enum(strsep(&token, \")\"));\n", parameters->head->data);
							insert(get_type_printf, gtp);
							insert(apply_act, stringao), free(stringao);
							par_count++, remove_first(parameters);
							par_count++, remove_first(parameters);
						}
						break;
					}
				}
				else if (tokend != ' ' && tokend != '?') push(domain, tokend);
			}
			fprintf(domainh, "};\n");
			for (int i = 0; i < par_count; i++) fprintf(tmpfile_check_show, "\t");
			while (!is_empty_list(apply_act))
				fprintf(tmpapply, "%s", apply_act->head->data), remove_first(apply_act);
			fprintf(tmpfile_check_show, "\tif (checktrue_%s(s)) printf(\"  - %s(", act_name, act_name);
			fprintf(tmpapply, "\t\tif (checktrue_%s(tcurts)) {\n", act_name);
			fprintf(tmpapply, "\t\t\tapply_%s(tcurts);\n", act_name);
			fprintf(tmpapply, "\t\t\treturn 0;\n\t\t} else return 1;\n\t}\n");
			for (int i = 0; i < par_count; i++)
				if (i == 0) fprintf(tmpfile_check_show, "%%s");
				else fprintf(tmpfile_check_show, ",%%s");
			fprintf(tmpfile_check_show, ")\\n\"");
			while (!is_empty_list(get_type_printf))
				fprintf(tmpfile_check_show, "%s", get_type_printf->head->data), remove_first(get_type_printf);
			fprintf(tmpfile_check_show, ");\n");
			for (int i = 0; i < par_count; i++) {
				for (int ii = par_count; ii > i; ii--) fprintf(tmpfile_check_show, "\t");
				fprintf(tmpfile_check_show, "}\n");
			}
			fprintf(tmpfile_check_show, "}\n");
			fclose(tmpfile_check_show);
			free_list(parameters_read), free_list(parameters), free_list(get_type_printf), free_list(apply_act);
			free(parameters_read), free(parameters), free(get_type_printf), free(apply_act);
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
							} else if (strcmp_list(effect, "forall") == 0) {
								create_forall_effect(domain_file, domainc);
								flag = 1, free_list(effect); 
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
	cat("/tmp/tmpfile_check_show", domainc);
	remove("/tmp/tmpfile_check_show");
	free_list(ha), free(ha);
	return;
}

void init(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp) {
	push(parenthesis_stack, '(');
	fprintf(domainc, "void initialize(void) {\n");
	fprintf(domainh, "void initialize(void);\n");
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
	fprintf(domainc, "\treturn;\n}\n");
	return;
}

void goal(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp) {
	push(parenthesis_stack, '(');
	fprintf(domainc, "bool checktrue_goal(void) {\n\treturn ");
	fprintf(domainh, "bool checktrue_goal(void);\n");
	LinkedList *goal_cond = create_list();
	Stack *clauses = create_stack(), *operators = create_stack();
	char flag = 0, sigarg = 0;
	while (fscanf(problem_file, "%c", &tokenp) != EOF && !is_empty_stack(parenthesis_stack)) {
		if ((tokenp == ' ' || tokenp == '(' || tokenp == ')')) {
			stack_to_list(clauses, goal_cond);
			if (!is_empty_list(goal_cond)) {
				char i;
				for (i = 0; i < KEYWORDS_SIZE; i++) {
					if (strcmp_list(goal_cond, KEYWORDS[i]) == 0) {
						if (flag)
							if (i == 2) fprintf(domainc, " %s !(", KEYWORDS[top(operators)[0]]);
							else fprintf(domainc, " %s (", KEYWORDS[top(operators)[0]]);
						else
							if (i == 2) fprintf(domainc, "!(");
							else fprintf(domainc, "(");
						flag = 0;
						break;
					}
				}
				if (i == KEYWORDS_SIZE) {
					char str[256], *cond = list_to_str(goal_cond);
					if (flag) fprintf(domainc, " %s checktrue_%s(", KEYWORDS[top(operators)[0]], cond);
					else fprintf(domainc, "checktrue_%s(", cond);
					do {
						if (tokenp == ' ') {
							fscanf(problem_file, "%[^)|^ ]s", str);
							if (sigarg) fprintf(domainc, ", %s", str);
							else fprintf(domainc, "%s", str), sigarg = 1;
						}
						else if (tokenp == ')') break;
					} while (fscanf(problem_file, "%c", &tokenp));
					free(cond);
				}
				push(operators, i);
			}
			free_list(goal_cond);
		}
		else push(clauses, tokenp);
		if (tokenp == '(') push(parenthesis_stack, tokenp);
		else if (tokenp == ')') {
			sigarg = 0, flag = 1;
			pop(parenthesis_stack);
			pop(operators);
			if (is_empty_stack(parenthesis_stack)) break;
			fprintf(domainc, ")");
		}
	}
	free_stack(operators), free_stack(clauses);
	free_list(goal_cond), free(goal_cond);
	fprintf(domainc, ";\n}\n");
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
		 *domainc = fopen("pddl.c", "w"),
		 *domainh = fopen("pddl.h", "w"),
		 *tmpshow = fopen("/tmp/tmpshow", "w"),
		 *tmpapply = fopen("/tmp/tmpapply", "w");
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
	} if (tmpshow == NULL ) {
		perror("Erro opening /tmp/tmpshow");
		return 1;
	} if (tmpapply == NULL ) {
		perror("Erro opening /tmp/tmpapply");
		return 1;
	}

	Stack *domain = create_stack(), *problem = create_stack(), *parenthesis_stack = create_stack();
	char tokend, tokenp;
	LinkedList *hd = create_list(), *hp = create_list(), *hl = create_list();
	SymbolTable *st = create_st();
	fprintf(domainc, "#include <stdio.h>\n#include \"pddl.h\"\n\n");
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
	free_list(hp);

	// Domain parser
	obj_sentinel = 0;	
	int act_count = 0;
	while (fscanf(domain_file, "%c", &tokend) != EOF) { 
		// Inicia o tratamento dos dados do domínio.
		if (tokend == ' ' || tokend == '(' || tokend == ')') {
			stack_to_list(domain, hd);
			if (strcmp_list(hd, ":constants") == 0)
				constants_n_objects(domain_file, st, domain, hl, tokend);
			else if (strcmp_list(hd, ":predicates") == 0)
				predicates(domain_file, domainc, domainh, st, parenthesis_stack, tokend);
			else if (strcmp_list(hd, ":action") == 0)
				action(domain_file, domainc, domainh, tmpshow, tmpapply, domain, parenthesis_stack, tokend, act_count++);
			free_list(hd);
		}
		else push(domain, tokend); 
		if (tokend == '(') push(domain, tokend);
		if (tokend == ')' && strcmp(top(domain), "(") == 0)
			pop(domain);
	}
	fclose(tmpshow), fclose(tmpapply);
	fprintf(domainh, "void show_actions(void);\n");
	fprintf(domainc, "void show_actions(void) {\n");
	cat("/tmp/tmpshow", domainc);
	remove("/tmp/tmpshow");
	fprintf(domainc, "}\n");
	fprintf(domainh, "int apply_actions(char *s);\n");
	fprintf(domainc, "int apply_actions(char *s) {\n");
	fprintf(domainc, "\tconst char *basename = strsep(&s, \"(\");\n");
	cat("/tmp/tmpapply", domainc);
	remove("/tmp/tmpapply");
	fprintf(domainc, "\treturn 2;\n}\n");

	// Problem parser
	while (fscanf(problem_file, "%c", &tokenp) != EOF) {
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '(' || tokenp == ')') {
			stack_to_list(problem, hp);
			if (!is_empty_list(hp) && strcmp_list(hp, ":init") == 0)
				init(problem_file, domainc, domainh, parenthesis_stack, tokenp);
			else if (!is_empty_list(hp) && strcmp_list(hp, ":goal") == 0)
				goal(problem_file, domainc, domainh, parenthesis_stack, tokenp);
			free_list(hp);
		}
		else push(problem, tokenp);
		if (tokenp == '(') push(problem, tokenp);
		if (tokenp == ')' && strcmp(top(problem), "(") == 0)
			pop(problem);
	}
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
