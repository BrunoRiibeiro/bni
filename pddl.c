#include <stdbool.h>
#include <string.h>
#define and &&
#define or ||

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
bool isadjacent[26][26];
bool tailsnake[26];
bool headsnake[26];
bool nextsnake[26][26];
bool blocked[26];
bool spawn[26];
bool nextspawn[26][26];
bool ispoint[26];
bool checktrue_isadjacent(int x, int y) {
	return isadjacent[x][y];
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
bool checktrue_nextspawn(int x, int y) {
	return nextspawn[x][y];
}
bool checktrue_ispoint(int x) {
	return ispoint[x];
}
struct move {
	int head;
	int newhead;
	int tail;
	int newtail;
};
bool checktrue_move(struct move s) {
	return (checktrue_headsnake(s.head) and checktrue_isadjacent(s.head, s.newhead) and checktrue_tailsnake(s.tail) and checktrue_nextsnake(s.newtail, s.tail) and !(checktrue_blocked(s.newhead)) and (checktrue_blocked(s.newhead) or !(checktrue_ispoint(s.newhead))));
}
void apply_move(struct move s) {
	blocked[s.newhead] = 1;
	headsnake[s.newhead] = 1;
	nextsnake[s.newhead][s.head] = 1;
	headsnake[s.head] = 0;
	blocked[s.tail] = 0;
	tailsnake[s.tail] = 0;
	nextsnake[s.newtail][s.tail] = 0;
	tailsnake[s.newtail] = 1;
}
struct move_and_eat_spawn {
	int head;
	int newhead;
	int spawnpoint;
	int nextspawnpoint;
};
bool checktrue_move_and_eat_spawn(struct move_and_eat_spawn s) {
	return (checktrue_headsnake(s.head) and checktrue_isadjacent(s.head, s.newhead) and !(checktrue_blocked(s.newhead)) and checktrue_ispoint(s.newhead) and checktrue_spawn(s.spawnpoint) and checktrue_nextspawn(s.spawnpoint, s.nextspawnpoint) and !(checktrue_=(s.spawnpoint, dummypoint)));
}
void apply_move_and_eat_spawn(struct move_and_eat_spawn s) {
	blocked[s.newhead] = 1;
	headsnake[s.newhead] = 1;
	nextsnake[s.newhead][s.head] = 1;
	headsnake[s.head] = 0;
	ispoint[s.newhead] = 0;
	ispoint[s.spawnpoint] = 1;
	spawn[s.spawnpoint] = 0;
	spawn[s.nextspawnpoint] = 1;
}
struct move_and_eat_no_spawn {
	int head;
	int newhead;
};
bool checktrue_move_and_eat_no_spawn(struct move_and_eat_no_spawn s) {
	return (checktrue_headsnake(s.head) and checktrue_isadjacent(s.head, s.newhead) and !(checktrue_blocked(s.newhead)) and checktrue_ispoint(s.newhead) and checktrue_spawn(dummypoint));
}
void apply_move_and_eat_no_spawn(struct move_and_eat_no_spawn s) {
	blocked[s.newhead] = 1;
	headsnake[s.newhead] = 1;
	nextsnake[s.newhead][s.head] = 1;
	headsnake[s.head] = 0;
	ispoint[s.newhead] = 0;
}
int main(void) {
	isadjacent[pos0_0][pos1_0] = true;
	isadjacent[pos0_0][pos0_1] = true;
	isadjacent[pos0_1][pos1_1] = true;
	isadjacent[pos0_1][pos0_2] = true;
	isadjacent[pos0_1][pos0_0] = true;
	isadjacent[pos0_2][pos1_2] = true;
	isadjacent[pos0_2][pos0_3] = true;
	isadjacent[pos0_2][pos0_1] = true;
	isadjacent[pos0_3][pos1_3] = true;
	isadjacent[pos0_3][pos0_4] = true;
	isadjacent[pos0_3][pos0_2] = true;
	isadjacent[pos0_4][pos1_4] = true;
	isadjacent[pos0_4][pos0_3] = true;
	isadjacent[pos1_0][pos2_0] = true;
	isadjacent[pos1_0][pos1_1] = true;
	isadjacent[pos1_0][pos0_0] = true;
	isadjacent[pos1_1][pos2_1] = true;
	isadjacent[pos1_1][pos1_2] = true;
	isadjacent[pos1_1][pos0_1] = true;
	isadjacent[pos1_1][pos1_0] = true;
	isadjacent[pos1_2][pos2_2] = true;
	isadjacent[pos1_2][pos1_3] = true;
	isadjacent[pos1_2][pos0_2] = true;
	isadjacent[pos1_2][pos1_1] = true;
	isadjacent[pos1_3][pos2_3] = true;
	isadjacent[pos1_3][pos1_4] = true;
	isadjacent[pos1_3][pos0_3] = true;
	isadjacent[pos1_3][pos1_2] = true;
	isadjacent[pos1_4][pos2_4] = true;
	isadjacent[pos1_4][pos0_4] = true;
	isadjacent[pos1_4][pos1_3] = true;
	isadjacent[pos2_0][pos3_0] = true;
	isadjacent[pos2_0][pos2_1] = true;
	isadjacent[pos2_0][pos1_0] = true;
	isadjacent[pos2_1][pos3_1] = true;
	isadjacent[pos2_1][pos2_2] = true;
	isadjacent[pos2_1][pos1_1] = true;
	isadjacent[pos2_1][pos2_0] = true;
	isadjacent[pos2_2][pos3_2] = true;
	isadjacent[pos2_2][pos2_3] = true;
	isadjacent[pos2_2][pos1_2] = true;
	isadjacent[pos2_2][pos2_1] = true;
	isadjacent[pos2_3][pos3_3] = true;
	isadjacent[pos2_3][pos2_4] = true;
	isadjacent[pos2_3][pos1_3] = true;
	isadjacent[pos2_3][pos2_2] = true;
	isadjacent[pos2_4][pos3_4] = true;
	isadjacent[pos2_4][pos1_4] = true;
	isadjacent[pos2_4][pos2_3] = true;
	isadjacent[pos3_0][pos4_0] = true;
	isadjacent[pos3_0][pos3_1] = true;
	isadjacent[pos3_0][pos2_0] = true;
	isadjacent[pos3_1][pos4_1] = true;
	isadjacent[pos3_1][pos3_2] = true;
	isadjacent[pos3_1][pos2_1] = true;
	isadjacent[pos3_1][pos3_0] = true;
	isadjacent[pos3_2][pos4_2] = true;
	isadjacent[pos3_2][pos3_3] = true;
	isadjacent[pos3_2][pos2_2] = true;
	isadjacent[pos3_2][pos3_1] = true;
	isadjacent[pos3_3][pos4_3] = true;
	isadjacent[pos3_3][pos3_4] = true;
	isadjacent[pos3_3][pos2_3] = true;
	isadjacent[pos3_3][pos3_2] = true;
	isadjacent[pos3_4][pos4_4] = true;
	isadjacent[pos3_4][pos2_4] = true;
	isadjacent[pos3_4][pos3_3] = true;
	isadjacent[pos4_0][pos4_1] = true;
	isadjacent[pos4_0][pos3_0] = true;
	isadjacent[pos4_1][pos4_2] = true;
	isadjacent[pos4_1][pos3_1] = true;
	isadjacent[pos4_1][pos4_0] = true;
	isadjacent[pos4_2][pos4_3] = true;
	isadjacent[pos4_2][pos3_2] = true;
	isadjacent[pos4_2][pos4_1] = true;
	isadjacent[pos4_3][pos4_4] = true;
	isadjacent[pos4_3][pos3_3] = true;
	isadjacent[pos4_3][pos4_2] = true;
	isadjacent[pos4_4][pos3_4] = true;
	isadjacent[pos4_4][pos4_3] = true;
	tailsnake[pos3_0] = true;
	headsnake[pos4_0] = true;
	nextsnake[pos4_0][pos3_0] = true;
	blocked[pos3_0] = true;
	blocked[pos4_0] = true;
	spawn[pos2_0] = true;
	nextspawn[pos1_0][dummypoint] = true;
	nextspawn[pos2_0][pos1_4] = true;
	nextspawn[pos1_4][pos1_1] = true;
	nextspawn[pos1_1][pos0_1] = true;
	nextspawn[pos0_1][pos3_3] = true;
	nextspawn[pos3_3][pos4_2] = true;
	nextspawn[pos4_2][pos3_4] = true;
	nextspawn[pos3_4][pos0_0] = true;
	nextspawn[pos0_0][pos1_2] = true;
	nextspawn[pos1_2][pos1_0] = true;
	ispoint[pos0_4] = true;
	ispoint[pos3_1] = true;
	ispoint[pos1_3] = true;
	ispoint[pos2_4] = true;
	ispoint[pos4_1] = true;
	return 0;
}
