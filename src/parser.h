#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "symbol_table.h"
#include "linked_list.h"
#include "stack.h"

extern const char *OPERATORS[];
#define OPERATORS_SIZE (sizeof(OPERATORS) / sizeof(OPERATORS[0]))
extern char obj_sentinel;

void write_operator(FILE *towrite, Stack *operators, int operator, int *forall_id, char flag);
int is_operator(LinkedList *word);
void create_forall_pre_goal(FILE *toread, FILE *towrite, int forall_id, int op_args, ...);
int create_fors(FILE *toread, FILE *towrite, LinkedList *types_list, int par_count);
void create_forall_effect(FILE *toread, FILE *towrite, int op_args, ...);
void create_enums(FILE *fc, FILE *fh, SymbolTable *st);
char *preprocess_file(const char *f);
void cat(const char *fname, FILE *f2);
void constants_n_objects(FILE *file, SymbolTable *st, Stack *stack, LinkedList *hl, char token);
void predicates(FILE *domain_file, FILE *domainc, FILE *domainh, SymbolTable *st, Stack *parenthesis_stack, char tokend);
void action(FILE *domain_file, FILE *domainc, FILE *domainh, FILE *tmpshow, FILE *tmpapply, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count);
void init(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp);
void goal(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp);

#endif /* PARSER_H */
