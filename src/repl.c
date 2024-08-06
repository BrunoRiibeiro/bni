#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pddl.h"

void printheader() {
	const char *header =
	"\033[90m################################################################################################\n"
	"\033[90m# " "\033[31m _             _     \033[32m _____   \033[33m _____   \033[34m _____   \033[35m _          \033[36m _____  \033[37m ______ \033[91m _____   _"       "       \033[90m#\n" 
	"\033[90m# " "\033[31m| |           (_)   \033[32m |  __ \\ \033[33m |  __ \\ \033[34m |  __ \\ \033[35m | |        \033[36m |  __ \\ \033[37m|  ____|\033[91m|  __ \\ | |"   "      \033[90m#\n"
	"\033[90m# " "\033[31m| |__   _ __   _    \033[32m | |__) |\033[33m | |  | |\033[34m | |  | |\033[35m | |        \033[36m | |__) |\033[37m| |__   \033[91m| |__) || |"        "      \033[90m#\n"
	"\033[90m# " "\033[31m| '_ \\ | '_ \\ | |   \033[32m |  ___/  \033[33m| |  | |\033[34m | |  | |\033[35m | |        \033[36m |  _  / \033[37m|  __|  \033[91m|  ___/ | |"      "      \033[90m#\n"
	"\033[90m# " "\033[31m| |_) || | | || |   \033[32m | |      \033[33m| |__| | \033[34m| |__| |\033[35m | |____    \033[36m | | \\ \\ \033[37m| |____ \033[91m| |     | |____"  "  \033[90m#\n"
	"\033[90m# " "\033[31m|_.__/ |_| |_||_|   \033[32m |_|      \033[33m|_____/  \033[34m|_____/  \033[35m|______|   \033[36m |_|  \\_\\\033[37m|______|\033[91m|_|     |______|" " \033[90m#\n"
	"\033[90m################################################################################################\n"
	"\033[0m";
	printf("\e[1;1H\e[2J");
    printf("%s\n", header);
}

void to_uppercase(char *str) {
	for (int i = 0; str[i] != '\0'; i++)
		str[i] = toupper((unsigned char) str[i]);
}

int ask_yes_no(const char *question) {
	char *response;
	for ( ; ; ) {
		response = readline(question);
		to_uppercase(response);
		if (response) {
			if (strcmp(response, "Y") == 0 || strcmp(response, "YES") == 0) {
				free(response);
				return 1;
			} else if (strcmp(response, "N") == 0 || strcmp(response, "NO") == 0) {
				free(response);
				return 0;
			}
			free(response);
		}
		printf("Please answer Y or N.\n");
	}
}

int main(void) {
	printheader();
	initialize();
	char *input;
	for ( ; ; ) {
		if (checktrue_goal())
			if(!ask_yes_no("Goal has been hit! Do u want to continue? (Y/n)\n")) break;
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
