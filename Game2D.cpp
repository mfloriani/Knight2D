#include <stdio.h>
#include <string> 
#include <c2d2/chien2d2.h>
#include <c2d2/chien2d2mapa.h>
#include <c2d2/chien2d2primitivas.h>
#include <c2d2/chienaudio2.h>
#include <c2d2/ator.h>
#include "Atores.h"
#include "Knight.h"
#include "Inimigo1.h"
#include "Inimigo2.h"
#include "Boss1.h"
#include "Boss2.h"

#define LARGURA_TELA		    1024
#define ALTURA_TELA			    600
#define GRAVIDADE			    250
#define MAXGRAVIDADE		    2000
#define TILE_MARCAS			    97
#define PLAYER				    0
#define BOTAO_JOGAR_X		    412
#define BOTAO_JOGAR_Y		    200
#define BOTAO_CARREGAR_X	    412
#define BOTAO_CARREGAR_Y	    250
#define BOTAO_CREDITOS_X	    412
#define BOTAO_CREDITOS_Y	    300
#define BOTAO_SAIR_X		    412
#define BOTAO_SAIR_Y		    400
#define BOTAO_RETORNAR_X	    412
#define BOTAO_RETORNAR_Y	    500
#define BOTAO_CARREGAR_FASE2_X	370
#define BOTAO_CARREGAR_FASE2_Y	225


unsigned int fonte32 = 0;
unsigned int botao_jogar = 0;
unsigned int botao_creditos = 0;
unsigned int botao_voltar = 0;
unsigned int botao_sair = 0;
unsigned int spmira = 0;
unsigned int titulo = 0;
unsigned int gameover = 0;
unsigned int creditos = 0;
unsigned int playerLife = 0;
unsigned int carregar = 0;
unsigned int botao_carregar = 0;
unsigned int botao_carrega_fase02 = 0;

unsigned int mapa1 = 0;
unsigned int mapa2 = 0;

unsigned int musicas[5];
unsigned int musicaAtual = 9999;

int numInimigos = 0;
Ator **inimigos = NULL;
Ator *player = NULL;
Evento ev;

enum { TELA_MENU = 1, TELA_FASE_01, TELA_FASE_02, TELA_CREDITOS, TELA_GAMEOVER, TELA_CARREGAR };
enum { FASE_01 = 1, FASE_02};
enum { MUSICA_F1, MUSICA_F2, MUSICA_BOSS, MUSICA_GAMEOVER, MUSICA_WIN};

int estadoAtual = TELA_MENU;
int faseAtual = FASE_01;
bool encerra = false;

void ProcessaControle(Ator *a)
{
	static Evento ev;
	static C2D2_Botao *teclado = C2D2_PegaTeclas();
	// TEsta se envia mensagens
	if (teclado[C2D2_CIMA].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_CIMA;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_CIMA].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_CIMA;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_BAIXO].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BAIXO;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_BAIXO].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BAIXO;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_ESQUERDA].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_ESQ;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_ESQUERDA].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_ESQ;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_DIREITA].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_DIR;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_DIREITA].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_DIR;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_ESPACO].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO1;
		ATOR_EnviaEvento(a, &ev);
	}
	if (teclado[C2D2_LCTRL].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO2;
		ATOR_EnviaEvento(a, &ev);
	}
}

