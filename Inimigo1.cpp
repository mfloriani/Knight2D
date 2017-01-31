#include <c2d2/chien2d2.h>

#include "Atores.h"
#include "Inimigo1.h"

//Animacao
// Vetor com as animacoes da nave (2 no total)
Animacao animInimigo1[] = {
	// Ordem: numero de quadros, tempo entre os quadros, vetor com a sequencia de quadros
	// INIMIGO1_PARADO ESQUERDA : 0
	{ 4, 10, { 0, 1, 2, 3 } },
	// INIMIGO1_PARADO direita : 1
	{ 4, 10, { 19, 18, 17, 16 } },
	// INIMIGO1_ANDANDO ESQUERDA : 2
	{ 4, 10, {  4,  5,  6,  7 } },
	// INIMIGO1_ANDANDO direita : 3
	{ 4, 10, { 23, 22, 21, 20 } },
	// INIMIGO1_MORRENDO_ESQUERDA: 4
	{ 4, 20, {  8,  9, 10, 11 } },
	// INIMIGO1_MORRENDO_DIREITA: 5
	{ 4, 20, { 27, 26, 25, 24 } },
	// INIMIGO1_MORREU_ESQUERDA: 6
	{ 1, 60, { 11 } },
	// INIMIGO1_MORREU_DIREITA: 7
	{ 1, 60, { 24 } },
	// INIMIGO1_ATACANDO_ESQUERDA: 8
	{ 4, 15, { 12, 13, 14, 15 } },
	// INIMIGO1_ATACANDO_DIREITA: 9
	{ 4, 15, { 31, 30, 29, 28 } }
};

bool GAME2D_CarregaInimigo1()
{
	return ATOR_CarregaAtorEstatico(INIMIGO1, "skeleton.png", 125, 115, 22, 25, 71, 87, animInimigo1, false, 0, 0, &AtualizaInimigo1);
	//return ATOR_CarregaAtorEstatico(INIMIGO1, "skeleton.png", 125, 115, 32, 34, 58, 79, animInimigo1, false, 0, 0, &AtualizaInimigo1);
}

bool AtualizaInimigo1(Ator *a, unsigned int mapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		//ATOR_TrocaEstado(a, INIMIGO1_PARADO, false);
		ATOR_TrocaEstado(a, INIMIGO1_ANDANDO, false);
		//ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
		//ATOR_TrocaEstado(a, INIMIGO1_ATACANDO, false);
		//a->direcao = 0;
		break;
	case INIMIGO1_PARADO:
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
	case INIMIGO1_ANDANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 1;
			a->estado.subestado = ESTADO_RODANDO;
			a->aux_int[0] = 3;
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
					//ATOR_TrocaEstado(a, INIMIGO1_ANDANDO, false);
				}
				break;
			case EVT_COLIDIU_PERSONAGEM:
				//ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
				break;
			case EVT_DANO:
				ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
				break;
			}
		}
		break;
	case INIMIGO1_ATACANDO:
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
				//ATOR_TrocaEstado(a, INIMIGO1_MORRENDO, false);
				break;
			}
		}
		break;
	case INIMIGO1_MORRENDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->invulneravel = true;
			if (a->direcao == 0)
			{
				ATOR_TrocaAnimacao(a, 5);
			}
			else
			{
				ATOR_TrocaAnimacao(a, 4);
			}

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
					ATOR_TrocaEstado(a, INIMIGO1_MORREU, false);
				break;
			}
		}
		break;
	case INIMIGO1_MORREU:
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
