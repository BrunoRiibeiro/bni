#include "parser.h"

const char *OPERATORS[] = {"and", "or", "not", "forall", "when"};
char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.

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
	fprintf(domainh, "void check_show_actions(const char *filename);\n");
	fprintf(domainc, "void check_show_actions(const char *filename) {\n");
	fprintf(domainc, "\tFILE *f = fopen(filename, \"w\");\n");
	cat("/tmp/tmpshow", domainc);
	remove("/tmp/tmpshow");
	fprintf(domainc, "\tfclose(f);\n}\n");
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

int create_fors(FILE *toread, FILE *towrite, LinkedList *types_list, int par_count) {
	char token, sig = 0, i = par_count;
	Stack *tokens = create_stack();
	LinkedList *word = create_list();
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
					for (; i < par_count; i++) {
						for (int ii = 0; ii < i; ii++) fprintf(towrite, "\t");
						fprintf(towrite, "\tfor (int i%d = 0; i%d < LENGTH_%s; i%d++) {\n", i, i, type, i);
					}
					free(type), sig = 0;
				}
			}
		} else push(tokens, token);
		if (token == ')') {
			for (; i < par_count; i++) {
				for (int ii = 0; ii < i; ii++) fprintf(towrite, "\t");
				fprintf(towrite, "\tfor (int i%d = 0; i%d < LENGTH_obj; i%d++) {\n", i, i, i);
			}
			break;
		}
		free_list(word);
	}
	free_list(word);
	return par_count;
}

void write_operator(FILE *towrite, Stack *operators, int operator, int *forall_id, char flag) {
	if (flag)
		if (operator == 2) fprintf(towrite, " %s !(", OPERATORS[top(operators)[0]]);
		else if (operator == 3) fprintf(towrite, " %s (forall%d", OPERATORS[top(operators)[0]], ++(*forall_id));
		else fprintf(towrite, " %s (", OPERATORS[top(operators)[0]]);
	else
		if (operator == 2) fprintf(towrite, "!(");
		else if (operator == 3) fprintf(towrite, "(forall%d", ++(*forall_id));
		else fprintf(towrite, "(");
}

int is_operator(LinkedList *word) {
	int i;
	for (i = 0; i < OPERATORS_SIZE; i++)
		if (strcmp_list(word, OPERATORS[i]) == 0) return i;
	return -1;
}

void create_forall_pre_goal(FILE *toread, FILE *towrite, int forall_id, int op_args, ...) {
	for (int i = 0; i < forall_id; i++) fprintf(towrite,  "\t");
	fprintf(towrite, "bool forall%d = true;\n", forall_id);
	char token, par_count = 0, flags[5] = {0,0,0,0,0};
	/* flags map
	 * [0]: flag = padrao é zero toda vez que um ) torna-se igual 1
	 * [1]: dummyflag = sao ou nao sao os argumentos de um checktrue
	 * [2]: sigarg = flag dos argumento: é ou não é o primeiro argumento
	 * [3]: has_empty =
	 * [4]: has_to_verify =
	 */
	Stack *parenthesis = create_stack(), *tokens = create_stack(), *operators = create_stack();
	LinkedList *word = create_list(), *types_list;
	va_list args;
	va_start(args, op_args);
	if (op_args >= 1) par_count = va_arg(args, int);
	if (op_args >= 2) types_list = va_arg(args, LinkedList*);
	else types_list = create_list();
	va_end(args);
	char fixed = par_count, filename[256];
	par_count = create_fors(toread, towrite, types_list, par_count);
	sprintf(filename, "/tmp/forall%d", forall_id);
	FILE *forfile = fopen(filename, "w");
	for (int i = 0; i < forall_id; i++) fprintf(forfile,  "\t");
	fprintf(forfile, "\tif (!");
	while (fscanf(toread, "%c", &token)) {
		if (token == ' ' || token == '(' || token == ')') {
			stack_to_list(tokens, word);
			if (!is_empty_list(word)) {
				if (word->head->data[0] != '?') {
					int operator = is_operator(word);
					if (operator != -1) {
						write_operator(forfile, operators, operator, &forall_id, flags[0]);
						if (operator == 3) create_forall_pre_goal(toread, towrite, forall_id, 2, par_count, types_list), flags[4] = 1;
						flags[0] = 0, flags[3] = 1;
					} else if (operator == -1 && !flags[1]) {
						char *preid = list_to_str(word);
						if (flags[0]) fprintf(forfile, " %s checktrue_%s(", OPERATORS[top(operators)[0]], preid);
						else fprintf(forfile, "checktrue_%s(", preid);
						free(preid), flags[0] = 0, flags[1] = 1, flags[4] = 1;
					} else if (operator == -1 && flags[1]) {
						char *arg = list_to_str(word);
						if (flags[2]) fprintf(forfile, ", %s", arg);
						else fprintf(forfile, "%s", arg), flags[2] = 1;
						pop(operators);
						free(arg);
					}
					push(operators, operator);
				} else {
					//add args dos predicados ?<...>
					char *arg = list_to_str(word);
					int index = par_count - 1 - search_on(types_list, arg+1);
					if (index == -1)
						if (flags[2]) fprintf(forfile, ", s.%s", arg+1);
						else fprintf(forfile, "s.%s", arg+1), flags[2] = 1;
					else
						if (flags[2]) fprintf(forfile, ", i%d", index);
						else fprintf(forfile, "i%d", index), flags[2] = 1;
					free(arg);
				}
			}
		}
		if (token == '(') push(parenthesis, token);
		else if (token == ')') {
			flags[2] = 0, flags[0] = 1, flags[1] = 0;
			pop(parenthesis);
			pop(operators);
			if (is_empty_stack(parenthesis)) break;
			fprintf(forfile, ")");
		}
		else if (token != ' ') push(tokens, token);
		free_list(word);
	}
	fclose(forfile);
	cat(filename, towrite);
	//remove(filename);
	if (flags[4]) fprintf(towrite, "))");
	else if (flags[3]) fprintf(towrite, "true))");
	else fprintf(towrite, "true)");
	fprintf(towrite, " forall%d = false;\n", forall_id);
	for (int i = fixed; i < par_count; i++) {
		for (int ii = 0; ii < par_count; ii++) fprintf(towrite, "\t");
		fprintf(towrite, "}\n");
	}
	free_stack(operators), free_stack(parenthesis), free_stack(tokens);
	free(word);
	return;
}

