#ifndef GAME2D_INIMIGO2_H
#define GAME2D_INIMIGO2_H

#include <c2d2/ator.h>

#define VANDA 1

enum { INIMIGO2_PARADO = ATOR_PROG_INI, INIMIGO2_ANDANDO, INIMIGO2_MORRENDO, INIMIGO2_MORREU, INIMIGO2_ATACANDO };

bool GAME2D_CarregaInimigo2();
bool AtualizaInimigo2(Ator *a, unsigned int mapa);


#endif
