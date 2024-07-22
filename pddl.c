#include <stdbool.h>
#include <string.h>
#include "ntree.h"

enum obj {
	dummypoint,
	pos0_0,
	pos0_1,
	pos0_2,
	pos0_3,
	pos0_4,
	pos1_0,
	pos1_1,
	pos1_2,
	pos1_3,
	pos1_4,
	pos2_0,
	pos2_1,
	pos2_2,
	pos2_3,
	pos2_4,
	pos3_0,
	pos3_1,
	pos3_2,
	pos3_3,
	pos3_4,
	pos4_0,
	pos4_1,
	pos4_2,
	pos4_3,
	pos4_4,
};
bool ISADJACENT[26][26];
bool tailsnake[26];
bool headsnake[26];
bool nextsnake[26][26];
bool blocked[26];
bool spawn[26];
bool NEXTSPAWN[26][26];
bool ispoint[26];
bool checktrue_ISADJACENT(int x, int y) {
	return ISADJACENT[x][y];
}
bool checktrue_tailsnake(int x) {
	return tailsnake[x];
}
bool checktrue_headsnake(int x) {
	return headsnake[x];
}
bool checktrue_nextsnake(int x, int y) {
	return nextsnake[x][y];
}
bool checktrue_blocked(int x) {
	return blocked[x];
}
bool checktrue_spawn(int x) {
	return spawn[x];
}
bool checktrue_NEXTSPAWN(int x, int y) {
	return NEXTSPAWN[x][y];
}
bool checktrue_ispoint(int x) {
	return ispoint[x];
}
struct move {
	unsigned long head;
	unsigned long newhead;
	unsigned long tail;
	unsigned long newtail;
	Ntree *precon;
	Ntree *effect;
};
struct move_and_eat_spawn {
	unsigned long head;
	unsigned long newhead;
	unsigned long spawnpoint;
	unsigned long nextspawnpoint;
	Ntree *precon;
	Ntree *effect;
};
struct move_and_eat_no_spawn {
	unsigned long head;
	unsigned long newhead;
	Ntree *precon;
	Ntree *effect;
};
int main(void) {
	ISADJACENT[pos0_0][pos1_0] = true;
	ISADJACENT[pos0_0][pos0_1] = true;
	ISADJACENT[pos0_1][pos1_1] = true;
	ISADJACENT[pos0_1][pos0_2] = true;
	ISADJACENT[pos0_1][pos0_0] = true;
	ISADJACENT[pos0_2][pos1_2] = true;
	ISADJACENT[pos0_2][pos0_3] = true;
	ISADJACENT[pos0_2][pos0_1] = true;
	ISADJACENT[pos0_3][pos1_3] = true;
	ISADJACENT[pos0_3][pos0_4] = true;
	ISADJACENT[pos0_3][pos0_2] = true;
	ISADJACENT[pos0_4][pos1_4] = true;
	ISADJACENT[pos0_4][pos0_3] = true;
	ISADJACENT[pos1_0][pos2_0] = true;
	ISADJACENT[pos1_0][pos1_1] = true;
	ISADJACENT[pos1_0][pos0_0] = true;
	ISADJACENT[pos1_1][pos2_1] = true;
	ISADJACENT[pos1_1][pos1_2] = true;
	ISADJACENT[pos1_1][pos0_1] = true;
	ISADJACENT[pos1_1][pos1_0] = true;
	ISADJACENT[pos1_2][pos2_2] = true;
	ISADJACENT[pos1_2][pos1_3] = true;
	ISADJACENT[pos1_2][pos0_2] = true;
	ISADJACENT[pos1_2][pos1_1] = true;
	ISADJACENT[pos1_3][pos2_3] = true;
	ISADJACENT[pos1_3][pos1_4] = true;
	ISADJACENT[pos1_3][pos0_3] = true;
	ISADJACENT[pos1_3][pos1_2] = true;
	ISADJACENT[pos1_4][pos2_4] = true;
	ISADJACENT[pos1_4][pos0_4] = true;
	ISADJACENT[pos1_4][pos1_3] = true;
	ISADJACENT[pos2_0][pos3_0] = true;
	ISADJACENT[pos2_0][pos2_1] = true;
	ISADJACENT[pos2_0][pos1_0] = true;
	ISADJACENT[pos2_1][pos3_1] = true;
	ISADJACENT[pos2_1][pos2_2] = true;
	ISADJACENT[pos2_1][pos1_1] = true;
	ISADJACENT[pos2_1][pos2_0] = true;
	ISADJACENT[pos2_2][pos3_2] = true;
	ISADJACENT[pos2_2][pos2_3] = true;
	ISADJACENT[pos2_2][pos1_2] = true;
	ISADJACENT[pos2_2][pos2_1] = true;
	ISADJACENT[pos2_3][pos3_3] = true;
	ISADJACENT[pos2_3][pos2_4] = true;
	ISADJACENT[pos2_3][pos1_3] = true;
	ISADJACENT[pos2_3][pos2_2] = true;
	ISADJACENT[pos2_4][pos3_4] = true;
	ISADJACENT[pos2_4][pos1_4] = true;
	ISADJACENT[pos2_4][pos2_3] = true;
	ISADJACENT[pos3_0][pos4_0] = true;
	ISADJACENT[pos3_0][pos3_1] = true;
	ISADJACENT[pos3_0][pos2_0] = true;
	ISADJACENT[pos3_1][pos4_1] = true;
	ISADJACENT[pos3_1][pos3_2] = true;
	ISADJACENT[pos3_1][pos2_1] = true;
	ISADJACENT[pos3_1][pos3_0] = true;
	ISADJACENT[pos3_2][pos4_2] = true;
	ISADJACENT[pos3_2][pos3_3] = true;
	ISADJACENT[pos3_2][pos2_2] = true;
	ISADJACENT[pos3_2][pos3_1] = true;
	ISADJACENT[pos3_3][pos4_3] = true;
	ISADJACENT[pos3_3][pos3_4] = true;
	ISADJACENT[pos3_3][pos2_3] = true;
	ISADJACENT[pos3_3][pos3_2] = true;
	ISADJACENT[pos3_4][pos4_4] = true;
	ISADJACENT[pos3_4][pos2_4] = true;
	ISADJACENT[pos3_4][pos3_3] = true;
	ISADJACENT[pos4_0][pos4_1] = true;
	ISADJACENT[pos4_0][pos3_0] = true;
	ISADJACENT[pos4_1][pos4_2] = true;
	ISADJACENT[pos4_1][pos3_1] = true;
	ISADJACENT[pos4_1][pos4_0] = true;
	ISADJACENT[pos4_2][pos4_3] = true;
	ISADJACENT[pos4_2][pos3_2] = true;
	ISADJACENT[pos4_2][pos4_1] = true;
	ISADJACENT[pos4_3][pos4_4] = true;
	ISADJACENT[pos4_3][pos3_3] = true;
	ISADJACENT[pos4_3][pos4_2] = true;
	ISADJACENT[pos4_4][pos3_4] = true;
	ISADJACENT[pos4_4][pos4_3] = true;
	tailsnake[pos3_0] = true;
	headsnake[pos4_0] = true;
	nextsnake[pos4_0][pos3_0] = true;
	blocked[pos3_0] = true;
	blocked[pos4_0] = true;
	spawn[pos2_0] = true;
	NEXTSPAWN[pos1_0][dummypoint] = true;
	NEXTSPAWN[pos2_0][pos1_4] = true;
	NEXTSPAWN[pos1_4][pos1_1] = true;
	NEXTSPAWN[pos1_1][pos0_1] = true;
	NEXTSPAWN[pos0_1][pos3_3] = true;
	NEXTSPAWN[pos3_3][pos4_2] = true;
	NEXTSPAWN[pos4_2][pos3_4] = true;
	NEXTSPAWN[pos3_4][pos0_0] = true;
	NEXTSPAWN[pos0_0][pos1_2] = true;
	NEXTSPAWN[pos1_2][pos1_0] = true;
	ispoint[pos0_4] = true;
	ispoint[pos3_1] = true;
	ispoint[pos1_3] = true;
	ispoint[pos2_4] = true;
	ispoint[pos4_1] = true;
	Ntree *precon_0 = create_tree(15);
	Data d_0_0;
	d_0_0.index = 0;
	strcpy(d_0_0.id, "AND");
	d_0_0.t = AND;
	insert(*precon_0->adj, d_0_0);
	Data d_0_1;
	d_0_1.index = 1;
	strcpy(d_0_1.id, "HEADSNAKE");
	d_0_1.t = PARAMETER;
	insert(*precon_0->adj, d_0_1);
	add_edge(precon_0, d_0_0, d_0_1);
	d_0_1.args = head;
	Data d_0_2;
	d_0_2.index = 2;
	strcpy(d_0_2.id, "ISADJACENT");
	d_0_2.t = PARAMETER;
	insert(*precon_0->adj, d_0_2);
	add_edge(precon_0, d_0_0, d_0_2);
	d_0_2.args = head;
	d_0_2.args = newhead;
	Data d_0_3;
	d_0_3.index = 3;
	strcpy(d_0_3.id, "TAILSNAKE");
	d_0_3.t = PARAMETER;
	insert(*precon_0->adj, d_0_3);
	add_edge(precon_0, d_0_0, d_0_3);
	d_0_3.args = tail;
	Data d_0_4;
	d_0_4.index = 4;
	strcpy(d_0_4.id, "NEXTSNAKE");
	d_0_4.t = PARAMETER;
	insert(*precon_0->adj, d_0_4);
	add_edge(precon_0, d_0_0, d_0_4);
	d_0_4.args = newtail;
	d_0_4.args = tail;
	Data d_0_5;
	d_0_5.index = 5;
	strcpy(d_0_5.id, "NOT");
	d_0_5.t = NOT;
	insert(*precon_0->adj, d_0_5);
	add_edge(precon_0, d_0_0, d_0_5);
	Data d_0_6;
	d_0_6.index = 6;
	strcpy(d_0_6.id, "BLOCKED");
	d_0_6.t = PARAMETER;
	insert(*precon_0->adj, d_0_6);
	add_edge(precon_0, d_0_5, d_0_6);
	d_0_6.args = newhead;
	Data d_0_7;
	d_0_7.index = 7;
	strcpy(d_0_7.id, "NOT");
	d_0_7.t = NOT;
	insert(*precon_0->adj, d_0_7);
	add_edge(precon_0, d_0_0, d_0_7);
	Data d_0_8;
	d_0_8.index = 8;
	strcpy(d_0_8.id, "ISPOINT");
	d_0_8.t = PARAMETER;
	insert(*precon_0->adj, d_0_8);
	add_edge(precon_0, d_0_7, d_0_8);
	d_0_8.args = newhead;
	Ntree *precon_1 = create_tree(15);
	Data d_1_0;
	d_1_0.index = 0;
	strcpy(d_1_0.id, "AND");
	d_1_0.t = AND;
	insert(*precon_1->adj, d_1_0);
	Data d_1_1;
	d_1_1.index = 1;
	strcpy(d_1_1.id, "HEADSNAKE");
	d_1_1.t = PARAMETER;
	insert(*precon_1->adj, d_1_1);
	add_edge(precon_1, d_1_0, d_1_1);
	d_1_1.args = head;
	Data d_1_2;
	d_1_2.index = 2;
	strcpy(d_1_2.id, "ISADJACENT");
	d_1_2.t = PARAMETER;
	insert(*precon_1->adj, d_1_2);
	add_edge(precon_1, d_1_0, d_1_2);
	d_1_2.args = head;
	d_1_2.args = newhead;
	Data d_1_3;
	d_1_3.index = 3;
	strcpy(d_1_3.id, "NOT");
	d_1_3.t = NOT;
	insert(*precon_1->adj, d_1_3);
	add_edge(precon_1, d_1_0, d_1_3);
	Data d_1_4;
	d_1_4.index = 4;
	strcpy(d_1_4.id, "BLOCKED");
	d_1_4.t = PARAMETER;
	insert(*precon_1->adj, d_1_4);
	add_edge(precon_1, d_1_3, d_1_4);
	d_1_4.args = newhead;
	Data d_1_5;
	d_1_5.index = 5;
	strcpy(d_1_5.id, "ISPOINT");
	d_1_5.t = PARAMETER;
	insert(*precon_1->adj, d_1_5);
	add_edge(precon_1, d_1_0, d_1_5);
	d_1_5.args = newhead;
	Data d_1_6;
	d_1_6.index = 6;
	strcpy(d_1_6.id, "SPAWN");
	d_1_6.t = PARAMETER;
	insert(*precon_1->adj, d_1_6);
	add_edge(precon_1, d_1_0, d_1_6);
	d_1_6.args = spawnpoint;
	Data d_1_7;
	d_1_7.index = 7;
	strcpy(d_1_7.id, "NEXTSPAWN");
	d_1_7.t = PARAMETER;
	insert(*precon_1->adj, d_1_7);
	add_edge(precon_1, d_1_0, d_1_7);
	d_1_7.args = spawnpoint;
	d_1_7.args = nextspawnpoint;
	Data d_1_8;
	d_1_8.index = 8;
	strcpy(d_1_8.id, "NOT");
	d_1_8.t = NOT;
	insert(*precon_1->adj, d_1_8);
	add_edge(precon_1, d_1_0, d_1_8);
	Data d_1_9;
	d_1_9.index = 9;
	strcpy(d_1_9.id, "=");
	d_1_9.t = PARAMETER;
	insert(*precon_1->adj, d_1_9);
	add_edge(precon_1, d_1_8, d_1_9);
	d_1_9.args = spawnpoint;
	Data d_1_10;
	d_1_10.index = 10;
	strcpy(d_1_10.id, "DUMMYPOINT");
	d_1_10.t = PARAMETER;
	insert(*precon_1->adj, d_1_10);
	add_edge(precon_1, d_1_9, d_1_10);
	Ntree *precon_2 = create_tree(15);
	Data d_2_0;
	d_2_0.index = 0;
	strcpy(d_2_0.id, "AND");
	d_2_0.t = AND;
	insert(*precon_2->adj, d_2_0);
	Data d_2_1;
	d_2_1.index = 1;
	strcpy(d_2_1.id, "HEADSNAKE");
	d_2_1.t = PARAMETER;
	insert(*precon_2->adj, d_2_1);
	add_edge(precon_2, d_2_0, d_2_1);
	d_2_1.args = head;
	Data d_2_2;
	d_2_2.index = 2;
	strcpy(d_2_2.id, "ISADJACENT");
	d_2_2.t = PARAMETER;
	insert(*precon_2->adj, d_2_2);
	add_edge(precon_2, d_2_0, d_2_2);
	d_2_2.args = head;
	d_2_2.args = newhead;
	Data d_2_3;
	d_2_3.index = 3;
	strcpy(d_2_3.id, "NOT");
	d_2_3.t = NOT;
	insert(*precon_2->adj, d_2_3);
	add_edge(precon_2, d_2_0, d_2_3);
	Data d_2_4;
	d_2_4.index = 4;
	strcpy(d_2_4.id, "BLOCKED");
	d_2_4.t = PARAMETER;
	insert(*precon_2->adj, d_2_4);
	add_edge(precon_2, d_2_3, d_2_4);
	d_2_4.args = newhead;
	Data d_2_5;
	d_2_5.index = 5;
	strcpy(d_2_5.id, "ISPOINT");
	d_2_5.t = PARAMETER;
	insert(*precon_2->adj, d_2_5);
	add_edge(precon_2, d_2_0, d_2_5);
	d_2_5.args = newhead;
	Data d_2_6;
	d_2_6.index = 6;
	strcpy(d_2_6.id, "SPAWN");
	d_2_6.t = PARAMETER;
	insert(*precon_2->adj, d_2_6);
	add_edge(precon_2, d_2_0, d_2_6);
	Data d_2_7;
	d_2_7.index = 7;
	strcpy(d_2_7.id, "DUMMYPOINT");
	d_2_7.t = PARAMETER;
	insert(*precon_2->adj, d_2_7);
	add_edge(precon_2, d_2_6, d_2_7);
	return 0;
}
