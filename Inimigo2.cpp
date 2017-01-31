#include <c2d2/chien2d2.h>

#include "Atores.h"
#include "Inimigo2.h"

//Animacao
Animacao animInimigo2[] = {
	// Ordem: numero de quadros, tempo entre os quadros, vetor com a sequencia de quadros
	// INIMIGO2_PARADO ESQUERDA : 0
	{ 2, 30, {  8,  9 } },
	// INIMIGO2_PARADO direita : 1
	{ 2, 30, { 27, 26 } },
	// INIMIGO2_ANDANDO ESQUERDA : 2
	{ 4, 10, { 4, 5, 6, 7 } },
	// INIMIGO2_ANDANDO direita : 3
	{ 4, 10, { 23, 22, 21, 20 } },
	// INIMIGO2_MORRENDO_ESQUERDA: 4
	{ 8, 10, {  8,  9, 10, 11, 12, 13, 14, 15 } },
	// INIMIGO2_MORRENDO_DIREITA: 5
	{ 8, 10, { 31, 30, 29, 28, 35, 34, 33, 32 } },
	// INIMIGO2_MORREU_ESQUERDA: 6
	{ 1, 60, { 15 } },
	// INIMIGO2_MORREU_DIREITA: 7
	{ 1, 60, { 28 } },
	// INIMIGO2_ATACANDO_ESQUERDA: 8
	{ 4, 15, {  0,  1,  2,  3 } },
	// INIMIGO2_ATACANDO_DIREITA: 9
	{ 4, 15, { 19, 18, 17, 16 } }
};

bool GAME2D_CarregaInimigo2()
{
	return ATOR_CarregaAtorEstatico(INIMIGO2, "zombie.png", 80, 80, 15, 19, 51, 60, animInimigo2, false, 0, 0, &AtualizaInimigo2);
}

bool AtualizaInimigo2(Ator *a, unsigned int mapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		//ATOR_TrocaEstado(a, INIMIGO2_PARADO, false);
		ATOR_TrocaEstado(a, INIMIGO2_ANDANDO, false);
		//ATOR_TrocaEstado(a, INIMIGO2_MORRENDO, false);
		//ATOR_TrocaEstado(a, INIMIGO2_ATACANDO, false);
		//a->direcao = 0;
		break;
	case INIMIGO2_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 0;
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 1);
			else
				ATOR_TrocaAnimacao(a, 0);
		}
		break;
	case INIMIGO2_ANDANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = VANDA;
			a->estado.subestado = ESTADO_RODANDO;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 3) : ATOR_TrocaAnimacao(a, 2);
		}

		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_FOCO_TELA:
				printf("Entrou.\n");
				break;
			case EVT_PERDE_FOCO_TELA:
				printf("Saiu.\n");
				break;
			case EVT_SAIU_FORA_MAPA:
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_ESQ || ev.subtipo == SUBEVT_COLIDIU_PAREDE_DIR)
				{
					if (a->direcao == 0)
					{
						a->direcao = 180;
						ATOR_TrocaAnimacao(a, 2);
					}
					else
					{
						a->direcao = 0;
						ATOR_TrocaAnimacao(a, 3);
					}
				}
				break;
			case EVT_COLIDIU_PERSONAGEM:
				break;
			case EVT_DANO:
				ATOR_TrocaEstado(a, INIMIGO2_MORRENDO, false);
				break;
			}
		}
		break;
	case INIMIGO2_ATACANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 0;
			a->estado.subestado = ESTADO_RODANDO;
			a->aux_int[0] = 3;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 9) : ATOR_TrocaAnimacao(a, 8);
		}

		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_FOCO_TELA:
				printf("Entrou.\n");
				break;
			case EVT_PERDE_FOCO_TELA:
				printf("Saiu.\n");
				break;
			case EVT_SAIU_FORA_MAPA:
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				break;
			case EVT_COLIDIU_PERSONAGEM:
				break;
			case EVT_DANO:
				ATOR_TrocaEstado(a, INIMIGO2_MORRENDO, false);
				break;
			}
		}
		break;
	case INIMIGO2_MORRENDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->invulneravel = true;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 5) : ATOR_TrocaAnimacao(a, 4);			
			a->velocidade = 0;
			a->temporizadores[0] = 60;
			a->estado.subestado = ESTADO_RODANDO;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
				break;
			case EVT_TEMPO:
				if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, INIMIGO2_MORREU, false);
				break;
			}
		}
		break;
	case INIMIGO2_MORREU:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 7) : ATOR_TrocaAnimacao(a, 6);
			a->velocidade = 0;
			a->temporizadores[0] = 60;
			a->estado.subestado = ESTADO_RODANDO;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_TEMPO:
				if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;
			}
		}
		break;
	case ATOR_ENCERRADO:
		return false;
	}
	return true;
}
