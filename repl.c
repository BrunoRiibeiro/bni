#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pddl.h"


typedef struct Map {
    char *name;
    void (*func)();
}Map;

int main(void) {
	Map functions[] = {
		{"mover", apply_mover},
		{"comprardindin1", apply_comprardindin1},
		{"comprardindin2", apply_comprardindin2},
		{"comprardindin3", apply_comprardindin3},
		{NULL, NULL}
	};
	initialize();
	char *input;
	while (1) {
		show_actions();
		input = readline("Digite a acao desejada: ");
		if (!input) break;
		if (*input) add_history(input);
		for (int i = 0; functions[i].name != NULL; i++)
			if (strcmp(input, functions[i].name) == 0) {
				functions[i].func();
				//printf("%s\n", functions[i].name);
				break;
			}
		free(input);
	}
	return 0;
}