bool CarregaRecursos()
{
	fonte32 = C2D2_CarregaFonte("arial32.png", 32);
	if (fonte32 == 0)
	{
		printf("Falhou carregar a fonte de tamanho 32.\n");
		return false;
	}

	C2D2_TrocaCorLimpezaTela(0, 0, 0);
	C2D2_LimpaTela();
	C2D2_DesenhaTexto(fonte32, 0, 0, "Carregando...", C2D2_TEXTO_ESQUERDA);
	C2D2_Sincroniza(C2D2_FPS_PADRAO);
	
	musicas[0] = CA2_CarregaMusica("monica_fase1.mp3");
	musicas[1] = CA2_CarregaMusica("monica_fase2.mp3");
	musicas[2] = CA2_CarregaMusica("monica_boss.mp3");
	musicas[3] = CA2_CarregaMusica("monica_gameover.mp3");
	musicas[4] = CA2_CarregaMusica("monica_win.mp3");
	

	botao_jogar = C2D2_CarregaSpriteSet("botao_jogar.png", 200, 40);
	if (botao_jogar == 0)
	{
		printf("Falhou carregar o botao de jogar.\n");
		return false;
	}	
	botao_creditos = C2D2_CarregaSpriteSet("botao_creditos.png", 200, 40);
	if (botao_creditos == 0)
	{
		printf("Falhou carregar o botao de creditos.\n");
		return false;
	}
	botao_sair = C2D2_CarregaSpriteSet("botao_sair.png", 200, 40);
	if (botao_sair == 0)
	{
		printf("Falhou carregar o botao desair.\n");
		return false;
	}
	botao_voltar = C2D2_CarregaSpriteSet("botao_voltar.png", 200, 40);
	if (botao_voltar == 0)
	{
		printf("Falhou carregar o botao de voltar.\n");
		return false;
	}
	spmira = C2D2_CarregaSpriteSet("mira.png", 21, 21);
	if (spmira == 0)
	{
		printf("Falhou carregar o sprite da mira.\n");
		return false;
	}
	titulo = C2D2_CarregaSpriteSet("titulo.png", 0, 0);
	if (titulo == 0)
	{
		printf("Falhou carregar a tela de tatulo.\n");
		return false;
	}
	
	gameover = C2D2_CarregaSpriteSet("gameover.png", 0, 0);
	if (gameover == 0)
	{
		printf("Falhou carregar a tela de gameover.\n");
		return false;
	}
	creditos = C2D2_CarregaSpriteSet("creditos.png", 0, 0);
	if (creditos == 0)
	{
		printf("Falhou carregar a tela de craditos.\n");
		return false;
	}
	playerLife = C2D2_CarregaSpriteSet("player_life.png", 96, 32);
	if (playerLife == 0)
	{
		printf("Falhou carregar a life do player.\n");
		return false;
	}

	mapa1 = C2D2M_CarregaMapaMappy("fase01.FMP", "spritesheet.png");
	if (mapa1 == 0)
	{
		printf("Falhou carregar a fase 1.\n");
		return false;
	}
	mapa2 = C2D2M_CarregaMapaMappy("fase02.FMP", "spritesheet.png");
	if (mapa2 == 0)
	{
		printf("Falhou carregar a fase 2.\n");
		return false;
	}

	botao_carregar = C2D2_CarregaSpriteSet("botao_carregar.png", 200, 40);
	if (botao_carregar == 0)
	{
		printf("Falhou carregar o botao carregar.\n");
		return false;
	}
	carregar = C2D2_CarregaSpriteSet("carregar_jogo.png", 0, 0);
	if (carregar == 0)
	{
		printf("Falhou carregar tela carregar.\n");
		return false;
	}
	botao_carrega_fase02 = C2D2_CarregaSpriteSet("carrega_fase02.png", 300, 40);
	if (botao_carrega_fase02 == 0)
	{
		printf("Falhou carregar o botao carrega fase 2.\n");
		return false;
	}

	//bool cplayer = GAME2D_CarregaPlayer();
	bool cplayer = GAME2D_CarregaKnight();
	bool cinimigo1 = GAME2D_CarregaInimigo1();
	bool cinimigo2 = GAME2D_CarregaInimigo2();
	bool cboss1 = GAME2D_CarregaBoss1();
	bool cboss2 = GAME2D_CarregaBoss2();

	if (!cplayer || !cinimigo1 || !cinimigo2 || !cboss1 || !cboss2)
	{
		printf("Falhou ao carregar atores.\n");
		return false;
	}

	return true;
}

void DescarregaRecursos()
{
	// faz a tela de descarregamento
	if (fonte32 != 0)
	{
		C2D2_TrocaCorLimpezaTela(0, 0, 0);
		C2D2_LimpaTela();
		C2D2_DesenhaTexto(fonte32, 0, 0, "Encerrando...", C2D2_TEXTO_ESQUERDA);
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	}
	// Apaga tudo, se for 0 a biblioteca ignora
	C2D2_RemoveFonte(fonte32);
	C2D2_RemoveSpriteSet(botao_jogar);
	C2D2_RemoveSpriteSet(botao_creditos);
	C2D2_RemoveSpriteSet(botao_sair);
	C2D2_RemoveSpriteSet(botao_voltar);
	C2D2_RemoveSpriteSet(spmira);
	C2D2_RemoveSpriteSet(titulo);
	C2D2_RemoveSpriteSet(gameover);
	C2D2_RemoveSpriteSet(creditos);
	C2D2_RemoveSpriteSet(playerLife);
	C2D2_RemoveSpriteSet(botao_carregar);
	C2D2_RemoveSpriteSet(carregar);
	C2D2_RemoveSpriteSet(botao_carrega_fase02);

}

