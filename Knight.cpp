#include <c2d2/chien2d2.h>
#include "Atores.h"
#include "Knight.h"

Animacao animKnight[] = {
	// Ordem: numero de quadros, tempo entre os quadros, vetor com a sequencia de quadros
	// KNIGHT_PARADO direita: 0
	{ 10, 10, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } },
	// KNIGHT_PARADO esquerda: 1
	{ 10, 10, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 } },
	// KNIGHT_ANDANDO direita: 2
	{ 10, 5, { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 } },
	// KNIGHT_ANDANDO esquerda: 3
	{ 10, 5, { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 } },
	// KNIGHT_PULANDO direita: 4
	{ 1, 1, { 85 } },
	// KNIGHT_PULANDO esquerda: 5
	{ 1, 1, { 90 } },
	// KNIGHT_CAINDO direita: 6
	{ 1, 1, { 85 } },
	// KNIGHT_CAINDO esquerda: 7
	{ 1, 1, { 90 } },
	// KNIGHT_VITORIA: 8
	{ 2, 30, { 9, 10 } },
	// KNIGHT_MORRENDO: 9
	{ 1, 1, { 12 } },
	// KNIGHT_MORREU: 10
	{ 1, 1, { 11 } },
	// KNIGHT_ATACANDO_ESQUERDA: 11
	{ 10, 2, { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 } },
	// KNIGHT_ATACANDO_DIREITA: 12
	{ 10, 2, { 59, 58, 57, 56, 55, 54, 53, 52, 51, 50 } },
};

char *sonsKnight[] = {
	"jump.wav",
	"bate.wav",
	"die.wav",
	"sword.wav"
};


bool GAME2D_CarregaKnight()
{
	return ATOR_CarregaAtorEstatico(PLAYER, "knight.png", 48, 60, 0, 0, 48, 58, animKnight, false, sonsKnight, 4, &AtualizaKnight);
}

