#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <process.h>
#include <conio.h>

#define TAMANHO 10
int numNavios;
bool atingiuTempoLimite;
bool acabouJogo = false;

char nome[50];

// struct que define a jogada no tabuleiro
typedef struct
{
    int x;
    int y;
} Ponto;

//struct que define os parametros do jogo
typedef struct
{
    char nome[50];
    int modo;
    int dificuldade;
    int tempoLimite;
    char vencedor[50];
    int pontuacao;
} ParametrosJogo;

// inicializa o tabuleiro com todas as casas como '~'
void inicializarTabuleiro(char tabuleiro[TAMANHO][TAMANHO])
{
    for (int i = 0; i < TAMANHO; i++)
    {
        for (int j = 0; j < TAMANHO; j++)
        {
            tabuleiro[i][j] = '~';
        }
    }
}

void imprimirTabuleiro(char tabuleiro[TAMANHO][TAMANHO], int oculto)
{
    printf("  ");
    for (int i = 0; i < TAMANHO; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < TAMANHO; j++)
        {
            // se oculto for como um e for um Navio, ele ira imprimir como '~'
            // senão ele imprimira como O para tiro na agua e X para acerto
            if (oculto && tabuleiro[i][j] == 'N')
            {
                printf("~ ");
            }
            else
            {
                printf("%c ", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }
    Sleep(1000);
}

// de forma aleatoria, substitui as casas do tabuleiro com '~' para 'N'
void posicionarNavios(char tabuleiro[TAMANHO][TAMANHO])
{
    int naviosPosicionados = 0;

    while (naviosPosicionados < numNavios)
    {
        int x = rand() % TAMANHO;
        int y = rand() % TAMANHO;
        if (tabuleiro[x][y] == '~')
        {
            tabuleiro[x][y] = 'N';
            naviosPosicionados++;
        }
    }
}

// Permite que o usuario posicione os navios manualmente
void posicionarNaviosUsuario(char tabuleiro[TAMANHO][TAMANHO])
{
    int naviosPosicionados = 0;
    Ponto navio;

    while (naviosPosicionados < numNavios)
    {
        imprimirTabuleiro(tabuleiro, 0);
        printf("Digite as coordenadas do navio %d (linha e coluna): ", naviosPosicionados + 1);
        scanf("%d %d", &navio.x, &navio.y);

        if (navio.x >= 0 && navio.x < TAMANHO && navio.y >= 0 && navio.y < TAMANHO && tabuleiro[navio.x][navio.y] == '~')
        {
            tabuleiro[navio.x][navio.y] = 'N';
            naviosPosicionados++;
        }
        else
        {
            printf("Coordenadas invalidas ou posicao ja ocupada. Tente novamente.\n");
        }
    }
}

// essa funcao basicamente compara se as coordenadas batem com algum navio e retorna True ou False
bool acertou(char tabuleiro[TAMANHO][TAMANHO], Ponto tiro)
{
    return tabuleiro[tiro.x][tiro.y] == 'N';
}

// se houve acerto marca com um X, se foi um tiro na agua, marca como O
void marcarAcertoOuErro(char tabuleiro[TAMANHO][TAMANHO], Ponto tiro, int acerto)
{
    if (acerto)
    {
        tabuleiro[tiro.x][tiro.y] = 'X';
    }
    else
    {
        tabuleiro[tiro.x][tiro.y] = 'O';
    }
}

// esta funcao serve para retornar true apenas quando a jogada atual nao foi realizada anteriormente
bool jogadaComputadorValida(Ponto jogadasComputador[], int jogadasFeitas, Ponto jogada)
{
    // verifica todas as jogadas e se, coincidir com alguma, retorna um false imediatamente
    for (int i = 0; i < jogadasFeitas; i++)
    {
        if (jogadasComputador[i].x == jogada.x && jogadasComputador[i].y == jogada.y)
        {
            return false;
        }
    }
    return true;
}

// faz a jogada aleatoria do computador
Ponto jogadaComputador(Ponto jogadasComputador[], int *jogadasFeitas)
{
    Ponto jogada;
    // apenas atribui as posicoes quando o while atender a condicao
    do
    {
        jogada.x = rand() % TAMANHO;
        jogada.y = rand() % TAMANHO;
    } while (!jogadaComputadorValida(jogadasComputador, *jogadasFeitas, jogada));

    jogadasComputador[*jogadasFeitas] = jogada;
    (*jogadasFeitas)++;
    return jogada;
}

void escritaJogadaComputador()
{
    printf("Computador esta fazendo a jogada");
    Sleep(1000);
    printf(" .");
    Sleep(1000);
    printf(" .");
    Sleep(1000);
    printf(" .\n\n");
}

void exibeHistorico(ParametrosJogo *parametrosRegistros[], int contador){
	for(int i=0; i < contador; i++){
		printf("\nRODADA: %d\n", i+1);
		printf("Vencedor: %s\n", parametrosRegistros[i]->vencedor);
		printf("Pontuacao: %d\n", parametrosRegistros[i]->pontuacao);
		printf("--------------------\n");
	}
}

//menu de exibicao para coletar o nome do 
void exibirMenu(ParametrosJogo *parametros, ParametrosJogo *parametrosRegistros[], int contador)
{   
    if (!contador){
        printf("Bem-vindo ao Jogo de Batalha Naval!\n");

        fflush(stdin);
        printf("Digite seu nome: ");
        //nome e uma variavel global pois nao vai mudar  jogador no meio do jogo
        scanf("%s", &nome);
    }
	 
	strcpy(parametros->nome, nome);
    
    //so sai do while quando selecionar ou sair, ou uma das dificuldades do jogo
    bool continuar = true;
    while (continuar)
    {
		printf("\nSelecione a opcao desejada:\n");
    
	    printf("1. Dificuldade facil - 10 navios e sem limite de tempo.\n");
	    printf("2. Dificuldade media - 8 navios e limite de 10 minutos.\n");
	    printf("3. Dificuldade dificil - 4 navios e limite de 5 minutos.\n");
	    printf("4. Visualizar historico.\n");
	    printf("5. Sair.\n");
	    
        printf("Escolha uma opcao: ");
        scanf("%d", &(parametros->dificuldade));

        switch (parametros->dificuldade)
        {
        case 1:
            numNavios = 10;
            continuar = false;
            break;
        case 2:
            numNavios = 8;
            parametros->tempoLimite = 600;
            continuar = false;
            break;
        case 3:
            numNavios = 4;
            parametros->tempoLimite = 300;
            continuar = false;
            break;
        case 4:
			exibeHistorico(parametrosRegistros, contador);
			break;
        case 5:
            exit(1);
        default:
            printf("Opcao invalida...\n");
        }
    }

    printf("\n1. Iniciar Jogo com posicionamento automatico\n");
    printf("2. Iniciar Jogo com posicionamento manual\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);

    if (opcao == 1 || opcao == 2)
    {
        parametros->modo = opcao;
    }
    else
    {
        printf("Opcao invalida. Encerrando o programa.\n");
        exit(1);
    }
}

//funcao para validar se atingiuTempoLimite esta como true e dessa forma encerrar o jogo
bool jogoAcabou(){
	if(atingiuTempoLimite){
		printf("\nTempo limite atingido. O jogo sera encerrado.\n");
		return true;
    }
    return false;
}

//apenas faz referencia para a funcao thread
void tempoLimiteEncerramento(void *);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
	
    ParametrosJogo *parametrosRegistros [1000];
	
	char nome[50];
    int contador=0;
    
    while (true){

		ParametrosJogo *parametros = (ParametrosJogo *)malloc(sizeof(ParametrosJogo));

	    exibirMenu(parametros, parametrosRegistros, contador);
	
	    char tabuleiroJogador[TAMANHO][TAMANHO];
	    char tabuleiroComputador[TAMANHO][TAMANHO];
	    Ponto tiro;
	    // esse array de struct serve para registrar todas as jogadas do computador,
	    // para que ele não repita a mesma jogada
	    Ponto jogadasComputador[TAMANHO * TAMANHO];
	    int acertosJogador = 0;
	    int acertosComputador = 0;
	    int jogadasFeitasComputador = 0;
	
	    inicializarTabuleiro(tabuleiroJogador);
	    inicializarTabuleiro(tabuleiroComputador);
	    // srand define a seed para a funcao rand() gerar numeros baseados nela.
	    // enquanto time(NULL) retorna o tempo em timestamp
	    
	    // Posiciona os navios de acordo com a escolha do usuario
	    if (parametros->modo == 1)
	    {
			srand(time(NULL));
	        posicionarNavios(tabuleiroJogador);
	    }
	    else
	    {
	        posicionarNaviosUsuario(tabuleiroJogador);
	    }
	
	    // necessario para alterar a seed anterior e mudar os aleatorios
	    srand(time(NULL) + 1);
	    posicionarNavios(tabuleiroComputador);
		
		if(parametros->dificuldade != 1){
			_beginthread(tempoLimiteEncerramento, 0, (void*)&(parametros->tempoLimite));
		}
		
	
	    while (acertosJogador < numNavios && acertosComputador < numNavios)
	    {
			if (jogoAcabou()){
				break;
			}
	        
	        printf("\nSeu tabuleiro:\n");
	        // imprime os tabuleiros de ambos os jogadores, um oculto e outro nao (o 0 e 1 representam isso)
	        imprimirTabuleiro(tabuleiroJogador, 0);
	
	        printf("\nTabuleiro do computador:\n");
	        imprimirTabuleiro(tabuleiroComputador, 1);
			
	        printf("\n%s, sua vez de jogar. Digite o seu tiro (linha e coluna): ", parametros->nome);
	        scanf("%d %d", &tiro.x, &tiro.y);
	
			if (jogoAcabou()){
				break;
			}
	        
	        // caso o usuario digite um numero fora do tabuleiro
	        if (tiro.x < 0 || tiro.x >= TAMANHO || tiro.y < 0 || tiro.y >= TAMANHO)
	        {
	            printf("Coordenadas invalidas. Tente novamente.\n");
	            continue;
	        }
	
	        bool resultadoTiroJogador = acertou(tabuleiroComputador, tiro);
	
	        if (resultadoTiroJogador)
	        {
	            printf("Voce acertou um navio!\n");
	            acertosJogador++;
	        }
	        else
	        {
	            printf("Voce atirou na agua!\n");
	        }
	
	        // nesta funcao ele marca no tabuleiro se o jogador/computador acertou ou errou
	        marcarAcertoOuErro(tabuleiroComputador, tiro, resultadoTiroJogador);
	
	        if (acertosJogador == numNavios)
	        {
				acabouJogo = true;
	            break;
	        }
	
	        Sleep(300);
	        // apenas uma escrita para dar mais naturalidade ao computador
	        escritaJogadaComputador();
	
	        // gera a jogada do computador
	        Ponto tiroComputador = jogadaComputador(jogadasComputador, &jogadasFeitasComputador);
	        printf("Computador jogou (%d, %d)!\n", tiroComputador.x, tiroComputador.y);
	        Sleep(500);
	
	        bool resultadoTiroComputador = acertou(tabuleiroJogador, tiroComputador);
	
	        if (resultadoTiroComputador)
	        {
	            printf("Computador acertou um navio!\n");
	            acertosComputador++;
	        }
	        else
	        {
	            printf("Computador atirou na agua!\n");
	        }
	
	        marcarAcertoOuErro(tabuleiroJogador, tiroComputador, resultadoTiroComputador);
	
	        if (acertosComputador == numNavios)
	        {
				acabouJogo = true;
	            break;
	        }
	
	        Sleep(1000);
	        printf("\n");
	    }
		
	    if (acertosJogador > acertosComputador)
	    {
	        printf("%s, voce afundou mais os navios do computador! %d - Navio(s)", parametros->nome, acertosJogador);
	        strcpy(parametros->vencedor, parametros->nome);
	        parametros->pontuacao = acertosJogador;
	        
			system("Winner.mp3");
	    }
	    else if (acertosJogador < acertosComputador)
	    {
	        printf("O computador afundou mais os seus navios, %s! %d - Navio(s)", parametros->nome, acertosComputador);
	        strcpy(parametros->vencedor, "Computador");
	        parametros->pontuacao = acertosComputador;
			
			printf("\nO tabuleiro do seu adversário era:\n");
	        imprimirTabuleiro(tabuleiroComputador, 0);
			
			system("GameOver.mp3");
	    }
	    else {
			printf("Voces tiveram um empate, %s! %d - Navio(s)\n", parametros->nome, acertosJogador);
			strcpy(parametros->vencedor, "EMPATE");
			parametros->pontuacao = acertosJogador;
	        
			printf("\nO tabuleiro do seu adversário era:\n");
	        imprimirTabuleiro(tabuleiroComputador, 0);
			
			system("GameOver.mp3");
	    }
	    
		printf("\nPressione qualquer tecla para continuar!\n");
	    getch();
	    //insere no vetor de structs na posicao do contador o struct de parametros atual
	    parametrosRegistros[contador] = parametros;
	    
	    //limpa a struct alocada dinamicamente
		free(parametros);
		parametros = NULL;

		//incrementa o contador e reseta as variaveis de controle do jogo
		contador++;
		atingiuTempoLimite = false;
		acabouJogo = false;
	}
	
    return 0;
}
//thread que faz a contagem do tempo e a define globalmente para parar o jogo
void tempoLimiteEncerramento(void *tempoLimite) {
    int segundos = 0;
    int limite = *((int*)tempoLimite);
    while ((segundos < limite) && !(acabouJogo)){
		Sleep(1000);
		segundos++;
    }
    if(!acabouJogo)
    	atingiuTempoLimite = true;
}
