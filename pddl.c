#include <stdio.h>
#include "pddl.h"

cellMap cell_map[LENGTH_cell] = {
	{"c0_0", c0_0},
	{"c0_1", c0_1},
	{"c0_2", c0_2},
	{"c1_0", c1_0},
	{"c1_1", c1_1},
	{"c1_2", c1_2},
	{"c2_0", c2_0},
	{"c2_1", c2_1},
	{"c2_2", c2_2},
};
const char *get_cell_names(enum cell e) {
	if (e >= 0 && e < LENGTH_cell)
		return cell_map[e].str;
	return NULL;
}
enum cell get_cell_enum(const char *s) {
	for (int i = 0; cell_map[i].str != NULL; i++)
		if (strcmp(s, cell_map[i].str) == 0)
			return cell_map[i].value;
	return LENGTH_cell;
}
bool on[9];
bool adj[9][9];
bool always_false[9];
bool notype[0];
bool checktrue_on(int c) {
	return on[c];
}
bool checktrue_adj(int c0, int c1) {
	return adj[c0][c1];
}
bool checktrue_always_false(int c) {
	return always_false[c];
}
bool checktrue_notype(int a) {
	return notype[a];
}
bool checktrue_click(struct click s) {
	return (s.cchecktrue__(, cell) (null) !(checktrue_always_false(s.c)));
}
void apply_click(struct click s) {
	for (int i0 = 0; i0 < LENGTH_cell; i0++) {
		for (int i1 = 0; i1 < LENGTH_cell; i1++) {
			for (int i2 = 0; i2 < LENGTH_obj; i2++) {
				on[i0] = 1;
				adj[i1][i2] = 1;
			}
		}
	}
}
void check_show_click(void) {
	struct click s;
	for (int i0 = 0; i0 < LENGTH_cell; i0++) {
		s.c = i0;
		if (checktrue_click(s)) printf("  - click(%s)\n", get_cell_names(i0));
	}
}
void show_actions(void) {
	check_show_click();
}
int apply_actions(char *s) {
	const char *basename = strsep(&s, "(");
	if (strcmp(basename, "click") == 0) {
		struct click tcurts;
		char *token;
		token = strsep(&s, ",");
		tcurts.c = get_cell_enum(strsep(&token, ")"));
		if (checktrue_click(tcurts)) {
			apply_click(tcurts);
			return 0;
		} else return 1;
	}
	return 2;
}
void initialize(void) {
	on[c0_1] = true;
	on[c1_0] = true;
	on[c1_1] = true;
	on[c1_2] = true;
	on[c2_1] = true;
	adj[c0_0][c0_1] = true;
	adj[c0_0][c1_0] = true;
	adj[c0_1][c0_2] = true;
	adj[c0_1][c1_1] = true;
	adj[c0_2][c1_2] = true;
	adj[c1_0][c1_1] = true;
	adj[c1_0][c2_0] = true;
	adj[c1_1][c1_2] = true;
	adj[c1_1][c2_1] = true;
	adj[c1_2][c2_2] = true;
	adj[c2_0][c2_1] = true;
	adj[c2_1][c2_2] = true;
	return;
}
bool checktrue_goal(void) {
	return (checktrue_?c(_, cell) forall !(checktrue_on(?c));
}
