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
	LENGTH_lugar
};
typedef struct lugarMap {
	const char *str;
	enum lugar value;
}lugarMap;
const char *get_lugar_names(enum lugar e);
bool checktrue_ligado(int l1, int l2);
bool checktrue_estou_em(int l);
bool checktrue_passei_em(int l);
bool checktrue_comprou_dindin0();
bool checktrue_comprou_dindin1();
bool checktrue_comprou_dindin2();
bool checktrue_comprou_dindin3();
bool checktrue_lugar_eh(int l1, int l2);
void check_show_mover(void);
struct mover {
	enum lugar l2;
	enum lugar l1;
};
bool checktrue_mover(struct mover s);
void apply_mover(struct mover s);
void check_show_comprardindin1(void);
struct comprardindin1 {
	enum lugar l1;
};
bool checktrue_comprardindin1(struct comprardindin1 s);
void apply_comprardindin1(struct comprardindin1 s);
void check_show_comprardindin2(void);
struct comprardindin2 {
	enum lugar l1;
};
bool checktrue_comprardindin2(struct comprardindin2 s);
void apply_comprardindin2(struct comprardindin2 s);
void check_show_comprardindin3(void);
struct comprardindin3 {
	enum lugar l1;
};
bool checktrue_comprardindin3(struct comprardindin3 s);
void apply_comprardindin3(struct comprardindin3 s);
void show_actions(void);
int apply_actions(char *s);
void initialize(void);
bool checktrue_goal(void);
#endif /* PDDL_H */