void SetaMusica(int musica, int vezes)
{
	if(musicaAtual == musica)
		return;
	musicaAtual = musica;
	CA2_TocaMusica(musicas[musicaAtual], vezes);
}

void TelaPrincipal()
{
	// Recupera o teclado
	C2D2_Botao *teclas = C2D2_PegaTeclas();
	C2D2_Mouse *mouse = C2D2_PegaMouse();

	// Variaveis para os botaes, 1 se o mouse esta em cima, 0 caso contrario
	int bjogar = 0, bcarregar = 0, bcreditos = 0, bsair = 0;

	// Sincroniza para atualizar o teclado
	C2D2_Sincroniza(C2D2_FPS_PADRAO);
	do
	{
		// Lagica: se pressiona ESC no menu, deve sair fora
		if (teclas[C2D2_ESC].pressionado)
			encerra = true;
		// Va se o mouse esta em cima de um botao e atualiza o estado
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_jogar, 0, BOTAO_JOGAR_X, BOTAO_JOGAR_Y))
			bjogar = 1;
		else
			bjogar = 0;
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_carregar, 0, BOTAO_CARREGAR_X, BOTAO_CARREGAR_Y))
			bcarregar = 1;
		else
			bcarregar = 0;
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_creditos, 0, BOTAO_CREDITOS_X, BOTAO_CREDITOS_Y))
			bcreditos = 1;
		else
			bcreditos = 0;
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_sair, 0, BOTAO_SAIR_X, BOTAO_SAIR_Y))
			bsair = 1;
		else
			bsair = 0;
		// Se o botao do mouse esta pressionado, toma uma aaao
		if (mouse->botoes[C2D2_MESQUERDO].pressionado)
		{
			// O botao ativo a o botao da aaao equivalente
			if (bjogar == 1)
				estadoAtual = TELA_FASE_01;
			else if (bcarregar == 1)
				estadoAtual = TELA_CARREGAR;
			else if (bcreditos == 1)
				estadoAtual = TELA_CREDITOS;
			else if (bsair == 1)
				encerra = true;
		}

		// Limpa a tela e desenha
		C2D2_LimpaTela();
		// Desenha a tela de tatulo
		C2D2_DesenhaSprite(titulo, 0, 0, 0);
		// Desenha os botaes de acordo com o estado
		C2D2_DesenhaSprite(botao_jogar, bjogar, BOTAO_JOGAR_X, BOTAO_JOGAR_Y);
		C2D2_DesenhaSprite(botao_carregar, bcarregar, BOTAO_CARREGAR_X, BOTAO_CARREGAR_Y);
		C2D2_DesenhaSprite(botao_creditos, bcreditos, BOTAO_CREDITOS_X, BOTAO_CREDITOS_Y);
		C2D2_DesenhaSprite(botao_sair, bsair, BOTAO_SAIR_X, BOTAO_SAIR_Y);
		//Desenha a mira
		C2D2_DesenhaSprite(spmira, 0, mouse->x, mouse->y);
		// Faz a troca do front buffer com o back buffer e atualiza as entradas
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	} while (!encerra && estadoAtual == TELA_MENU);
}

