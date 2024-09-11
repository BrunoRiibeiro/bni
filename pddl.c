#include <stdio.h>
#include "pddl.h"

dindinMap dindin_map[LENGTH_dindin] = {
	{"d1", d1},
	{"d2", d2},
	{"d3", d3},
	{"d4", d4},
	{"d5", d5},
	{"d6", d6},
	{"d7", d7},
	{"d8", d8},
	{"d9", d9},
};
const char *get_dindin_names(enum dindin e) {
	if (e >= 0 && e < LENGTH_dindin)
		return dindin_map[e].str;
	return NULL;
}
enum dindin get_dindin_enum(const char *s) {
	for (int i = 0; dindin_map[i].str != NULL; i++)
		if (strcmp(s, dindin_map[i].str) == 0)
			return dindin_map[i].value;
	return LENGTH_dindin;
}
lugarMap lugar_map[LENGTH_lugar] = {
	{"s7", s7},
	{"corredors", corredors},
	{"escada", escada},
	{"corredori", corredori},
	{"patio", patio},
	{"io_uac", io_uac},
	{"calcada_uac", calcada_uac},
	{"calcada_ued", calcada_ued},
	{"saidap1", saidap1},
	{"dindingourmet", dindingourmet},
	{"saidacarro1", saidacarro1},
	{"saidap2", saidap2},
	{"pistacooper", pistacooper},
	{"avenida", avenida},
	{"brt", brt},
};
const char *get_lugar_names(enum lugar e) {
	if (e >= 0 && e < LENGTH_lugar)
		return lugar_map[e].str;
	return NULL;
}
enum lugar get_lugar_enum(const char *s) {
	for (int i = 0; lugar_map[i].str != NULL; i++)
		if (strcmp(s, lugar_map[i].str) == 0)
			return lugar_map[i].value;
	return LENGTH_lugar;
}
bool ligado[15][15];
bool estou_em[15];
bool passei_em[15];
bool comprar_dindin[24];
bool tem_dindin[24];
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
bool checktrue_comprar_dindin(int d) {
	return comprar_dindin[d];
}
bool checktrue_tem_dindin(int d) {
	return tem_dindin[d];
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
void check_show_mover(FILE *f) {
	struct mover s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l2 = i0;
		for (int i1 = 0; i1 < LENGTH_lugar; i1++) {
			s.l1 = i1;
			if (checktrue_mover(s)) fprintf(f, "mover(%s,%s)\n", get_lugar_names(i1), get_lugar_names(i0));
		}
	}
}
bool checktrue_comprardindin(struct comprardindin s) {
	return (checktrue_lugar_eh(s.l, dindingourmet) and checktrue_tem_dindin(s.d));
}
void apply_comprardindin(struct comprardindin s) {
	comprar_dindin[s.d] = 1;
	tem_dindin[s.d] = 0;
}
void check_show_comprardindin(FILE *f) {
	struct comprardindin s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l = i0;
		for (int i1 = 0; i1 < LENGTH_dindin; i1++) {
			s.d = i1;
			if (checktrue_comprardindin(s)) fprintf(f, "comprardindin(%s,%s)\n", get_dindin_names(i1), get_lugar_names(i0));
		}
	}
}
bool checktrue_comprar_todos_dindin(struct comprar_todos_dindin s) {
	bool forall1 = true;
	for (int i0 = 0; i0 < LENGTH_dindin; i0++) {
		if (!checktrue_tem_dindin(i0)) {forall1 = false; break;}
	}
	return ((forall1) and checktrue_lugar_eh(s.l, dindingourmet));
}
void apply_comprar_todos_dindin(struct comprar_todos_dindin s) {
	for (int i0 = 0; i0 < LENGTH_dindin; i0++) {
		comprar_dindin[i0] = 1;
		tem_dindin[i0] = 0;
	}
}
void check_show_comprar_todos_dindin(FILE *f) {
	struct comprar_todos_dindin s;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		s.l = i0;
		if (checktrue_comprar_todos_dindin(s)) fprintf(f, "comprar_todos_dindin(%s)\n", get_lugar_names(i0));
	}
}
bool checktrue_terremoto(struct terremoto s) {
	bool forall1 = true;
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		bool forall2 = true;
		for (int i1 = 0; i1 < LENGTH_dindin; i1++) {
			if (!(true)) {forall2 = false; break;}
		}
		if (!((forall2))) {forall1 = false; break;}
	}
	return (forall1);
}
void apply_terremoto(struct terremoto s) {
	for (int i0 = 0; i0 < LENGTH_lugar; i0++) {
		for (int i1 = 0; i1 < LENGTH_lugar; i1++) {
			ligado[i0][i1] = 0;
		}
	}
}
void check_show_terremoto(FILE *f) {
	struct terremoto s;
	if (checktrue_terremoto(s)) fprintf(f, "terremoto()\n");
}
void check_show_actions(const char *filename) {
	FILE *f = fopen(filename, "w");
	check_show_mover(f);
	check_show_comprardindin(f);
	check_show_comprar_todos_dindin(f);
	check_show_terremoto(f);
	fclose(f);
}
int apply_actions(char *s) {
	const char *basename = strsep(&s, "(");
	if (strcmp(basename, "mover") == 0) {
		struct mover tcurts;
		char *token;
		token = strsep(&s, ",");
		tcurts.l1 = get_lugar_enum(strsep(&token, ")"));
		token = strsep(&s, ",");
		tcurts.l2 = get_lugar_enum(strsep(&token, ")"));
		if (checktrue_mover(tcurts)) {
			apply_mover(tcurts);
			return 0;
		} else return 1;
	}
	if (strcmp(basename, "comprardindin") == 0) {
		struct comprardindin tcurts;
		char *token;
		token = strsep(&s, ",");
		tcurts.d = get_dindin_enum(strsep(&token, ")"));
		token = strsep(&s, ",");
		tcurts.l = get_lugar_enum(strsep(&token, ")"));
		if (checktrue_comprardindin(tcurts)) {
			apply_comprardindin(tcurts);
			return 0;
		} else return 1;
	}
	if (strcmp(basename, "comprar_todos_dindin") == 0) {
		struct comprar_todos_dindin tcurts;
		char *token;
		token = strsep(&s, ",");
		tcurts.l = get_lugar_enum(strsep(&token, ")"));
		if (checktrue_comprar_todos_dindin(tcurts)) {
			apply_comprar_todos_dindin(tcurts);
			return 0;
		} else return 1;
	}
	if (strcmp(basename, "terremoto") == 0) {
		struct terremoto tcurts;
		char *token;
		if (checktrue_terremoto(tcurts)) {
			apply_terremoto(tcurts);
			return 0;
		} else return 1;
	}
	return 2;
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
	tem_dindin[d1] = true;
	tem_dindin[d2] = true;
	tem_dindin[d3] = true;
	tem_dindin[d4] = true;
	tem_dindin[d5] = true;
	tem_dindin[d6] = true;
	tem_dindin[d7] = true;
	tem_dindin[d8] = true;
	tem_dindin[d9] = true;
	return;
}
bool checktrue_goal(void) {
	bool forall1 = true;
	for (int i0 = 0; i0 < LENGTH_dindin; i0++) {
		if (!!(checktrue_tem_dindin(i0))) {forall1 = false; break;}
	}
	return (checktrue_estou_em(calcada_ued) and (forall1) and !(checktrue_ligado(calcada_ued, saidap1)));
}
