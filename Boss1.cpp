#include <c2d2/chien2d2.h>

#include "Atores.h"
#include "Boss1.h"

//Animacao
// Vetor com as animacoes da nave (2 no total)
Animacao animBoss1[] = {
	// Ordem: numero de quadros, tempo entre os quadros, vetor com a sequencia de quadros
	// BOSS1_PARADO ESQUERDA : 0
	{ 1, 30, { 0 } },
	// BOSS1_PARADO DIREITA : 1
	{ 1, 30, { 11 } },
	// BOSS1_ANDANDO ESQUERDA : 2
	{ 1, 30, { 1 } },
	// BOSS1_ANDANDO direita : 3
	{ 1, 30, { 10 } },
	// BOSS1_MORRENDO_ESQUERDA: 4
	{ 5, 20, { 4, 4, 5, 6, 7 } },
	// BOSS1_MORRENDO_DIREITA: 5
	{ 5, 20, { 15, 15, 14, 13, 12 } },
	// BOSS1_MORREU_ESQUERDA: 6
	{ 1, 30, { 7 } },
	// BOSS1_MORREU_DIREITA: 7
	{ 1, 30, { 7 } }
};

bool GAME2D_CarregaBoss1()
{
	return ATOR_CarregaAtorEstatico(BOSS1, "death.png", 145, 100, 47, 20, 61, 71, animBoss1, false, 0, 0, &AtualizaBoss1);
}

bool AtualizaBoss1(Ator *a, unsigned int mapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		//ATOR_TrocaEstado(a, BOSS1_PARADO, false);
		ATOR_TrocaEstado(a, BOSS1_ANDANDO, false);
		//ATOR_TrocaEstado(a, BOSS1_MORRENDO, false);
		//a->direcao = 0;
		break;
	case BOSS1_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 0;
			a->estado.subestado = ESTADO_RODANDO;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 1) : ATOR_TrocaAnimacao(a, 0);
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_DANO:
				ATOR_TrocaEstado(a, BOSS1_MORRENDO, false);
				break;
			}
		}
		break;
	case BOSS1_ANDANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = VANDA;
			a->estado.subestado = ESTADO_RODANDO;
			//a->aux_int[0] = 3;
			//ATOR_TrocaAnimacao(a, 2);
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
					//ATOR_TrocaEstado(a, BOSS1_ANDANDO, false);
				}
				break;
			case EVT_COLIDIU_PERSONAGEM:
				//ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
				break;
			case EVT_DANO:
				ATOR_TrocaEstado(a, BOSS1_MORRENDO, false);
				break;
			}
		}
		break;
	case BOSS1_MORRENDO:
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
					ATOR_TrocaEstado(a, BOSS1_MORREU, false);
				break;
			}
		}
		break;
	case BOSS1_MORREU:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 7) : ATOR_TrocaAnimacao(a, 6);
			a->velocidade = 0;
			a->temporizadores[0] = 120;
			a->estado.subestado = ESTADO_RODANDO;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
				break;
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