void TelaCarregar()
{
	// Recupera o teclado
	C2D2_Botao *teclas = C2D2_PegaTeclas();
	C2D2_Mouse *mouse = C2D2_PegaMouse();

	// Variavel para o botao, 1 se o mouse esta em cima, 0 caso contrario
	int bvoltar = 0, bcarregarFase2 = 0;

	// Sincroniza para atualizar o teclado
	C2D2_Sincroniza(C2D2_FPS_PADRAO);
	// Troca a cor de limpeza para verde
	C2D2_TrocaCorLimpezaTela(0, 0, 0);
	do
	{
		// Lagica: se pressiona ESC no menu, deve sair fora
		if (teclas[C2D2_ESC].pressionado)
			estadoAtual = TELA_MENU;
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_carrega_fase02, 0, BOTAO_CARREGAR_FASE2_X, BOTAO_CARREGAR_FASE2_Y))
			bcarregarFase2 = 1;
		else
			bcarregarFase2 = 0;
		// Va se o mouse esta em cima de um botao e atualiza o estado
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_voltar, 0, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y))
			bvoltar = 1;
		else
			bvoltar = 0;
		// Se o botao do mouse esta pressionado, toma uma aaao
		if (mouse->botoes[C2D2_MESQUERDO].pressionado)
		{
			// O botao ativo a o botao da aaao equivalente
			if (bvoltar == 1)
				estadoAtual = TELA_MENU;
			else if (bcarregarFase2 == 1)
				estadoAtual = TELA_FASE_02;
		}

		// Limpa a tela e desenha
		C2D2_LimpaTela();
		C2D2_DesenhaSprite(carregar, 0, 0, 0);
		// Desenha o botao de acordo com o estado
		C2D2_DesenhaSprite(botao_carrega_fase02, bcarregarFase2, BOTAO_CARREGAR_FASE2_X, BOTAO_CARREGAR_FASE2_Y);
		C2D2_DesenhaSprite(botao_voltar, bvoltar, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y);
		//Desenha a mira
		C2D2_DesenhaSprite(spmira, 0, mouse->x, mouse->y);
		// Faz a troca do front buffer com o back buffer e atualiza as entradas
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	} while (estadoAtual == TELA_CARREGAR);
}

void TelaCreditos()
{
	// Recupera o teclado
	C2D2_Botao *teclas = C2D2_PegaTeclas();
	C2D2_Mouse *mouse = C2D2_PegaMouse();

	// Variavel para o botao, 1 se o mouse esta em cima, 0 caso contrario
	int bvoltar = 0;

	// Sincroniza para atualizar o teclado
	C2D2_Sincroniza(C2D2_FPS_PADRAO);
	// Troca a cor de limpeza para verde
	C2D2_TrocaCorLimpezaTela(0, 0, 0);
	do
	{
		// Lagica: se pressiona ESC no menu, deve sair fora
		if (teclas[C2D2_ESC].pressionado)
			estadoAtual = TELA_MENU;
		// Va se o mouse esta em cima de um botao e atualiza o estado
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_voltar, 0, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y))
			bvoltar = 1;
		else
			bvoltar = 0;
		// Se o botao do mouse esta pressionado, toma uma aaao
		if (mouse->botoes[C2D2_MESQUERDO].pressionado)
		{
			// O botao ativo a o botao da aaao equivalente
			if (bvoltar == 1)
				estadoAtual = TELA_MENU;
		}

		// Limpa a tela e desenha
		C2D2_LimpaTela();
		C2D2_DesenhaSprite(creditos, 0, 0, 0);
		// Desenha o botao de acordo com o estado
		C2D2_DesenhaSprite(botao_voltar, bvoltar, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y);
		//Desenha a mira
		C2D2_DesenhaSprite(spmira, 0, mouse->x, mouse->y);
		// Faz a troca do front buffer com o back buffer e atualiza as entradas
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	} while (estadoAtual == TELA_CREDITOS);
}

void DesenhaVidas()
{
	C2D2_DesenhaSprite(playerLife, player->vidas - 1, 0, 0);
}

void DesenhaPontos()
{
	std::string s = std::to_string(player->aux_int[0]);
	char const *pchar = s.c_str();

	C2D2_DesenhaTexto(fonte32, 700, 0, "PONTOS:", C2D2_TEXTO_ESQUERDA);
	C2D2_DesenhaTexto(fonte32, 1024, 0, pchar, C2D2_TEXTO_DIREITA);
}

