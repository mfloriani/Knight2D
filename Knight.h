#ifndef GAME2D_KNIGHT_H
#define GAME2D_KNIGHT_H

#include <c2d2/ator.h>

#define VANDA	3
#define VPULO   1000

enum {
	KNIGHT_INICIO = ATOR_PROG_INI, KNIGHT_PARADO, KNIGHT_ANDANDO, KNIGHT_PULANDO,
	KNIGHT_CAINDO, KNIGHT_VITORIA, KNIGHT_MORRENDO, KNIGHT_MORREU, KNIGHT_ATACANDO,
	KNIGHT_SUBINDO_ESCADA
};

bool GAME2D_CarregaKnight();
bool AtualizaKnight(Ator *a, unsigned int mapa);

#endif