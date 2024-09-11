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

char *preprocess_file(const char *f);
void constants_n_objects(FILE *file, SymbolTable *st, Stack *stack, LinkedList *hl, char token);
void predicates(FILE *domain_file, FILE *domainc, FILE *domainh, SymbolTable *st, Stack *parenthesis_stack, char tokend);
void action(FILE *domain_file, FILE *domainc, FILE *domainh, FILE *tmpshow, FILE *tmpapply, Stack *domain, Stack *parenthesis_stack, char tokend, int act_count);
void parameters(FILE *domain_file, FILE *domainh, FILE *tmpapply, FILE *tmpfile_check_show, Stack *domain, char *act_name);
void precondition(FILE *domain_file, FILE *domainc, Stack *domain, Stack *parenthesis_stack, char *act_name);
void effect(FILE *domain_file, FILE *domainc, Stack *domain, Stack *parenthesis_stack, char *act_name);
void init(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp);
void goal(FILE *problem_file, FILE *domainc, FILE *domainh, Stack *parenthesis_stack, char tokenp);
void create_enums(FILE *fc, FILE *fh, SymbolTable *st);
int is_operator(LinkedList *word);
void write_operator(FILE *towrite, Stack *operators, int operator, int *forall_id, char flag);
int create_fors(FILE *toread, FILE *towrite, LinkedList *types_list, int par_count);
void create_forall_pre_goal(FILE *toread, FILE *towrite, LinkedList *types_list, int forall_id, int par_count);
void create_forall_effect(FILE *toread, FILE *towrite, LinkedList *types_list, int par_count);
void cat(const char *fname, FILE *f2);

#endif /* PARSER_H */