void PreparaFase(unsigned int mapa, int camadas, int camadaMarcas)
{
	C2D2M_VelocidadeCamadaMapa(mapa, camadas - 1, 1);
	for (int i = 0, vel = camadas - 1; i < camadas - 1; i++, vel--)
	{
		C2D2M_VelocidadeCamadaMapa(mapa, i, vel);
	}
	C2D2M_CamadaMarcas(mapa, camadaMarcas, TILE_MARCAS);
	C2D2M_GravidadeMapa(mapa, GRAVIDADE, MAXGRAVIDADE);

	// cria o player
	int x = 60, y = 60;
	C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_INICIO, &x, &y);
	player = ATOR_CriaAtor(PLAYER, x, y, 0);

	//criando inimigos 1
	numInimigos = 0;
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO1, &x, &y))
	{
		numInimigos++;
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			numInimigos++;
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO2, &x, &y))
	{
		numInimigos++;
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			numInimigos++;
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_BOSS1, &x, &y))
	{
		numInimigos++;
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			numInimigos++;
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_BOSS2, &x, &y))
	{
		numInimigos++;
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			numInimigos++;
	}

	inimigos = (Ator**)malloc(sizeof(Ator*)*numInimigos);
	memset(inimigos, 0, numInimigos*sizeof(Ator*));
	numInimigos = 0;
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO1, &x, &y))
	{
		inimigos[numInimigos++] = ATOR_CriaAtor(INIMIGO1, x, y, 180);
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			inimigos[numInimigos++] = ATOR_CriaAtor(INIMIGO1, x, y, 180);
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO2, &x, &y))
	{
		inimigos[numInimigos++] = ATOR_CriaAtor(INIMIGO2, x, y, 180);
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			inimigos[numInimigos++] = ATOR_CriaAtor(INIMIGO2, x, y, 180);
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_BOSS1, &x, &y))
	{
		inimigos[numInimigos++] = ATOR_CriaAtor(BOSS1, x, y, 180);
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			inimigos[numInimigos++] = ATOR_CriaAtor(BOSS1, x, y, 180);
	}
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_BOSS2, &x, &y))
	{
		inimigos[numInimigos++] = ATOR_CriaAtor(BOSS2, x, y, 180);
		while (C2D2M_ProximoBlocoMarca(mapa, &x, &y))
			inimigos[numInimigos++] = ATOR_CriaAtor(BOSS2, x, y, 180);
	}
}

void ProcessaColisao(int mapa)
{
	// Testa as colisoes com marcadores do mapa
	if (ATOR_ColidiuBlocoCenario(player, mapa, C2D2M_MORTE))
	{
		ev.tipoEvento = EVT_COLIDIU_ARMADILHA;
		ATOR_EnviaEvento(player, &ev);
	}
	if (ATOR_ColidiuBlocoCenario(player, mapa, C2D2M_CHECK))
	{
		ev.tipoEvento = EVT_CHECKPOINT;
		ATOR_EnviaEvento(player, &ev);
		SetaMusica(MUSICA_BOSS,-1);
	}
	if (ATOR_ColidiuBlocoCenario(player, mapa, C2D2M_FIM))
	{
		ev.tipoEvento = EVT_FIM_FASE;
		ATOR_EnviaEvento(player, &ev);
	}
	if (ATOR_ColidiuBlocoCenario(player, mapa, MARCA_MOEDA))
	{
		ev.tipoEvento = EVT_MOEDA;
		ATOR_EnviaEvento(player, &ev);
	}
	if (ATOR_ColidiuBlocoCenario(player, mapa, MARCA_VIDA))
	{
		ev.tipoEvento = EVT_VIDA;
		ATOR_EnviaEvento(player, &ev);
	}
	if (ATOR_ColidiuBlocoCenario(player, mapa, C2D2M_ESCADAV))
	{
		ev.tipoEvento = EVT_ESCADA;
		//player->aux = 1;
		ATOR_EnviaEvento(player, &ev);
	}
	/*else
	{
		player->aux = 0;
	}*/
	if (ATOR_ColidiuBlocoCenario(player, mapa, MARCA_MOLA))
	{
		ev.tipoEvento = EVT_MOLA;
		ATOR_EnviaEvento(player, &ev);
	}


	for (int i = 0; i < numInimigos; i++)
	{
		if (inimigos[i] != 0)
		{
			if (ATOR_ColidiuAtores(player, inimigos[i]))
			{
				if (player->estado.estado == KNIGHT_ATACANDO)
				{
					ev.tipoEvento = EVT_DANO;
					ev.valor = 10;
					ATOR_EnviaEvento(inimigos[i], &ev);
				}
			}
		}
	}

	ATOR_AplicaEstado(player, mapa, LARGURA_TELA, ALTURA_TELA);

	for (int i = 0; i < numInimigos; i++)
	{
		if (inimigos[i] != 0)
		{
			ATOR_AplicaEstado(inimigos[i], mapa, LARGURA_TELA, ALTURA_TELA);
		}
	}

}

