#ifndef PDDL_H
#define PDDL_H
#include <stdbool.h>
#include <string.h>
#define and &&
#define or ||

enum cell {
	c0_0,
	c0_1,
	c0_2,
	c1_0,
	c1_1,
	c1_2,
	c2_0,
	c2_1,
	c2_2,
	LENGTH_cell
};
typedef struct cellMap {
	const char *str;
	enum cell value;
}cellMap;
const char *get_cell_names(enum cell e);
bool checktrue_on(int c);
bool checktrue_adj(int c0, int c1);
bool checktrue_always_false(int c);
bool checktrue_notype(int a);
void check_show_click(void);
struct click {
	enum cell c;
};
bool checktrue_click(struct click s);
void apply_click(struct click s);
void show_actions(void);
int apply_actions(char *s);
void initialize(void);
bool checktrue_goal(void);
#endif /* PDDL_H */
