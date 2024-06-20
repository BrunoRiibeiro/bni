#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

const char* KEYWORDS[] = {"AND", "OR", "NOT"};
#define KEYWORDS_SIZE (sizeof(KEYWORDS) / sizeof(KEYWORDS[0]))

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


int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <domain_file> <problem_file>\n", argv[0]);
		return 1;
    }
	char *domain_file_name = argv[1], *problem_file_name = argv[2];
	
	FILE *domain_file = fopen(domain_file_name, "r"), *problem_file = fopen(problem_file_name, "r"), *domainc = fopen("pddl.c", "w"), *tmpfile = fopen("tmpfile", "w");
	if (domain_file == NULL) {
		perror("Error opening domain file");
		return 1;
	} if (problem_file == NULL) {
		perror("Erro opening problem file");
		return 1;
	} if (domainc == NULL ) {
		perror("Erro opening problem file");
		return 1;
	} if (domainc == NULL ) {
		perror("Erro opening problem file");
		return 1;
	}

	Stack *domain = create_stack(), *problem = create_stack(), *parenthesis_stack = create_stack(); // Stacks para tokenizar arquivos de domínio e problemas. 'p_aux' é usado para fazer a correspondência de parênteses em ':predicates'.
	char tokend, tokenp; // Variáveis de token para arquivos de domínio e problemas.
	LinkedList *hd = create_list(), *hp = create_list(), *hl = create_list(); // Linked lists para leitura de strings em arquivos de domínio e problemas.
	SymbolTable *st = create_st();
	char *buffer = NULL; // Buffer para a função getline() em comentários ignorados.
	size_t size_allocated = 0; // Tamanho alocado para a função getline() em comentários ignorados.
	char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.
	fprintf(domainc, "#include <stdbool.h>\n#include \"ntree.h\"\n\n");

	// Problem parser :objects

	while (fscanf(problem_file, "%c", &tokenp)) {
		// Ignorar os comentário.
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			while (!is_empty_stack(problem) && strcmp(top(problem), "(")) 
				insert(hp, top(problem)), pop(problem);
			// Tratamento dos dados do  bloco "objects" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":objects") == 0) {
				for ( ; ; ) {
					// count = how many objects are in one line.
					// obj = object type string.
					char obj[100];
					unsigned short count = 0;
					while (fscanf(problem_file, "%s", obj) && obj[0] != '-' && obj[0] != ')') {
						hifen_to_underscore(obj);
						insert(hl, obj);
						count++; 
					} 
					// Verifica se existe um tipo dos objetos lidos.
					if (obj[0] == '-') obj_sentinel = 1; 
					// Caso nao tenha declarado nenhum tipo de objeto, define objeto padrao como "obj". 
					if (!obj_sentinel) {
						add_st(st, "obj", count, hl); 
						free_list(hl);
						break;
					}
					// Bloco de :objects termina com um '\n' antes do ')'.
					if (obj[0] == ')') break;
					// Ler até encontrar ")" ou espaco ou quebra de linha ou tab.
					fscanf(problem_file, " %[^)|^ |^\n|^\t]s", obj);
					// Adiciona ou aumenta a quantidade de repeticoes de um tipo de objeto a tabela de simbolos.
					add_st(st, obj, count, hl);
					free_list(hl);
					if (fscanf(problem_file, "%c", &tokenp) && tokenp == ')') break;
				}
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
	while (fscanf(domain_file, "%c", &tokend) != EOF) { 
		// Ignora os comentários.
		if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		// Inicia o tratamento dos dados do domínio.
		if (tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == ')') {
			while (strcmp(top(domain), "("))
				insert(hd, top(domain)), pop(domain);
			printf("%d\n", amount(domain));
			print_list(hd);
			if (strcmp_list(hd, ":constants") == 0) {
				for ( ; ; ) {
					// count = how many constants are in one line.
					// obj = object type string.
					char obj[100];
					unsigned short count = 0;
					while (fscanf(domain_file, "%s", obj) && obj[0] != '-' && obj[0] != ')') {
						hifen_to_underscore(obj);
						insert(hl, obj);
						count++;
					}
					// Verifica se existe um tipo de constante lidos.
					if (obj[0] == '-') obj_sentinel = 1;
					// Caso nao tenha declarado nenhum tipo de objeto, define objeto padrao como "obj".
					if (!obj_sentinel) {
						add_st(st, "obj", count, hl);
						free_list(hl);
						break;
					}
					// Bloco de :constants termina com um '\n' antes do ')'.
					if (obj[0] == ')') {
						if (strcmp(top(domain), "(") == 0)
							pop(domain); 
						break;
					}
					// Ler até encontrar ")" ou espaco ou quebra de linha ou tab.
					fscanf(domain_file, " %[^)|^ |^\n|^\t]s", obj);
					// Adiciona ou aumenta a quantidade de repeticoes de um tipo de objeto a tabela de simbolos.
					add_st(st, obj, count, hl);
					free_list(hl);
					if (fscanf(domain_file, "%c", &tokend) && tokend == ')') break;
				}
			}
			// Tratamento dos dados do  bloco "predicates".
			else if (strcmp_list(hd, ":predicates") == 0) {
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
			}
			else if (strcmp_list(hd, ":action") == 0) {
				LinkedList *ha = create_list();
				push(parenthesis_stack, '(');
				char str[100];
				fscanf(domain_file, "%s", str);
				hifen_to_underscore(str);
				fprintf(domainc, "struct %s {\n", str);
				while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(parenthesis_stack)) {
					if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
					if (tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == '(')
						while (strcmp(top(domain), "("))
							insert(ha, top(domain)), pop(domain);
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
						int precon_count = 0;
						fprintf(domainc, "\tNtree *precon;\n");
						fprintf(tmpfile, "Ntree *precon = create_tree(15);\n");
						while (fscanf(domain_file, "%c", &tokend)) {
							if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
							if ((tokend == ' ' || tokend == '\n' || tokend == '\t' || tokend == '(') && (amount(domain) != 3)){
								while (strcmp(top(domain), "("))
									insert(precondition, top(domain)), pop(domain);
								print_list(precondition);
								set_uppercase(precondition);
								fprintf(tmpfile, "Data d%d;\n", precon_count);
								fprintf(tmpfile, "d%d.index = %d;\n", precon_count, precon_count);
								char *preid = list_to_str(precondition);
								fprintf(tmpfile, "strcpy(d%d.id, \"%s\");\n", precon_count, preid), free(preid);
								for (int i = 0; i < KEYWORDS_SIZE; i++) {	
									if (strcmp_list(precondition, KEYWORDS[i]) == 0)
										fprintf(tmpfile, "d%d.t = %s;\n", precon_count, KEYWORDS[i]);
								}
								precon_count++;
							}	
							if (tokend == '(') push(parenthesis_stack, tokend);
							else if (tokend == ')') {
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
				free_list(ha);
				free(ha);
			}
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
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			// Transfere os tokens da pilha para a lista, para assim serem lidos como string (strcmp_list).
			while (!is_empty_stack(problem) && strcmp(top(problem), "("))
				insert(hp, top(problem)), pop(problem);
			// Tratamento dos dados do  bloco ":init" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":init") == 0) {
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
						fprintf(domainc, "\t%s", str);
					}
					else if (tokenp == ')') {
						pop(parenthesis_stack);
						if (is_empty_stack(parenthesis_stack)) break;
						fprintf(domainc, " = true;\n");
					}
				}
			}
			free_list(hp);
		}
		if (tokenp != ' ' && tokenp != '\t' && tokenp != '\n')
			push(problem, tokenp);
	}
	fprintf(domainc, "\treturn 0;\n}\n");


	/* -----------free-n-close------------------- */
	fclose(domain_file), fclose(problem_file), fclose(domainc), fclose(tmpfile);
	free_stack(domain), free_stack(problem), free_stack(parenthesis_stack);
	free_list(hp), free_list(hd), free_list(hl);
	free(hp), free(hd), free(hl);
	free_st(st);
	free(buffer);
	return 0;
}