void AtualizaInimigos(int mapa)
{
	for (int i = 0; i < numInimigos; i++)
	{
		if (inimigos[i] != 0)
		{
			if (!ATOR_Atualiza(inimigos[i], mapa))
			{
				free(inimigos[i]);
				inimigos[i] = 0;
			}
		}
	}
}

void DesenhaInimigos(int mapa)
{
	for (int i = 0; i < numInimigos; i++)
	{
		if (inimigos[i] != 0)
		{
			ATOR_Desenha(inimigos[i], mapa, 0, 0);
		}
	}
}

void ProcessaEventos()
{
	while (ATOR_ProximoEventoJogo(&ev))
	{
		switch (ev.tipoEvento)
		{
		case EVT_FIM_FASE:
			if (ev.subtipo == SUBEVT_FIM_FASE_VITORIA)
			{
				if (faseAtual == FASE_01)
				{
					faseAtual = FASE_02;
					estadoAtual = TELA_FASE_02;
				}
				else if (faseAtual == FASE_02)
				{
					SetaMusica(MUSICA_WIN, 1);
					estadoAtual = TELA_CREDITOS;
				}
			}
			else if (ev.subtipo == SUBEVT_FIM_FASE_DERROTA)
			{
				estadoAtual = TELA_GAMEOVER;
			}
			break;
		}
	}
}

void LiberarAtores()
{
	free(player);
	for (int i = 0; i < numInimigos; i++)
	{
		if (inimigos[i] != 0)
		{
			free(inimigos[i]);
		}
	}
	free(inimigos);
}

void Fase01()
{
	faseAtual = FASE_01;
	PreparaFase(mapa1, 5, 5);
	SetaMusica(MUSICA_F1, -1);

	C2D2_TrocaCorLimpezaTela(121, 188, 255);
	C2D2_Botao *teclado = C2D2_PegaTeclas();
	do
	{
		if (teclado[C2D2_ESC].pressionado)
		{
			estadoAtual = TELA_MENU;
		}

		C2D2_LimpaTela();

		ProcessaColisao(mapa1);
		
		ProcessaControle(player);

		ATOR_Atualiza(player, mapa1);

		AtualizaInimigos(mapa1);

		ATOR_CentraMapa(player, mapa1, LARGURA_TELA, ALTURA_TELA);

		ProcessaEventos();

		C2D2M_DesenhaCamadaMapa(mapa1, 0, 0, 0, LARGURA_TELA, ALTURA_TELA);
		C2D2M_DesenhaCamadaMapa(mapa1, 1, 0, 0, LARGURA_TELA, ALTURA_TELA);
		C2D2M_DesenhaCamadaMapa(mapa1, 2, 0, 0, LARGURA_TELA, ALTURA_TELA);
		C2D2M_DesenhaCamadaMapa(mapa1, 3, 0, 0, LARGURA_TELA, ALTURA_TELA);

		ATOR_Desenha(player, mapa1, 0, 0);

		DesenhaInimigos(mapa1);

		C2D2M_DesenhaCamadaMapa(mapa1, 4, 0, 0, LARGURA_TELA, ALTURA_TELA);

		DesenhaVidas();
		DesenhaPontos();

		C2D2_Sincroniza(C2D2_FPS_PADRAO);
		if (teclado[C2D2_ENTER].ativo)
		{
			SDL_Delay(500);
		}
	} while (estadoAtual == TELA_FASE_01);

	LiberarAtores();
}

