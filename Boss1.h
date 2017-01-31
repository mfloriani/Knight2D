#ifndef GAME2D_BOSS1_H
#define GAME2D_BOSS1_H

#include <c2d2/ator.h>

#define VANDA	3

enum { BOSS1_PARADO = ATOR_PROG_INI, BOSS1_ANDANDO, BOSS1_MORRENDO, BOSS1_MORREU };

bool GAME2D_CarregaBoss1();
bool AtualizaBoss1(Ator *a, unsigned int mapa);


#endif