void create_forall_effect(FILE *toread, FILE *towrite, int op_args, ...) {
	char token, par_count = 0, sig = 0, flag = 0, isnot = 1;
	Stack *parenthesis = create_stack(), *tokens = create_stack();
	LinkedList *word = create_list(), *types_list;
	va_list args;
	va_start(args, op_args);
	if (op_args >= 1) par_count = va_arg(args, int);
	if (op_args >= 2) types_list = va_arg(args, LinkedList*);
	else types_list = create_list();
	va_end(args);
	char fixed = par_count;
	par_count = create_fors(toread, towrite, types_list, par_count);
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
						create_forall_effect(toread, towrite, 2, par_count, types_list); 
						flag = 1, free_list(word);
						continue;
					} else if (strcmp_list(word, "and") == 0) {free_list(word); continue;}
					char *predicate = list_to_str(word);
					for (int i = 0; i < par_count; i++) fprintf(towrite, "\t");
					fprintf(towrite, "\t%s", predicate);
					free(predicate);
				} else {
					//add args dos predicados ?<...>
					char *arg = list_to_str(word);
					int index = par_count - 1 - search_on(types_list, arg+1);
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
	for (int i = fixed; i < par_count; i++) {
		for (int ii = 0; ii < par_count; ii++) fprintf(towrite, "\t");
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
	fprintf(domainh, "void check_show_%s(FILE *f);\n", act_name);
	fprintf(tmpshow, "\tcheck_show_%s(f);\n", act_name);
	fprintf(tmpfile_check_show, "void check_show_%s(FILE *f) {\n\tstruct %s s;\n", act_name, act_name);
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
			fprintf(tmpfile_check_show, "\tif (checktrue_%s(s)) fprintf(f, \"%s(", act_name, act_name);
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
			fprintf(domainc, "bool checktrue_%s(struct %s s) {\n", act_name, act_name);
			fprintf(domainh, "bool checktrue_%s(struct %s s);\n", act_name, act_name);
			FILE *toreturn = fopen("/tmp/toreturn", "w");
			fprintf(toreturn, "\treturn ");
			LinkedList *precondition = create_list();
			Stack *operators = create_stack();
			int forall_id = 0;
			char flags[5] = {0,0,0,0,0};
			/* flags map
			 * [0]: flag = padrao é zero toda vez que um ) torna-se igual 1
			 * [1]: dummyflag = sao ou nao sao os argumentos de um checktrue
			 * [2]: sigarg = flag dos argumento: é ou não é o primeiro argumento
			 * [3]: has_empty =
			 * [4]: has_to_verify =
			 */
			while (fscanf(domain_file, "%c", &tokend)) {
				if ((tokend == ' ' || tokend == '(' || tokend == ')') && (amount(domain) != 2)) {
					stack_to_list(domain, precondition);
					if (!is_empty_list(precondition)) {
						if (precondition->head->data[0] != '?') {
							int operator = is_operator(precondition);
							if (operator != -1) {
								write_operator(toreturn, operators, operator, &forall_id, flags[0]);
								if (operator == 3) create_forall_pre_goal(domain_file, domainc, forall_id++, 0), flags[4] = 1;
								flags[0] = 0, flags[3] = 1;
							} else if (operator == -1 && !flags[1]) {
								char *preid = list_to_str(precondition);
								if (flags[0]) fprintf(toreturn, " %s checktrue_%s(", OPERATORS[top(operators)[0]], preid);
								else fprintf(toreturn, "checktrue_%s(", preid);
								free(preid), flags[0] = 0, flags[1] = 1, flags[4] = 1;
							} else if (operator == -1 && flags[1]) {
								char *arg = list_to_str(precondition);
								if (flags[2]) fprintf(toreturn, ", %s", arg);
								else fprintf(toreturn, "%s", arg), flags[2] = 1;
								pop(operators);
								free(arg);
							}
							push(operators, operator);
						} else {
							//add args dos predicados ?<...>
							char *arg = list_to_str(precondition);
							if (flags[2]) fprintf(toreturn, ", s.%s", arg+1);
							else fprintf(toreturn, "s.%s", arg+1), flags[2] = 1;
							free(arg);
						}
					}
				}
				if (tokend == '(') push(parenthesis_stack, tokend);
				else if (tokend == ')') {
					flags[2] = 0, flags[0] = 1, flags[1] = 0;
					pop(parenthesis_stack);
					pop(operators);
					if (amount(parenthesis_stack) == 1) break;
					fprintf(toreturn, ")");
				}
				else if (tokend != ' ') push(domain, tokend);
				free_list(precondition);
			}
			fclose(toreturn);
			cat("/tmp/toreturn", domainc);
			remove("/tmp/toreturn");
			if (flags[4]) fprintf(domainc, ");\n}\n");
			else if (flags[3]) fprintf(domainc, "true);\n}\n");
			else fprintf(domainc, "true;\n}\n");
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
								create_forall_effect(domain_file, domainc, 0);
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
	fprintf(domainc, "bool checktrue_goal(void) {\n");
	fprintf(domainh, "bool checktrue_goal(void);\n");
	LinkedList *goal_cond = create_list();
	Stack *clauses = create_stack(), *operators = create_stack();
	int forall_id = 0;
	char flags[2] = {0,0};
	/* flags map
	 * [0]: flag = padrao é zero toda vez que um ) torna-se igual 1
	 * [1]: sigarg = flag dos argumento: é ou não é o primeiro argumento
	 */
	FILE *toreturn = fopen("/tmp/toreturn", "w");
	fprintf(toreturn, "\treturn ");
	while (fscanf(problem_file, "%c", &tokenp) != EOF && !is_empty_stack(parenthesis_stack)) {
		if ((tokenp == ' ' || tokenp == '(' || tokenp == ')')) {
			stack_to_list(clauses, goal_cond);
			if (!is_empty_list(goal_cond)) {
				int operator = is_operator(goal_cond);
				if (operator != -1) {
					write_operator(toreturn, operators, operator, &forall_id, flags[0]);
					if (operator == 3) create_forall_pre_goal(problem_file, domainc, forall_id++, 0);
					flags[0] = 0;
				} else {
					char str[256], *cond = list_to_str(goal_cond);
					if (flags[0]) fprintf(toreturn, " %s checktrue_%s(", OPERATORS[top(operators)[0]], cond);
					else fprintf(toreturn, "checktrue_%s(", cond);
					do {
						if (tokenp == ' ') {
							fscanf(problem_file, "%[^)|^ ]s", str);
							if (flags[1]) fprintf(toreturn, ", %s", str);
							else fprintf(toreturn, "%s", str), flags[1] = 1;
						}
						else if (tokenp == ')') break;
					} while (fscanf(problem_file, "%c", &tokenp));
					free(cond);
				}
				push(operators, operator);
			}
			free_list(goal_cond);
		}
		else push(clauses, tokenp);
		if (tokenp == '(') push(parenthesis_stack, tokenp);
		else if (tokenp == ')') {
			flags[1] = 0, flags[0] = 1;
			pop(parenthesis_stack);
			pop(operators);
			if (is_empty_stack(parenthesis_stack)) break;
			fprintf(toreturn, ")");
		}
	}
	fclose(toreturn);
	cat("/tmp/toreturn", domainc);
	remove("/tmp/toreturn");
	free_stack(operators), free_stack(clauses);
	free_list(goal_cond), free(goal_cond);
	fprintf(domainc, ";\n}\n");
	return;
}
