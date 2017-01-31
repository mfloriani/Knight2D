#ifndef GAME2D_BOSS2_H
#define GAME2D_BOSS2_H

#include <c2d2/ator.h>

#define VANDA	2

enum { BOSS2_PARADO = ATOR_PROG_INI, BOSS2_ANDANDO, BOSS2_MORRENDO, BOSS2_MORREU, BOSS2_ESPECIAL };

bool GAME2D_CarregaBoss2();
bool AtualizaBoss2(Ator *a, unsigned int mapa);


#endif
