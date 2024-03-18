#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

void create_enums(SymbolTable *st) {
	Node_st *aux = st->head;
	char buff[100];
	while (aux) {
		sprintf(buff, "printf 'enum %s {\n' >> domain.c", aux->data.id);
		system(buff);
		Node_str *obj_list = aux->data.list.head;
		while (obj_list) {
			sprintf(buff, "printf '\t%s,\n' >> domain.c", obj_list->data), system(buff);
			obj_list = obj_list->next;
		}
		system("echo '};' >> domain.c");
		aux = aux->next;
	}
}

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

	Stack domain, problem, p_aux; // Stacks para tokenizar arquivos de domínio e problemas. 'p_aux' é usado para fazer a correspondência de parênteses em ':predicates'.
	create_stack(&domain, 1000), create_stack(&problem, 1000), create_stack(&p_aux, 100);
	Item tokend, tokenp; // Variáveis de token para arquivos de domínio e problemas.
	LinkedList *hd = create_list(), *hp = create_list(); // Linked lists para leitura de strings em arquivos de domínio e problemas.
	LinkedList_str *hl = create_list_str();
	SymbolTable *st = create_st();
	char *buffer = NULL; // Buffer para a função getline() em comentários ignorados.
	size_t size_allocated = 0; // Tamanho alocado para a função getline() em comentários ignorados.
	char obj_sentinel = 0; // Sentinela para objetos que nao tem tipo.

	// Problem parser :objects

	while (fscanf(problem_file, "%c", &tokenp)) {
		// Ignorar os comentário.
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			while (!is_empty_stack(&problem) && top(&problem) != '(')
				insert_list(hp, top(&problem)), pop(&problem);
			// Tratamento dos dados do  bloco "objects" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":objects") == 0) {
				for ( ; ; ) {
					// count = how many objects are in one line.
					// obj = object type string.
					char count = 0, obj[100], buff[100];
					while (fscanf(problem_file, "%s", obj) && obj[0] != '-' && obj[0] != ')') {
						insert_list_str(hl, obj);
						count++;
					}
					// Verifica se existe um tipo dos objetos lidos.
					if (obj[0] == '-') obj_sentinel = 1;
					// Caso nao tenha declarado nenhum tipo de objeto, define objeto padrao como "obj".
					if (!obj_sentinel) {
						sprintf(obj, "obj"), add_st(st, obj, count, hl); 
						free_list_str(hl);
						break;
					}
					// Bloco de :objects termina com um '\n' antes do ')'.
					if (obj[0] == ')') break;
					// Ler até encontrar ")" ou espaco ou quebra de linha ou tab.
					fscanf(problem_file, " %[^)|^ |^\n|^\t]s", obj);
					// Adiciona ou aumenta a quantidade de repeticoes de um tipo de objeto a tabela de simbolos.
					add_st(st, obj, count, hl);
					free_list_str(hl);
					if (fscanf(problem_file, "%c", &tokenp) && tokenp == ')') break;
				}
				print_st(st);
				create_enums(st);
				// Pausar a leitura do problema depois de terminar o tratamento de dados do "objects".
				break;
			}
			free_list(hp);
		}
		push(&problem, tokenp);
	}

	// Domain parser
	
	while (fscanf(domain_file, "%c", &tokend) != EOF) {
		// Ignora os comentários.
			if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
		// Inicia o tratamento dos dados do domínio.
		if (tokend == ' ' || tokend == '\n' || tokend == '\t') {
			while (top(&domain) != '(')
				insert_list(hd, top(&domain)),	pop(&domain);
			// Tratamento dos dados do  bloco "predicates".
			if (strcmp_list(hd, ":predicates") == 0) {
				push(&p_aux, '(');
				// count = quantos '?' em uma linha.
				char count = 0;
				while (fscanf(domain_file, "%c", &tokend) && !is_empty_stack(&p_aux)) {
					// Ignora comentários dentro do bloco.
					if (tokend == ';') getline(&buffer, &size_allocated, domain_file);
					// str = nome do predicado.
					// buff = string para ser printada no arquivo pela função system.
					char str[100], buff[100];
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
						sprintf(buff, "printf '[%lld]' >> domain.c", get_qtd(st, str));
						for (int i = 0; i < count; i++)
							system(buff);
						count = 0;
					}
					else if (tokend == '(') {
						push(&p_aux, tokend), fscanf(domain_file, "%[^)|^ ]s", str);
						sprintf(buff, "printf 'bool %s' >> domain.c", str);
						system(buff);
					}
					else if (tokend == ')') {
						pop(&p_aux);
						if (is_empty_stack(&p_aux)) break;
						else if (!obj_sentinel) {
							sprintf(str, "obj"); 
							sprintf(buff, "printf '[%lld]' >> domain.c", get_qtd(st, str));
							for (int i = 0; i < count; i++)
								system(buff);
							count = 0;
						}
						system("echo ';' >> domain.c");
					}
				}
			}
			free_list(hd);
		}
		push(&domain, tokend);
	}


	// Problem parser :init

	system("echo 'int main(void) {' >> domain.c");
	while (fscanf(problem_file, "%c", &tokenp) != EOF) {
		// Ignorar os comentário.
		if (tokenp == ';') getline(&buffer, &size_allocated, problem_file);
		// Tratamento dos dados.
		if (tokenp == ' ' || tokenp == '\n' || tokenp == '\t') {
			// Transfere os tokens da pilha para a lista, para assim serem lidos como string (strcmp_list).
			while (!is_empty_stack(&problem) && top(&problem) != '(')
				insert_list(hp, top(&problem)), pop(&problem);
			// Tratamento dos dados do  bloco ":init" até encontrar ")".
			if (!is_empty_list(hp) && strcmp_list(hp, ":init") == 0) {
				push(&p_aux, '(');
				while (fscanf(problem_file, "%c", &tokenp) && !is_empty_stack(&p_aux)) {
					// Ignora comentários dentro do bloco.
					if (tokenp == ';') getline(&buffer, &size_allocated, domain_file);
					// str = nome do predicado.
					// buff = string para ser printada no arquivo pela função system.
					char str[100], buff[100];
					if (tokenp == ' ' && amount(&p_aux) == 2) {
						fscanf(problem_file, "%[^)|^ ]s", str);
						sprintf(buff, "printf '[%s]' >> domain.c", str);
						system(buff);

					}
					else if (tokenp == '(') {
						push(&p_aux, tokenp), fscanf(problem_file, "%[^)|^ ]s", str);
						sprintf(buff, "printf '\t%s' >> domain.c", str);
						system(buff);
					}
					else if (tokenp == ')') {
						pop(&p_aux);
						if (is_empty_stack(&p_aux)) break;
						system("echo ' = true;' >> domain.c");
					}
				}
			}
			free_list(hp);
		}
		push(&problem, tokenp);
	}
	system("echo '\treturn 0;\n}' >> domain.c");
	fclose(domain_file), fclose(problem_file);
	return 0;
}
