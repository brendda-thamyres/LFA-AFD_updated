/*
 * AFD - Reconhecedor de Constantes Numéricas Reais
 * Solução TIPO 2: Tabela de Transições
 * Padrão aceito: inteiros e reais com vírgula decimal
 *   Exemplos válidos:  3,14   42   0,5   100,00
 *   Exemplos inválidos: 3.14 (aceita apenas '3'), .14, 3,
 *
 * Estados:
 *   0 - inicial
 *   1 - lendo dígitos inteiros (estado de aceitação parcial)
 *   2 - lendo dígitos após vírgula (estado de aceitação)
 *  -1 - estado de erro/rejeição
 *
 * Categorias de caractere (colunas da tabela):
 *   0 - dígito (0-9)
 *   1 - vírgula (',')
 *   2 - outro (qualquer outro caractere)
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEMA 256
#define ERRO       -1
#define NUM_ESTADOS 3
#define NUM_COLUNAS 3

/* Tabela de transições: tabela[estado][categoria] = proximo_estado */
int tabela[NUM_ESTADOS][NUM_COLUNAS] = {
    /*         digito  virgula  outro */
    /* q0 */  {  1,     ERRO,   ERRO  },
    /* q1 */  {  1,     2,      ERRO  },
    /* q2 */  {  2,     ERRO,   ERRO  }
};

/* Estados de aceitação */
int aceita[NUM_ESTADOS] = {
    0,  /* q0: NÃO aceita */
    1,  /* q1: aceita (inteiro) */
    1   /* q2: aceita (real com vírgula) */
};

/* Retorna a categoria do caractere para indexar a tabela */
int categoria(char c) {
    if (isdigit((unsigned char)c)) return 0;
    if (c == ',')                   return 1;
    return 2;
}

/*
 * Tenta reconhecer um token a partir da posição *pos na cadeia entrada.
 * Avança *pos até onde o AFD conseguir avançar.
 * Retorna 1 se encontrou token, 0 caso contrário.
 * Preenche lexema com o token reconhecido.
 */
int reconhecer(const char *entrada, int *pos, char *lexema) {
    int estado = 0;
    int ultimo_aceito = -1;
    int ultimo_aceito_pos = *pos;
    int i = *pos;
    int len = (int)strlen(entrada);

    while (i < len) {
        int col = categoria(entrada[i]);
        int prox = tabela[estado][col];

        if (prox == ERRO) break;

        estado = prox;
        i++;

        if (aceita[estado]) {
            ultimo_aceito = estado;
            ultimo_aceito_pos = i;
        }
    }

    if (ultimo_aceito != -1) {
        int tamanho = ultimo_aceito_pos - *pos;
        strncpy(lexema, entrada + *pos, tamanho);
        lexema[tamanho] = '\0';
        *pos = ultimo_aceito_pos;
        return 1;
    }

    return 0;
}

int main(void) {
    char entrada[MAX_LEXEMA];
    char lexema[MAX_LEXEMA];
    int encontrou = 0;

    printf("AFD - Reconhecedor de Constantes Numéricas Reais\n");
    printf("--------------------------------------------------\n");
    printf("Digite a entrada: ");

    if (!fgets(entrada, sizeof(entrada), stdin)) {
        fprintf(stderr, "Erro ao ler entrada.\n");
        return 1;
    }

    /* Remove newline final */
    int n = (int)strlen(entrada);
    if (n > 0 && entrada[n - 1] == '\n') entrada[--n] = '\0';

    printf("Entrada: \"%s\"\n\n", entrada);

    int pos = 0;
    while (pos < n) {
        /* Pula caracteres não-numéricos procurando início de token */
        if (!isdigit((unsigned char)entrada[pos])) {
            pos++;
            continue;
        }

        int pos_antes = pos;
        if (reconhecer(entrada, &pos, lexema)) {
            printf("Token reconhecido: %s\n", lexema);
            encontrou = 1;
            break; /* Aceita apenas o PRIMEIRO token encontrado */
        } else {
            pos = pos_antes + 1; /* Avança para não travar */
        }
    }

    if (!encontrou) {
        printf("Nenhum token reconhecido.\n");
    }

    return 0;
}
