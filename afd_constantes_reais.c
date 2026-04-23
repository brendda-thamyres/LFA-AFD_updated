/*
 * AFD - Reconhecedor de Constantes Numericas Reais
 * Solucao 2: estado corrente global e transicoes por switch
 *
 * Padrao aceito:
 *   numero -> digitos
 *           | - digitos
 *           | digitos , digitos
 *           | - digitos , digitos
 *
 * Exemplos validos:   3,14   -3,14   42   -42   0,5   100,00
 * Exemplos parciais:  3.14 reconhece 3, 3, reconhece 3
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

// macros para indicar os estados do afd
#define MAX_LEXEMA 256
#define ESTADO_ERRO        -1
#define ESTADO_INICIAL      0
#define ESTADO_SINAL        1
#define ESTADO_INTEIRO      2
#define ESTADO_VIRGULA      3
#define ESTADO_FRACIONARIO  4
#define ESTADO_FINAL        5

/*
 * Estado corrente do AFD explicitamente acessivel de forma global,
 * conforme a descricao da solucao.
 */

int estado_corrente = ESTADO_INICIAL;

int adicionar_caractere(char *lexema, int *tamanho, char c) {
    if (*tamanho >= MAX_LEXEMA - 1) {
        estado_corrente = ESTADO_ERRO;
        return 0;
    }

    lexema[*tamanho] = c;
    (*tamanho)++;
    lexema[*tamanho] = '\0';
    return 1;
}

/*
 * Tenta reconhecer um token a partir da posicao *pos na cadeia entrada.
 * Toda a logica do AFD fica no laco abaixo; o switch controla as
 * transicoes e cada estado trata suas situacoes de erro.
 */
int reconhecer(const char *entrada, int *pos, char *lexema) {
    int i = *pos;
    int tamanho = 0;

    lexema[0] = '\0';
    estado_corrente = ESTADO_INICIAL;

    while (estado_corrente != ESTADO_FINAL && estado_corrente != ESTADO_ERRO) {
        char c = entrada[i];

        switch (estado_corrente) {
            case ESTADO_INICIAL:
                if (c == '-') {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                    estado_corrente = ESTADO_SINAL;
                } else if (isdigit((unsigned char)c)) {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                    estado_corrente = ESTADO_INTEIRO;
                } else {
                    estado_corrente = ESTADO_ERRO;
                }
                break;

            case ESTADO_SINAL:
                if (isdigit((unsigned char)c)) {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                    estado_corrente = ESTADO_INTEIRO;
                } else {
                    estado_corrente = ESTADO_ERRO;
                }
                break;

            case ESTADO_INTEIRO:
                if (isdigit((unsigned char)c)) {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                } else if (c == ',') {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                    estado_corrente = ESTADO_VIRGULA;
                } else {
                    estado_corrente = ESTADO_FINAL;
                }
                break;

            case ESTADO_VIRGULA:
                if (isdigit((unsigned char)c)) {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                    estado_corrente = ESTADO_FRACIONARIO;
                } else {
                    /*
                     * Virgula sem digito depois: nao aceita, pois o estado virgula nao é sinal,
                     logo, ou ele recebe outro digito pra ir para o estado final ou dá erro
                     */

                    estado_corrente = ESTADO_ERRO;
                    
                    // tamanho--;
                    // lexema[tamanho] = '\0';
                    // i--;
                    // estado_corrente = ESTADO_FINAL;
                    
                }
                break;

            case ESTADO_FRACIONARIO:
                if (isdigit((unsigned char)c)) {
                    if (!adicionar_caractere(lexema, &tamanho, c)) break;
                    i++;
                } else {
                    estado_corrente = ESTADO_FINAL;
                }
                break;

            default:
                estado_corrente = ESTADO_ERRO;
                break;
        }
    }

    //depois que o lexema acaba, reconhece o token se tiver chegado no estado final, att a posição
    if (estado_corrente == ESTADO_FINAL && tamanho > 0) {
        *pos = i;
        return 1;
    }

    //retorna estado de erro se nao é o estado final
    return 0;
}

int main(void) {

    //variaveis de entrada
    char entrada[MAX_LEXEMA];
    char lexema[MAX_LEXEMA];
    //int encontrou = 0;
    int opcao;

    printf("AFD - Reconhecedor de Constantes Numericas Reais\n");
    printf("--------------------------------------------------\n");
    do {
        printf("Digite a entrada: ");
    
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            fprintf(stderr, "Erro ao ler entrada.\n");
            return 1;
        }
    
        //limpando a string
        int n = (int)strlen(entrada);
        if (n > 0 && entrada[n - 1] == '\n') {
            entrada[--n] = '\0';
        }
    
        printf("Entrada: \"%s\"\n\n", entrada);
    
        int pos = 0;
        /*percorre toda a entrada*/
        if (reconhecer(entrada, &pos, lexema)) {
            printf("Token reconhecido: %s\n", lexema);
        } else {
            printf("Nenhum token reconhecido.\n");
        }

        //adicionei uma opção de fazer outro teste so pra nao precisar executar toda vez
        printf("\n=== Digite 1 para inserir outra entrada ou outro numero para sair do programa:  ");
        scanf("%d", &opcao);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (opcao == 1);

    Sleep(5000);
    return 0;
}       
