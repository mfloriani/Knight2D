#ifndef GAME2D_PLAYER_H
#define GAME2D_PLAYER_H

#include <c2d2/ator.h>

#define VANDA	3
#define VPULO   1000

enum {
	PLAYER_INICIO = ATOR_PROG_INI, PLAYER_PARADO, PLAYER_ANDANDO, PLAYER_PULANDO,
	PLAYER_CAINDO, PLAYER_VITORIA, PLAYER_MORRENDO, PLAYER_MORREU
};

bool GAME2D_CarregaPlayer();
bool AtualizaPlayer(Ator *a, unsigned int mapa);

#endif
