# AFD - Reconhecedor de Constantes Numericas Reais

Implementacao de um Automato Finito Deterministico para reconhecimento de constantes numericas reais.

A solucao atual usa:

- uma variavel global `estado_corrente`;
- transicoes controladas por `switch`;
- leitura de uma linha completa com `fgets`;
- varredura da entrada ate encontrar o primeiro token numerico valido;
- pausa de 5 segundos antes de fechar o programa, usando `Sleep(5000)`.

---

## Compilacao

No Windows com MinGW:

```bash
gcc -Wall -o afd.exe afd_constantes_reais.c
```

Como o programa usa `Sleep`, ele inclui a biblioteca:

```c
#include <windows.h>
```

---

## Como executar

```bash
.\afd.exe
```

O programa pede uma linha de entrada e tenta encontrar o primeiro token numerico real valido dentro dela.

---

## Padrao reconhecido

O separador decimal aceito e a virgula. O ponto nao e tratado como separador decimal.

O AFD reconhece:

```text
numero -> digitos
        | - digitos
        | digitos , digitos
        | - digitos , digitos
```

Exemplos validos:

```text
3,14
-3,14
42
-42
0,5
100,00
```

---

## Observacoes importantes

O programa reconhece apenas o primeiro token valido encontrado.

Por exemplo, na entrada:

```text
212131easd123,54
```

o primeiro token reconhecido e:

```text
212131
```

Isso acontece porque, depois dos digitos iniciais, o caractere `e` encerra aquele token. O programa imprime o primeiro token encontrado e para a busca.

Se uma virgula aparecer sem digito depois, o AFD volta para o ultimo ponto aceito e reconhece apenas a parte inteira.

Exemplo:

```text
3,
```

gera:

```text
Token reconhecido: 3
```

---

## Estados do AFD

| Estado no codigo | Significado |
|------------------|-------------|
| `ESTADO_INICIAL` | Inicio da tentativa de reconhecimento |
| `ESTADO_SINAL` | Leu o sinal negativo `-` |
| `ESTADO_INTEIRO` | Leu um ou mais digitos da parte inteira |
| `ESTADO_VIRGULA` | Leu a virgula decimal |
| `ESTADO_FRACIONARIO` | Leu um ou mais digitos da parte fracionaria |
| `ESTADO_FINAL` | Token aceito |
| `ESTADO_ERRO` | Token invalido |

---

## Tabela de transicoes

| Estado | Digito | `-` | `,` | Outro |
|--------|--------|-----|-----|-------|
| `ESTADO_INICIAL` | `ESTADO_INTEIRO` | `ESTADO_SINAL` | erro | erro |
| `ESTADO_SINAL` | `ESTADO_INTEIRO` | erro | erro | erro |
| `ESTADO_INTEIRO` | `ESTADO_INTEIRO` | final | `ESTADO_VIRGULA` | final |
| `ESTADO_VIRGULA` | `ESTADO_FRACIONARIO` | final parcial | final parcial | final parcial |
| `ESTADO_FRACIONARIO` | `ESTADO_FRACIONARIO` | final | final | final |

`final parcial` significa que a virgula e descartada e o AFD aceita somente a parte inteira ja reconhecida.

---

## Exemplos de testes

### Entradas com token reconhecido

| Entrada | Token reconhecido |
|---------|-------------------|
| `var := 3,14` | `3,14` |
| `x := 42` | `42` |
| `resultado := 0,5` | `0,5` |
| `total := 100,00` | `100,00` |
| `valor := -3,14` | `-3,14` |
| `abc -42 fim` | `-42` |
| `212131easd123,54` | `212131` |

### Entradas com reconhecimento parcial

| Entrada | Token reconhecido | Motivo |
|---------|-------------------|--------|
| `var := 3.14` | `3` | O ponto nao e separador decimal valido. |
| `a := 10.5` | `10` | O AFD para antes do ponto. |
| `3,` | `3` | A virgula nao tem digito depois. |
| `-7,abc` | `-7` | A virgula nao forma parte fracionaria valida. |

### Entradas sem token reconhecido

| Entrada | Resultado |
|---------|-----------|
| `var := abc` | Nenhum token reconhecido. |
| `var :=` | Nenhum token reconhecido. |
| `,14` | Nenhum token reconhecido. |
| `-abc` | Nenhum token reconhecido. |
| `xyz` | Nenhum token reconhecido. |

---

## Saida esperada

Para uma entrada aceita:

```text
AFD - Reconhecedor de Constantes Numericas Reais
--------------------------------------------------
Digite a entrada: var := 3,14
Entrada: "var := 3,14"

Token reconhecido: 3,14
```

Para uma entrada com primeiro token inteiro antes de letras:

```text
AFD - Reconhecedor de Constantes Numericas Reais
--------------------------------------------------
Digite a entrada: 212131easd123,54
Entrada: "212131easd123,54"

Token reconhecido: 212131
```

Para uma entrada sem token valido:

```text
AFD - Reconhecedor de Constantes Numericas Reais
--------------------------------------------------
Digite a entrada: var := abc
Entrada: "var := abc"

Nenhum token reconhecido.
```

Depois de mostrar o resultado, o programa aguarda 5 segundos antes de encerrar.
