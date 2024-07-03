#include <stdbool.h>
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
	return 0;
}
