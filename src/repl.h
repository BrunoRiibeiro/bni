#ifndef REPL_H
#define REPL_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pddl.h"

char **action_name_completion(const char *text, int start, int end);
char *action_name_generator(const char *text, int state);
void show_actions(const char *filename);
void free_names(void);
int ask_yes_no(const char *question);
void to_uppercase(char *str);
void printheader();

#endif /* REPL_H */