bool AtualizaKnight(Ator *a, unsigned int mapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, KNIGHT_INICIO, false);
		//ATOR_TrocaEstado(a, KNIGHT_ATACANDO, false);
		//a->direcao = 180;
		a->vidas = 3;
		a->aux_int[0] = 0;
		//a->invulneravel = true;
		break;
	case KNIGHT_INICIO:
		// Indica para qual direcao esta olhando
		a->olhandoPara = 0;
		// Guarda a posicao inicial
		a->aux_real[0] = a->x;
		a->aux_real[1] = a->y;
		ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
		break;
	case KNIGHT_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = 0;
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 0);
			else
				ATOR_TrocaAnimacao(a, 1);
		}

		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_PRESSIONOU_BOTAO1:
				ATOR_TrocaEstado(a, KNIGHT_PULANDO, false);
				break;
			case EVT_PRESSIONOU_BOTAO2:
				ATOR_TrocaEstado(a, KNIGHT_ATACANDO, false);
				break;
			case EVT_PRESSIONOU_ESQ:
				ATOR_TrocaEstado(a, KNIGHT_ANDANDO, false);
				a->direcao = 180;
				break;
			case EVT_PRESSIONOU_DIR:
				ATOR_TrocaEstado(a, KNIGHT_ANDANDO, false);
				a->direcao = 0;
				break;
			case EVT_COMECOU_CAIR:
				ATOR_TrocaEstado(a, KNIGHT_CAINDO, false);
				break;
			case EVT_COLIDIU_ARMADILHA:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_CHECKPOINT:
				a->aux_real[0] = a->x;
				a->aux_real[1] = a->y;
				break;
			case EVT_FIM_FASE:
				ATOR_TrocaEstado(a, KNIGHT_VITORIA, false);
				break;
			case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			}
		}
		break;
	case KNIGHT_PULANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_Impulsiona(a, VPULO);
			ATOR_TocaEfeitoTela(a, 0, mapa);
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 4);
			else
				ATOR_TrocaAnimacao(a, 5);

		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_TOPO_PULO:
				ATOR_TrocaEstado(a, KNIGHT_CAINDO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_CIMA)
				{
					ATOR_TrocaEstado(a, KNIGHT_CAINDO, false);
					ATOR_TocaEfeitoTela(a, 1, mapa);
				}
				break;
			case EVT_PRESSIONOU_ESQ:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 180;
					ATOR_TrocaAnimacao(a, 5);
				}
				break;
			case EVT_PRESSIONOU_DIR:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 0;
					ATOR_TrocaAnimacao(a, 4);
				}
				break;
			case EVT_LIBEROU_ESQ:
				if (a->velocidade != 0 && a->direcao == 180)
					a->velocidade = 0;
				break;
			case EVT_LIBEROU_DIR:
				if (a->velocidade != 0 && a->direcao == 0)
					a->velocidade = 0;
				break;
			case EVT_SAIU_FORA_MAPA:
				if (ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
					ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_COLIDIU_ARMADILHA:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_CHECKPOINT:
				a->aux_real[0] = a->x;
				a->aux_real[1] = a->y;
				break;
			case EVT_FIM_FASE:
				ATOR_TrocaEstado(a, KNIGHT_VITORIA, false);
				break;
			case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_ESCADA:
				//ATOR_TrocaEstado(a, KNIGHT_SUBINDO_ESCADA, false);
				break;
			/*case EVT_PRESSIONOU_CIMA:
				printf("EVT_PRESSIONOU_CIMA\n");
				if (a->aux == 1)
				{
					printf("TA NA ESCADA\n");
					ATOR_TrocaEstado(a, KNIGHT_SUBINDO_ESCADA, false);
					
					//a->aux = 3;
				}
				//else if (a->aux ==3)
				//ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;*/
			}
		}
		break;
	/*case KNIGHT_SUBINDO_ESCADA:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			printf("KNIGHT_SUBINDO_ESCADA\n");
			a->estado.subestado = ESTADO_RODANDO;
			//a->direcao = 90;
			a->velocidade = 0;
			//a->gravidade = false;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_PRESSIONOU_CIMA:
				//a->y -= 10;
				break;
			/*case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_BAIXO)
				{
					ATOR_TocaEfeitoTela(a, 1, mapa);
					if (a->velocidade == 0)
						ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
					else
						ATOR_TrocaEstado(a, KNIGHT_ANDANDO, false);
				}
				break;
			case EVT_PRESSIONOU_CIMA:
				a->y -= 10;
				break;
			
			default:
				//ATOR_TrocaEstado(a, KNIGHT_CAINDO, false);
				break;
			}
		}
		break;*/
	case KNIGHT_CAINDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 6);
			else
				ATOR_TrocaAnimacao(a, 7);

		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_SAIU_FORA_MAPA:
				if (ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
					ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_BAIXO)
				{
					ATOR_TocaEfeitoTela(a, 1, mapa);
					if (a->velocidade == 0)
						ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
					else
						ATOR_TrocaEstado(a, KNIGHT_ANDANDO, false);
				}
				break;
			case EVT_PRESSIONOU_ESQ:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 180;
					ATOR_TrocaAnimacao(a, 5);
				}
				break;
			case EVT_PRESSIONOU_DIR:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 0;
					ATOR_TrocaAnimacao(a, 4);
				}
				break;
			case EVT_LIBEROU_ESQ:
				if (a->velocidade != 0 && a->direcao == 180)
					a->velocidade = 0;
				break;
			case EVT_LIBEROU_DIR:
				if (a->velocidade != 0 && a->direcao == 0)
					a->velocidade = 0;
				break;
			case EVT_COLIDIU_ARMADILHA:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_CHECKPOINT:
				a->aux_real[0] = a->x;
				a->aux_real[1] = a->y;
				break;
			case EVT_FIM_FASE:
				ATOR_TrocaEstado(a, KNIGHT_VITORIA, false);
				break;
			case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_MOLA:
				ATOR_TocaEfeitoTela(a, 0, mapa);
				ATOR_Impulsiona(a, 2000);
				break;
			}
		}

		break;

	case KNIGHT_ANDANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->velocidade = VANDA;
			a->estado.subestado = ESTADO_RODANDO;
			if (a->direcao == 0)
				ATOR_TrocaAnimacao(a, 2);
			else
				ATOR_TrocaAnimacao(a, 3);
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_PRESSIONOU_BOTAO1:
				ATOR_TrocaEstado(a, KNIGHT_PULANDO, false);
				break;
			case EVT_PRESSIONOU_BOTAO2:
				ATOR_TrocaEstado(a, KNIGHT_ATACANDO, false);
				break;
			case EVT_LIBEROU_ESQ:
				if (a->direcao == 180)
					ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;
			case EVT_LIBEROU_DIR:
				if (a->direcao == 0)
					ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_CIMA)
					ATOR_TocaEfeitoTela(a, 1, mapa);
				break;
			case EVT_SAIU_FORA_MAPA:
				if (ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
					ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_COLIDIU_ARMADILHA:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_CHECKPOINT:
				a->aux_real[0] = a->x;
				a->aux_real[1] = a->y;
				break;
			case EVT_FIM_FASE:
				ATOR_TrocaEstado(a, KNIGHT_VITORIA, false);
				break;
			case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, KNIGHT_MORRENDO, false);
				break;
			case EVT_MOEDA:
				a->aux_int[0]++;
				break;
			case EVT_VIDA:
				a->aux_int[0]++;
				break;
			}
		}
		break;
	case KNIGHT_ATACANDO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_TocaEfeitoTela(a, 3, mapa);
			a->estado.subestado = ESTADO_RODANDO;
			a->direcao == 0 ? ATOR_TrocaAnimacao(a, 11) : ATOR_TrocaAnimacao(a, 12);
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_PRESSIONOU_BOTAO1:
				ATOR_TrocaEstado(a, KNIGHT_PULANDO, false);
				break;
			case EVT_PRESSIONOU_ESQ:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 180;
					ATOR_TrocaAnimacao(a, 5);
				}
				break;
			case EVT_PRESSIONOU_DIR:
				if (a->velocidade == 0)
				{
					a->velocidade = VANDA;
					a->direcao = 0;
					ATOR_TrocaAnimacao(a, 4);
				}
				break;
			case EVT_LIBEROU_ESQ:
				if (a->direcao == 180)
					ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;
			case EVT_LIBEROU_DIR:
				if (a->direcao == 0)
					ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_CIMA)
					ATOR_TocaEfeitoTela(a, 1, mapa);
				break;
			case EVT_COLIDIU_PERSONAGEM:
				break;
			case EVT_RECOMECOU_ANIMACAO:
				ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
				break;
			}
		}
		break;
	case KNIGHT_MORRENDO:
		//ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
		//break;
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_Impulsiona(a, VPULO*1.5);
			ATOR_TrocaAnimacao(a, 9);
			ATOR_TocaEfeitoTela(a, 2, mapa);
			a->temporizadores[0] = 120;
			a->estado.subestado = ESTADO_RODANDO;
			a->velocidade = 0;
			a->vidas--;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_COLIDIU_PAREDE:
				if (ev.subtipo == SUBEVT_COLIDIU_PAREDE_BAIXO)
					ATOR_TrocaEstado(a, KNIGHT_MORREU, false);
				break;
			case EVT_TEMPO:
				if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, KNIGHT_MORREU, false);
				break;
			}
		}
		break;
	case KNIGHT_MORREU:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_TrocaAnimacao(a, 10);
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
				{
					if (a->vidas>0)
					{
						a->x = a->aux_real[0];
						a->y = a->aux_real[1];
						a->direcao = 0;
						ATOR_TrocaEstado(a, KNIGHT_PARADO, false);
					}
					else
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				}
				break;
			}
		}
		break;
	case ATOR_ENCERRADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->estado.subestado = ESTADO_RODANDO;
			a->velocidade = 0;
			ev.tipoEvento = EVT_FIM_FASE;
			ev.subtipo = SUBEVT_FIM_FASE_DERROTA;
			ATOR_EnviaEventoJogo(&ev);
		}

		return false;
	case KNIGHT_VITORIA:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			a->estado.subestado = ESTADO_RODANDO;
			ATOR_TrocaAnimacao(a, 8);
			a->temporizadores[0] = 60;
			a->velocidade = 0;
		}
		while (ATOR_ProximoEvento(a, &ev))
		{
			if (ev.tipoEvento == EVT_TEMPO)
			{
				ev.tipoEvento = EVT_FIM_FASE;
				ev.subtipo = SUBEVT_FIM_FASE_VITORIA;
				ATOR_EnviaEventoJogo(&ev);
			}
		}
		break;
	}
	return true;
}
