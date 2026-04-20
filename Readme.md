# AFD — Reconhecedor de Constantes Numéricas Reais

Implementação de um Autômato Finito Determinístico para reconhecimento de constantes numéricas reais, usando a Solução TIPO 2 (tabela de transições). O programa lê uma linha de entrada, varre os caracteres e devolve o primeiro token numérico que casar com o padrão.

---

## Compilação

```bash
gcc -Wall -o afd afd_constantes_reais.c
```

No Windows com MinGW:

```bash
gcc -Wall -o afd.exe afd_constantes_reais.c
```

---

## Como executar

```bash
# Linux / macOS
./afd

# Windows
.\afd.exe
```

O programa vai pedir uma linha de entrada. Digite qualquer coisa — uma expressão, uma atribuição, um número solto — e ele vai encontrar (ou não) o primeiro token numérico válido.

---

## O que o AFD reconhece

O separador decimal aceito é a **vírgula**, não o ponto. O padrão é:

```
número  →  dígitos
         | dígitos , dígitos
```

Algumas regras importantes: precisa ter pelo menos um dígito antes da vírgula, e pelo menos um dígito depois. Uma vírgula sozinha ou no início não conta. Se encontrar um ponto no meio de um número, o AFD para e aceita só a parte inteira que veio antes.

---

## Tabela de transições

| Estado | Dígito | Vírgula | Outro |
|--------|--------|---------|-------|
| q0     | q1     | erro    | erro  |
| q1 *  | q1     | q2      | erro  |
| q2 *  | q2     | erro    | erro  |

Os estados marcados com * são estados de aceitação. Qualquer transição marcada como erro faz o AFD parar e retornar ao último ponto aceito.

---

## Exemplos de testes

### Entradas que geram token reconhecido

| Entrada            | Token reconhecido |
|--------------------|-------------------|
| `var := 3,14`      | `3,14`            |
| `x := 42`          | `42`              |
| `resultado := 0,5` | `0,5`             |
| `total := 100,00`  | `100,00`          |
| `lendo 7,5 agora`  | `7,5`             |
| `3,14`             | `3,14`            |
| `999`              | `999`             |

### Entradas que geram aceitação parcial

Nesses casos o programa reconhece alguma coisa, mas não o número completo. Vale entender o motivo.

| Entrada        | Token reconhecido | O que acontece                                              |
|----------------|-------------------|-------------------------------------------------------------|
| `var := 3.14`  | `3`               | O ponto não é separador válido. O AFD aceita o `3` e para. |
| `a := 10.5`    | `10`              | Mesmo caso.                                                 |
| `3,`           | `3`               | Vírgula sem dígitos depois. Aceita só a parte inteira.      |

### Entradas que não geram nenhum token

| Entrada      | O que acontece                                  |
|--------------|-------------------------------------------------|
| `var := abc` | Nenhum dígito encontrado na entrada.            |
| `var :=`     | Sem valor nenhum após a atribuição.             |
| `,14`        | Vírgula antes de qualquer dígito é inválida.    |
| `xyz`        | Só letras, nada a reconhecer.                   |

---

## Saída esperada

Para uma entrada aceita:

```
AFD - Reconhecedor de Constantes Numéricas Reais
--------------------------------------------------
Digite a entrada: var := 3,14
Entrada: "var := 3,14"

Token reconhecido: 3,14
```

Para uma entrada sem token válido:

```
AFD - Reconhecedor de Constantes Numéricas Reais
--------------------------------------------------
Digite a entrada: var := abc
Entrada: "var := abc"

Nenhum token reconhecido.
```

---