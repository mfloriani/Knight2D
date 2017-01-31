#include <c2d2/chien2d2.h>

#include "Atores.h"
#include "Boss2.h"

//Animacao
// Vetor com as animacoes da nave (2 no total)
Animacao animBoss2[] = {
	// Ordem: numero de quadros, tempo entre os quadros, vetor com a sequencia de quadros
	// BOSS2_PARADO ESQUERDA : 0
	{ 4, 15, { 0, 1, 2, 3 } },
	// BOSS2_PARADO DIREITA : 1
	{ 4, 15, { 31, 30, 29, 28 } },
	// BOSS2_ANDANDO ESQUERDA : 2
	{ 4, 15, { 4, 5, 6, 7 } },
	// BOSS2_ANDANDO direita : 3
	{ 4, 15, { 27, 26, 25, 24 } },
	// BOSS2_MORRENDO_ESQUERDA: 4
	{ 8, 10, { 16, 17, 18, 19, 20, 21, 22, 23 } },
	// BOSS2_MORRENDO_DIREITA: 5
	{ 8, 10, { 47, 46, 45, 44, 43, 42, 41, 40 } },
	// BOSS2_MORREU_ESQUERDA: 6
	{ 1, 30, { 23 } },
	// BOSS2_MORREU_DIREITA: 7
	{ 1, 30, { 40 } },
	// BOSS2_ESPECIAL_ESQUERDA: 8
	{ 8, 10, { 8, 9, 10, 11, 12, 13, 14, 15 } },
	// BOSS2_ESPECIAL_DIREITA: 9
	{ 8, 10, { 39, 38, 37, 36, 35, 34, 33, 32 } },
};

bool GAME2D_CarregaBoss2()
{
	return ATOR_CarregaAtorEstatico(BOSS2, "rider.png", 160, 150, 36, 26, 110, 124, animBoss2, false, 0, 0, &AtualizaBoss2);
}

bool AtualizaBoss2(Ator *a, unsigned int mapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		//ATOR_TrocaEstado(a, BOSS2_PARADO, false);
		ATOR_TrocaEstado(a, BOSS2_ANDANDO, false);
		//ATOR_TrocaEstado(a, BOSS2_MORRENDO, false);
		//ATOR_TrocaEstado(a, BOSS2_ESPECIAL, false);
		a->direcao = 0;
		break;
	case BOSS2_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 0;
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 1);
			else
				ATOR_TrocaAnimacao(a, 0);
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_DANO:
				ATOR_TrocaEstado(a, BOSS2_MORRENDO, false);
				break;
			}
		}
		break;
	case BOSS2_ANDANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = VANDA;
			a->estado.subestado = ESTADO_RODANDO;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 3) : ATOR_TrocaAnimacao(a, 2);
			a->temporizadores[0] = 119;
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
					ATOR_TrocaEstado(a, BOSS2_ANDANDO, false);
				}
				break;
			case EVT_COLIDIU_PERSONAGEM:
				//ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
				break;
			case EVT_DANO:
				ATOR_TrocaEstado(a, BOSS2_MORRENDO, false);
				break;
			case EVT_TEMPO:
				if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, BOSS2_ESPECIAL, false);
					break;
			}
		}
		break;
	case BOSS2_MORRENDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->invulneravel = true;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 5) : ATOR_TrocaAnimacao(a, 4);
			a->velocidade = 0;
			a->temporizadores[0] = 30;
			a->estado.subestado = ESTADO_RODANDO;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
				break;
			case EVT_RECOMECOU_ANIMACAO:
				//ATOR_TrocaEstado(a, BOSS2_MORREU, false);
				break;
			case EVT_TEMPO:
				if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, BOSS2_MORREU, false);
				break;
			}
		}
		break;
	case BOSS2_MORREU:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 7) : ATOR_TrocaAnimacao(a, 6);
			a->velocidade = 0;
			a->temporizadores[0] = 20;
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
	case BOSS2_ESPECIAL:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 9) : ATOR_TrocaAnimacao(a, 8);
			a->velocidade = 0;
			a->temporizadores[1] = 60;
			a->estado.subestado = ESTADO_RODANDO;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
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
					//ATOR_TrocaEstado(a, BOSS2_ANDANDO, false);
				}
				break;
			case EVT_TEMPO:
				if (ev.subtipo == 1)
				{
					a->velocidade = 10;
					a->temporizadores[2] = 60;
				}
				if (ev.subtipo == 2)
				{
					ATOR_TrocaEstado(a, BOSS2_ANDANDO, false);
				}
				break;
			}
		}
		break;
	case ATOR_ENCERRADO:
		return false;
	}
	return true;
}