void Fase02()
{
	faseAtual = FASE_02;
	PreparaFase(mapa2, 3, 3);
	SetaMusica(MUSICA_F2, -1);

	C2D2_TrocaCorLimpezaTela(0, 0, 0);
	C2D2_Botao *teclado = C2D2_PegaTeclas();
	do
	{
		if (teclado[C2D2_ESC].pressionado)
		{
			estadoAtual = TELA_MENU;
		}

		C2D2_LimpaTela();

		ProcessaColisao(mapa2);

		ProcessaControle(player);

		ATOR_Atualiza(player, mapa2);

		AtualizaInimigos(mapa2);

		ATOR_CentraMapa(player, mapa2, LARGURA_TELA, ALTURA_TELA);

		ProcessaEventos();

		C2D2M_DesenhaCamadaMapa(mapa2, 0, 0, 0, LARGURA_TELA, ALTURA_TELA);
		C2D2M_DesenhaCamadaMapa(mapa2, 1, 0, 0, LARGURA_TELA, ALTURA_TELA);

		ATOR_Desenha(player, mapa2, 0, 0);

		DesenhaInimigos(mapa2);

		C2D2M_DesenhaCamadaMapa(mapa2, 2, 0, 0, LARGURA_TELA, ALTURA_TELA);

		DesenhaVidas();
		DesenhaPontos();

		C2D2_Sincroniza(C2D2_FPS_PADRAO);
		if (teclado[C2D2_ENTER].ativo)
		{
			SDL_Delay(500);
		}
	} while (estadoAtual == TELA_FASE_02);

	LiberarAtores();
}

void TelaGameover()
{
	SetaMusica(MUSICA_GAMEOVER, 1);
	// Recupera o teclado
	C2D2_Botao *teclas = C2D2_PegaTeclas();
	C2D2_Mouse *mouse = C2D2_PegaMouse();

	// Variavel para o botao, 1 se o mouse esta em cima, 0 caso contrario
	int bvoltar = 0;

	// Sincroniza para atualizar o teclado
	C2D2_Sincroniza(C2D2_FPS_PADRAO);
	// Troca a cor de limpeza para verde
	C2D2_TrocaCorLimpezaTela(128, 255, 128);
	do
	{
		// Logica: se pressiona ESC no menu, deve sair fora
		if (teclas[C2D2_ESC].pressionado)
			estadoAtual = TELA_MENU;
		// Ve se o mouse esta em cima de um botao e atualiza o estado
		if (C2D2_ColidiuSprites(spmira, 0, mouse->x, mouse->y, botao_voltar, 0, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y))
			bvoltar = 1;
		else
			bvoltar = 0;
		// Se o botao do mouse esta pressionado, toma uma aaao
		if (mouse->botoes[C2D2_MESQUERDO].pressionado)
		{
			// O botao ativo a o botao da aaao equivalente
			if (bvoltar == 1)
				estadoAtual = TELA_MENU;
		}

		// Limpa a tela e desenha
		C2D2_LimpaTela();
		C2D2_DesenhaSprite(gameover, 0, 0, 0);
		// Desenha o botao de acordo com o estado
		C2D2_DesenhaSprite(botao_voltar, bvoltar, BOTAO_RETORNAR_X, BOTAO_RETORNAR_Y);
		//Desenha a mira
		C2D2_DesenhaSprite(spmira, 0, mouse->x, mouse->y);
		// Faz a troca do front buffer com o back buffer e atualiza as entradas
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	} while (estadoAtual == TELA_GAMEOVER);
}

int main(int narg, char **valarg)
{
	// Inicia a Chien2D 2 e testa se deu tudo certo
	if (C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "The Knight"))
	{
		printf("Iniciou a Chien2D 2 com sucesso\n");
	}
	else
	{
		printf("Falhou ao iniciar a Chien2D 2. Encerrando o programa.\n");
		return 0;
	}
	C2D2P_Inicia();
	C2D2M_Inicia();
	CA2_Inicia();
	ATOR_Inicia();

	if (!CarregaRecursos())
	{
		return 0;
	}

	do
	{
		switch (estadoAtual)
		{
		case TELA_MENU:
			TelaPrincipal();
			break;
		case TELA_FASE_01:
			Fase01();
			break;
		case TELA_CARREGAR:
			TelaCarregar();
			break;
		case TELA_FASE_02:
			Fase02();
			break;
		case TELA_CREDITOS:
			TelaCreditos();
			break;
		case TELA_GAMEOVER:
			TelaGameover();
			break;
		default:
			encerra = true;
			break;
		}
	} while (!encerra);

	DescarregaRecursos();
	ATOR_DescarregaAtor(PLAYER);
	ATOR_Encerra();
	C2D2M_Encerra();
	CA2_Encerra();
	C2D2_Encerra();
	return 0;
}
