#ifndef PDDL_H
#define PDDL_H
#include <stdbool.h>
#include <string.h>
#define and &&
#define or ||

enum lugar {
	s7,
	corredors,
	escada,
	corredori,
	patio,
	io_uac,
	calcada_uac,
	calcada_ued,
	saidap1,
	dindingourmet,
	saidacarro1,
	saidap2,
	pistacooper,
	avenida,
	brt,
};
bool ligado[15][15];
bool estou_em[15];
bool passei_em[15];
bool comprou_dindin0;
bool comprou_dindin1;
bool comprou_dindin2;
bool comprou_dindin3;
bool lugar_eh[15][15];
bool checktrue_ligado(int l1, int l2);
bool checktrue_estou_em(int l);
bool checktrue_passei_em(int l);
bool checktrue_comprou_dindin0();
bool checktrue_comprou_dindin1();
bool checktrue_comprou_dindin2();
bool checktrue_comprou_dindin3();
bool checktrue_lugar_eh(int l1, int l2);
struct mover {
	int l1;
	int l2;
};
bool checktrue_mover(struct mover s);
void apply_mover(struct mover s);
struct comprardindin1 {
	int l1;
};
bool checktrue_comprardindin1(struct comprardindin1 s);
void apply_comprardindin1(struct comprardindin1 s);
struct comprardindin2 {
	int l1;
};
bool checktrue_comprardindin2(struct comprardindin2 s);
void apply_comprardindin2(struct comprardindin2 s);
struct comprardindin3 {
	int l1;
};
bool checktrue_comprardindin3(struct comprardindin3 s);
void apply_comprardindin3(struct comprardindin3 s);
#endif /* PDDL_H */
