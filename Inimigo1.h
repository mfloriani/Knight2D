#ifndef GAME2D_INIMIGO1_H
#define GAME2D_INIMIGO1_H

#include <c2d2/ator.h>

#define VANDA	3

enum { INIMIGO1_PARADO = ATOR_PROG_INI, INIMIGO1_ANDANDO, INIMIGO1_MORRENDO, INIMIGO1_MORREU, INIMIGO1_ATACANDO };

bool GAME2D_CarregaInimigo1();
bool AtualizaInimigo1(Ator *a, unsigned int mapa);


#endif
