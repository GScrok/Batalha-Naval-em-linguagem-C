#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define TAMANHO 10
#define NUM_NAVIOS 5

// struct que define a jogada no tabuleiro
typedef struct
{
    int x;
    int y;
} Ponto;

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
            // senão ele imprimirá como O para tiro na água e X para acerto
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

// de forma aleatória, substitui as casas do tabuleiro com '~' para 'N'
void posicionarNavios(char tabuleiro[TAMANHO][TAMANHO])
{
    int naviosPosicionados = 0;

    while (naviosPosicionados < NUM_NAVIOS)
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

// essa função basicamente compara se as coordenadas batem com algum navio e retorna True ou False
bool acertou(char tabuleiro[TAMANHO][TAMANHO], Ponto tiro)
{
    return tabuleiro[tiro.x][tiro.y] == 'N';
}

// se houve acerto marcará como um X, se foi um tiro na água, marcará como O
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

// esta função serve para retornar true apenas quando a jogada atual não foi realizada anteriormente
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

// faz a jogada aleatória do computador
Ponto jogadaComputador(Ponto jogadasComputador[], int *jogadasFeitas)
{
    Ponto jogada;
    // apenas atribui as posições quando o while atender a condição
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

void exibirMenu(char *nome)
{
    printf("Bem-vindo ao Jogo de Batalha Naval!\n");

    fflush(stdin);
    printf("Digite seu nome: ");
    scanf("%s", nome);

    // Remove newline character if present
    size_t len = strlen(nome);
    if (len > 0 && nome[len - 1] == '\n')
    {
        nome[len - 1] = '\0';
    }

    printf("1. Iniciar Jogo\n");
    printf("Escolha uma opção: ");

    int opcao;
    scanf("%d", &opcao);
    getchar(); // to consume the newline character left by scanf

    if (opcao != 1)
    {
        printf("Opção inválida. Encerrando o programa.\n");
        exit(1);
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    char nome[50];
    exibirMenu(nome);

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
    // srand define a seed para a função rand() gerar números baseados nela.
    // enquanto time(NULL) retorna o tempo em timestamp
    srand(time(NULL));
    // posiciona os navios de forma aleatória para cada jogador
    posicionarNavios(tabuleiroJogador);
    // necessário para alterar a seed anterior e mudar os aleatórios
    srand(time(NULL) + 1);
    posicionarNavios(tabuleiroComputador);

    while (acertosJogador < NUM_NAVIOS && acertosComputador < NUM_NAVIOS)
    {
        printf("Seu tabuleiro:\n");
        // imprime os tabuleiros de ambos os jogadores, um oculto e outro não (o 0 e 1 representam isso)
        imprimirTabuleiro(tabuleiroJogador, 0);

        printf("\nTabuleiro do computador:\n");
        imprimirTabuleiro(tabuleiroComputador, 1);

        printf("\n%s, sua vez de jogar. Digite o seu tiro (linha e coluna): ", nome);
        scanf("%d %d", &tiro.x, &tiro.y);

        // caso o usuário digite um número fora do tabuleiro
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

        // nesta função ele marca no tabuleiro se o jogador/computador acertou ou errou
        marcarAcertoOuErro(tabuleiroComputador, tiro, resultadoTiroJogador);

        if (acertosJogador == NUM_NAVIOS)
        {
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

        if (acertosComputador == NUM_NAVIOS)
        {
            break;
        }

        Sleep(1000);
        printf("\n");
    }

    if (acertosJogador == NUM_NAVIOS)
    {
        printf("%s, voce afundou todos os navios do computador!\n", nome);
        system("Ayrton Senna - Tema Da Vitória (Super Mario World Version).mp3");
    }
    else
    {
        printf("O computador afundou todos os seus navios, %s!\n", nome);
        system("SUPER MARIO - game over - sound effect.mp3");
    }

    return 0;
}
