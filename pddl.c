#include "pddl.h"

bool ligado[15][15];
bool estou_em[15];
bool passei_em[15];
bool comprou_dindin0;
bool comprou_dindin1;
bool comprou_dindin2;
bool comprou_dindin3;
bool lugar_eh[15][15];
bool checktrue_ligado(int l1, int l2) {
	return ligado[l1][l2];
}
bool checktrue_estou_em(int l) {
	return estou_em[l];
}
bool checktrue_passei_em(int l) {
	return passei_em[l];
}
bool checktrue_comprou_dindin0() {
	return comprou_dindin0;
}
bool checktrue_comprou_dindin1() {
	return comprou_dindin1;
}
bool checktrue_comprou_dindin2() {
	return comprou_dindin2;
}
bool checktrue_comprou_dindin3() {
	return comprou_dindin3;
}
bool checktrue_lugar_eh(int l1, int l2) {
	return lugar_eh[l1][l2];
}
bool checktrue_mover(struct mover s) {
	return (checktrue_estou_em(s.l1) and (checktrue_ligado(s.l1, s.l2) or checktrue_ligado(s.l2, s.l1)));
}
void apply_mover(struct mover s) {
	estou_em[s.l1] = 0;
	passei_em[s.l2] = 1;
	estou_em[s.l2] = 1;
	lugar_eh[s.l1][s.l1] = 0;
	lugar_eh[s.l2][s.l2] = 1;
}
void check_show_mover(void) {
	struct mover s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l2 = i0;
		for (int i1 = 0; i1 < LENGTH_lugar; i1++) {
			s.l1 = i1;
			if (checktrue_mover(s)) show_mover();
		}
	}
}
bool checktrue_comprardindin1(struct comprardindin1 s) {
	return (checktrue_comprou_dindin0() and checktrue_estou_em(s.l1) and checktrue_lugar_eh(s.l1, dindingourmet));
}
void apply_comprardindin1(struct comprardindin1 s) {
	comprou_dindin1 = 1;
	comprou_dindin0 = 0;
}
void check_show_comprardindin1(void) {
	struct comprardindin1 s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l1 = i0;
		if (checktrue_comprardindin1(s)) show_comprardindin1();
	}
}
bool checktrue_comprardindin2(struct comprardindin2 s) {
	return (checktrue_comprou_dindin1() and checktrue_estou_em(s.l1) and checktrue_lugar_eh(s.l1, dindingourmet));
}
void apply_comprardindin2(struct comprardindin2 s) {
	comprou_dindin2 = 1;
	comprou_dindin1 = 0;
}
void check_show_comprardindin2(void) {
	struct comprardindin2 s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l1 = i0;
		if (checktrue_comprardindin2(s)) show_comprardindin2();
	}
}
bool checktrue_comprardindin3(struct comprardindin3 s) {
	return (checktrue_comprou_dindin2() and checktrue_estou_em(s.l1) and checktrue_lugar_eh(s.l1, dindingourmet));
}
void apply_comprardindin3(struct comprardindin3 s) {
	comprou_dindin3 = 1;
	comprou_dindin2 = 0;
}
void check_show_comprardindin3(void) {
	struct comprardindin3 s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l1 = i0;
		if (checktrue_comprardindin3(s)) show_comprardindin3();
	}
}
void initialize(void) {
	ligado[s7][corredors] = true;
	ligado[corredors][escada] = true;
	ligado[escada][corredori] = true;
	ligado[corredori][patio] = true;
	ligado[patio][io_uac] = true;
	ligado[io_uac][calcada_uac] = true;
	ligado[calcada_uac][calcada_ued] = true;
	ligado[calcada_ued][dindingourmet] = true;
	ligado[calcada_ued][saidap1] = true;
	ligado[calcada_ued][saidap2] = true;
	ligado[calcada_ued][saidacarro1] = true;
	ligado[saidap1][pistacooper] = true;
	ligado[saidap2][pistacooper] = true;
	ligado[saidacarro1][pistacooper] = true;
	ligado[pistacooper][avenida] = true;
	ligado[avenida][brt] = true;
	estou_em[s7] = true;
	lugar_eh[s7][s7] = true;
	passei_em[s7] = true;
	comprou_dindin0 = true;
	return;
}
