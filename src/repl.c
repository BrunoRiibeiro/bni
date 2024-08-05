#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pddl.h"

typedef struct Map {
    char *name;
    void (*func)();
}Map;

void printheader() {
    const char *header =
		"\033[35m"
        "#################\n"
        "#\033[33m bni PDDL REPL\033[35m #\n"
        "#################\n\n" "\033[0m";
	printf("\e[1;1H\e[2J");
    printf("%s", header);
}

int main(void) {
	Map functions[] = {
		{"mover", apply_mover},
		{"comprardindin1", apply_comprardindin1},
		{"comprardindin2", apply_comprardindin2},
		{"comprardindin3", apply_comprardindin3},
		{NULL, NULL}
	};
	printheader();
	initialize();
	char *input;
	while (1) {
		printf("Please enter one of the currently available actions:\n");
		show_actions();
		input = readline("\n>> ");
		if (!input || strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) break;
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
