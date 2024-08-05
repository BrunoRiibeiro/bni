#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pddl.h"

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
	printheader();
	initialize();
	char *input;
	while (1) {
		printf("Please enter one of the currently available actions:\n");
		show_actions();
		input = readline(">> ");
		if (!input || strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) break;
		if (*input) add_history(input);
		apply_actions(input);
		free(input);
	}
	return 0;
}
