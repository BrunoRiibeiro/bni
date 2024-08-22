#ifndef PDDL_H
#define PDDL_H
#include <stdbool.h>
#include <string.h>
#define and &&
#define or ||

enum dindin {
	d1,
	d2,
	d3,
	d4,
	d5,
	d6,
	d7,
	d8,
	d9,
	LENGTH_dindin
};
typedef struct dindinMap {
	const char *str;
	enum dindin value;
}dindinMap;
const char *get_dindin_names(enum dindin e);
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
bool checktrue_comprar_dindin(int d);
bool checktrue_tem_dindin(int d);
bool checktrue_lugar_eh(int l1, int l2);
void check_show_mover(FILE *f);
struct mover {
	enum lugar l2;
	enum lugar l1;
};
bool checktrue_mover(struct mover s);
void apply_mover(struct mover s);
void check_show_comprardindin(FILE *f);
struct comprardindin {
	enum lugar l;
	enum dindin d;
};
bool checktrue_comprardindin(struct comprardindin s);
void apply_comprardindin(struct comprardindin s);
void check_show_comprar_todos_dindin(FILE *f);
struct comprar_todos_dindin {
	enum lugar l;
};
bool checktrue_comprar_todos_dindin(struct comprar_todos_dindin s);
void apply_comprar_todos_dindin(struct comprar_todos_dindin s);
void check_show_terremoto(FILE *f);
struct terremoto {
};
bool checktrue_terremoto(struct terremoto s);
void apply_terremoto(struct terremoto s);
void check_show_actions(const char *filename);
int apply_actions(char *s);
void initialize(void);
bool checktrue_goal(void);
#endif /* PDDL_H */
