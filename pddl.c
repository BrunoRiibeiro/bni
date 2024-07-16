#include <stdbool.h>
#include <string.h>
#include "ntree.h"

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
};
bool bulb[9];
bool broken[9];
bool adjacent_vertex[9][9][9];
bool adjacent_edge[9][9][9][9];
bool adjacent[9][9][9][9][9];
struct CLICK_VERTEX {
	unsigned long c;
	unsigned long c1;
	unsigned long c2;
	Ntree *precon;
	Ntree *effect;
};
struct CLICK_EDGE {
	unsigned long c;
	unsigned long c1;
	unsigned long c2;
	unsigned long c3;
	Ntree *precon;
	Ntree *effect;
};
struct CLICK {
	unsigned long c;
	unsigned long c1;
	unsigned long c2;
	unsigned long c3;
	unsigned long c4;
	Ntree *precon;
	Ntree *effect;
};
int main(void) {
	adjacent_vertex[c0_0][c0_1][c1_0] = true;
	adjacent_edge[c0_1][c0_0][c0_2][c1_1] = true;
	bulb[c0_1] = true;
	adjacent_vertex[c0_2][c0_1][c1_2] = true;
	adjacent_edge[c1_0][c0_0][c1_1][c2_0] = true;
	bulb[c1_0] = true;
	adjacent[c1_1][c0_1][c1_0][c1_2][c2_1] = true;
	broken[c1_1] = true;
	adjacent_edge[c1_2][c0_2][c1_1][c2_2] = true;
	bulb[c1_2] = true;
	adjacent_vertex[c2_0][c1_0][c2_1] = true;
	adjacent_edge[c2_1][c1_1][c2_0][c2_2] = true;
	bulb[c2_1] = true;
	adjacent_vertex[c2_2][c1_2][c2_1] = true;
	Ntree *precon_0 = create_tree(15);
	Data d_0_0;
	d_0_0.index = 0;
	strcpy(d_0_0.id, "AND");
	d_0_0.t = AND;
	insert(*precon_0->adj, d_0_0);
	Data d_0_1;
	d_0_1.index = 1;
	strcpy(d_0_1.id, "ADJACENT-VERTEX");
	d_0_1.t = PARAMETER;
	insert(*precon_0->adj, d_0_1);
	add_edge(precon_0, d_0_0, d_0_1);
	d_0_1.args = ?c;
	d_0_1.args = ?c1;
	d_0_1.args = ?c2;
	Ntree *precon_1 = create_tree(15);
	Data d_1_0;
	d_1_0.index = 0;
	strcpy(d_1_0.id, "AND");
	d_1_0.t = AND;
	insert(*precon_1->adj, d_1_0);
	Data d_1_1;
	d_1_1.index = 1;
	strcpy(d_1_1.id, "ADJACENT-EDGE");
	d_1_1.t = PARAMETER;
	insert(*precon_1->adj, d_1_1);
	add_edge(precon_1, d_1_0, d_1_1);
	d_1_1.args = ?c;
	d_1_1.args = ?c1;
	d_1_1.args = ?c2;
	d_1_1.args = ?c3;
	Ntree *precon_2 = create_tree(15);
	Data d_2_0;
	d_2_0.index = 0;
	strcpy(d_2_0.id, "AND");
	d_2_0.t = AND;
	insert(*precon_2->adj, d_2_0);
	Data d_2_1;
	d_2_1.index = 1;
	strcpy(d_2_1.id, "ADJACENT");
	d_2_1.t = PARAMETER;
	insert(*precon_2->adj, d_2_1);
	add_edge(precon_2, d_2_0, d_2_1);
	d_2_1.args = ?c;
	d_2_1.args = ?c1;
	d_2_1.args = ?c2;
	d_2_1.args = ?c3;
	d_2_1.args = ?c4;
	return 0;
}
