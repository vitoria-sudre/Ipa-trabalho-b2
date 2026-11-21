# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos (LPA)  
**Semestre:** 2026/2  
**Trabalho:** Trabalho do 2º Bimestre — Sistema de Gestão de Estacionamento  
**Linguagem:** C (Padrão C11)  
**Modalidade:** Dupla

---

## 1. Integrantes e Identificação

**Integrante 1:** Vitoria Sudre de Melo  
**GitHub:** @vitória-sudre

**Responsabilidades:** Estrutura de dados, menu principal, cadastro de veículos, validações alfanuméricas e matriz do mapa de vagas.

**Integrante 2:** Natasha Novaes dos Santos  
**GitHub:**natashanovaes55-codificadora

**Responsabilidades:** Vetor dinâmico, realocação de memória (`realloc`), movimentação, remoção de registros e relatório de ocupação.

---

## 2. Descrição do Cenário e Escopo

O **Sistema de Gestão de Estacionamento** é uma aplicação de terminal desenvolvida em linguagem C11 para gerenciar 20 vagas físicas de estacionamento, organizadas em uma matriz de 4 fileiras por 5 colunas.

O sistema permite cadastrar, consultar, atualizar, movimentar, remover e listar veículos, além de apresentar o mapa de vagas e um relatório de ocupação.

### Premissas e Regras de Negócio

1. Os registros permanecem na memória durante a execução do programa (não há gravação em arquivos).
2. Cada veículo possui um identificador único de 7 caracteres alfanuméricos (A–Z e 0–9).
3. A descrição do veículo deve conter entre 1 e 60 caracteres (não pode ser só espaços).
4. As categorias são: 1 para carro e 2 para moto.
5. Os identificadores são convertidos para letras maiúsculas; `abc1d23` e `ABC1D23` representam o mesmo veículo.
6. Não é permitido cadastrar identificadores duplicados.
7. O número de veículos cadastrados deve corresponder ao número de vagas ocupadas na matriz.
8. O vetor dinâmico começa com capacidade para 2 veículos e dobra de tamanho utilizando `realloc` quando necessário.
9. O sistema permite no máximo 20 veículos cadastrados.
10. Não é permitido cadastrar dois veículos na mesma vaga.
11. Ao remover um veículo, a vaga correspondente é liberada.
12. Operações inválidas são recusadas sem modificar os dados do estacionamento.
13. O identificador não pode ser alterado depois do cadastro.

---

## 3. Estruturas de Dados Utilizadas

O projeto utiliza uma struct para armazenar os dados dos veículos, uma matriz para representar as vagas e um vetor dinâmico para armazenar os registros.

### 3.1. Estrutura Veiculo

```c
typedef struct {
    char id[TAM_ID + 1];            /* 7 caracteres + '\0' */
    char descricao[MAX_DESC + 1];   /* até 60 caracteres + '\0' */
    int categoria;                  /* 1 = carro, 2 = moto */
    int fileira;                    /* 1 a 4 */
    int coluna;                     /* 1 a 5 */
} Veiculo;
```

Fileira e coluna são guardadas de 1 a 4 e de 1 a 5 (os mesmos valores que o usuário enxerga). A conversão para índice da matriz (valor − 1) é feita sempre no momento de acessar o mapa.

### 3.2. Matriz do Estacionamento

```c
int mapa[FILEIRAS][COLUNAS];   /* 4 x 5 = 20 vagas */
```

- `0` (`VAGA_LIVRE`): vaga livre.
- `1` (`VAGA_OCUPADA`): vaga ocupada.

A matriz é percorrida com laços aninhados (inicialização, contagem de ocupadas e exibição do mapa).

### 3.3. Vetor Dinâmico (struct `Colecao`)

```c
typedef struct {
    Veiculo *dados;     /* vetor alocado dinamicamente */
    int quantidade;     /* registros em uso */
    int capacidade;     /* espaço reservado */
} Colecao;
```

O vetor é criado com `malloc` (capacidade inicial 2). Quando `quantidade` atinge `capacidade`, a função `colecao_ampliar` usa `realloc` para dobrar o espaço, guardando o resultado em um ponteiro temporário: se falhar, o vetor original continua válido e o cadastro é cancelado sem alterar nada. Ao encerrar, `colecao_liberar` executa `free`.

| Etapa | Capacidade |
|---|---:|
| Inicial | 2 |
| Primeira expansão | 4 |
| Segunda expansão | 8 |
| Terceira expansão | 16 |
| Quarta expansão | 32 |

Mesmo que a capacidade interna chegue a 32, o estacionamento continua limitado a 20 veículos, devido à quantidade de vagas físicas. A capacidade do vetor **não** é a quantidade de vagas.

---

## 4. Funcionalidades e Menu

```text
===== ESTACIONAMENTO =====
1 - Cadastrar veiculo
2 - Consultar veiculo por identificador
3 - Atualizar descricao ou categoria
4 - Mover veiculo para outra vaga
5 - Remover veiculo
6 - Listar veiculos cadastrados
7 - Exibir mapa de vagas
8 - Exibir relatorio da ocupacao
0 - Encerrar
Escolha uma opcao:
```

- **Cadastrar veículo:** registra um novo veículo em uma vaga livre, validando todos os campos antes de alterar qualquer dado.
- **Consultar veículo:** localiza um veículo pelo identificador e mostra seus dados completos.
- **Atualizar veículo:** altera a descrição e/ou a categoria (Enter mantém o valor atual), preservando identificador e vaga.
- **Mover veículo:** transfere o veículo para outra vaga livre; se o destino for a própria vaga atual, informa sem alterar nada.
- **Remover veículo:** libera a vaga e remove o registro do vetor sem deixar lacunas.
- **Listar veículos:** mostra todos os veículos ativos (ou mensagem específica se não houver nenhum).
- **Exibir mapa:** mostra as 20 vagas, com `X` para ocupada e `.` para livre, e legenda.
- **Relatório de ocupação:** total de vagas, ocupadas, livres, percentual (duas casas decimais), carros, motos, além da quantidade de registros e da capacidade do vetor dinâmico.

---

## 5. Organização dos Arquivos e Modularização

```text
lpa-trabalho-b2/
├── src/
│   ├── estacionamento.c
│   ├── estacionamento.h
│   └── main.c
├── teste/
│   └── analises.md
├── .gitignore
├── compilar.bat
├── LICENSE
└── README.md
```

### Responsabilidade dos arquivos

- **main.c:** função principal, menu interativo e funções de interface (`ui_cadastrar`, `ui_consultar`, `ui_atualizar`, `ui_mover`, `ui_remover`) que leem os dados do usuário e chamam as funções de lógica.
- **estacionamento.h:** constantes, estruturas (`Veiculo`, `Colecao`, `Relatorio`), enums de resultado e protótipos.
- **estacionamento.c:** implementação das validações, da coleção dinâmica, do mapa, do relatório e das operações.
- **teste/analises.md:** resumo dos testes manuais realizados (os casos detalhados estão na seção 7 deste README).
- **compilar.bat:** compila e executa o programa no Windows com GCC.
- **.gitignore:** impede o envio de executáveis e arquivos de IDE.
- **LICENSE:** licença MIT do projeto.

### Principais funções

**Leitura segura de entrada**
- `ler_linha()`: lê uma linha com `fgets`, descarta o excesso digitado e informa o status (ok, vazia, longa, EOF). Nunca usa `gets`.
- `ler_inteiro()`: lê um inteiro com `strtol`, rejeitando texto que não seja número.

**Coleção dinâmica (ponteiro para `Colecao`)**
- `colecao_iniciar()`: `malloc` com capacidade inicial 2.
- `colecao_ampliar()`: `realloc` com ponteiro temporário, dobrando a capacidade.
- `colecao_liberar()`: `free` do vetor no encerramento.
- `colecao_buscar()` e `indice_por_id()`: busca por identificador.
- `colecao_remover_indice()`: remove deslocando os elementos seguintes.
- `colecao_listar()` e `veiculo_exibir()`: saída dos registros.

**Validação**
- `normalizar_id()`: valida 7 caracteres A–Z/0–9 e converte para maiúsculas.
- `descricao_valida()`, `categoria_valida()`, `posicao_valida()`.

**Mapa de vagas (matriz)**
- `mapa_inicializar()`, `mapa_contar_ocupadas()`, `mapa_exibir()`.

**Operações (retornam um código `Resultado`)**
- `cadastrar_veiculo()`, `atualizar_veiculo()`, `mover_veiculo()`, `remover_veiculo()`.
- `mensagem_resultado()`: converte o código de resultado em mensagem.

**Relatório e consistência**
- `calcular_relatorio()` e `exibir_relatorio()`.
- `verificar_consistencia()`: confere que registros ativos = células ocupadas e que cada registro aponta para uma vaga exclusiva.

Todas as operações validam **antes** de alterar a matriz ou o vetor; em qualquer recusa, os dados ficam exatamente como estavam. O estado do programa (matriz e coleção) é declarado em `main` e passado por parâmetro, sem variáveis globais.

---

## 6. Instruções de Compilação e Execução

É necessário ter o compilador GCC instalado e configurado no sistema (disponível no PATH).

### 6.1. Windows — compilação e execução automática (recomendado)

```bash
.\compilar.bat
```

### 6.2. Windows — compilação e execução manual

Na pasta principal do projeto:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento.exe
```

Para executar:

```bash
.\estacionamento.exe
```

### 6.3. Linux ou macOS

Para compilar:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
```

Para executar:

```bash
./estacionamento
```

**Observação:** o `compilar.bat` automatiza a compilação e a execução apenas no Windows. Em Linux e macOS, os comandos devem ser executados manualmente.

---

## 7. Casos de Teste Documentados

Os testes foram feitos manualmente no terminal. Cada caso foi executado de forma independente, podendo usar cadastros e posições diferentes entre si; as saídas abaixo foram copiadas do terminal.

| Teste | Entrada ou operação | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|
| T01 | Iniciar o programa e usar as opções 6, 7 e 8 | Listagem vazia, 20 vagas livres e 0,00% | Mensagem de nenhum veículo, mapa todo livre e relatório com 0,00% | Aprovado |
| T02 | Cadastrar veículos com dados válidos | Registrar e apresentar os dados | CALAKS7 e CALAKS8 cadastrados com sucesso | Aprovado |
| T03 | Cadastrar identificador já existente (em minúsculas) | Recusar o cadastro duplicado | Sistema informou que já existia veículo com esse identificador | Aprovado |
| T04 | Cadastrar em vaga ocupada e em posição inválida | Recusar a operação | Mensagens de vaga ocupada e de posição inválida | Aprovado |
| T05 | Cadastrar o terceiro veículo | Capacidade do vetor passar de 2 para 4 e registros anteriores preservados | Relatório mostrou 3 registros e capacidade 4 | Aprovado |
| T06 | Atualizar descrição e categoria | Apresentar os novos dados | BBB0002 atualizado para descrição "carro" e categoria Moto | Aprovado |
| T07 | Mover veículo para vaga livre | Atualizar a posição | BBB0002 movido para F4/C1 | Aprovado |
| T08 | Remover veículo cadastrado | Excluir o registro e liberar a vaga | BBB0002 removido e ausente da listagem seguinte | Aprovado |
| T09 | Tentar cadastrar o 21º veículo | Recusar por lotação | Sistema informou que o estacionamento estava lotado | Aprovado |
| T10 | Encerrar o programa (opção 0) | Finalizar e liberar a memória | Mensagem de encerramento e memória liberada | Aprovado |

### 7.1. T01 — Estacionamento Vazio

Logo após iniciar o programa:

**Opção 6 (listar):**

```text
Nenhum veiculo cadastrado.
```

**Opção 7 (mapa):**

```text
   C1  C2  C3  C4  C5  
F1 .   .   .   .   .   
F2 .   .   .   .   .   
F3 .   .   .   .   .   
F4 .   .   .   .   .   
Legenda: X = ocupada; . = livre
```

**Opção 8 (relatório):**

```text
Total de vagas.....: 20
Vagas ocupadas.....: 0
Vagas livres.......: 20
Percentual ocupacao: 0.00%
Carros.............: 0
Motos..............: 0
Registros no vetor.: 0
Capacidade do vetor: 2
```

**Resultado:** mapa totalmente livre, listagem vazia e ocupação de 0,00%.

### 7.2. T02 — Cadastro de Veículos Válidos

**Primeiro cadastro:**

```text
Identificador: CALAKS7
Descricao: preto
Categoria: 2
Fileira: 1
Coluna: 2

Veiculo CALAKS7 cadastrado na vaga F1/C2.
```

**Segundo cadastro:**

```text
Identificador: CALAKS8
Descricao: verde
Categoria: 1
Fileira: 1
Coluna: 1

Veiculo CALAKS8 cadastrado na vaga F1/C1.
```

Depois dos dois cadastros, o relatório apresentou:

```text
Total de vagas.....: 20
Vagas ocupadas.....: 2
Vagas livres.......: 18
Percentual ocupacao: 10.00%
Carros.............: 1
Motos..............: 1
```

**Resultado:** os dois veículos foram cadastrados e o relatório apresentou duas vagas ocupadas.

### 7.3. T03 — Identificador Duplicado (sem distinção de maiúsculas e minúsculas)

Tentativa de cadastrar novamente o identificador `bbb0002`, que já existia como `BBB0002`:

```text
Identificador: bbb0002
Descricao: branco
Categoria: 1

Cadastro cancelado. Ja existe um veiculo com esse identificador.
```

**Resultado:** o sistema reconheceu o duplicado mesmo com letras minúsculas e recusou o cadastro, sem alterar o mapa nem a contagem.

### 7.4. T04 — Vaga Ocupada e Posição Inválida

**Vaga ocupada:**

```text
Identificador: bbb0002
Fileira: 1
Coluna: 5

Cadastro cancelado. Vaga ocupada: operacao recusada.
```

**Posição fora dos limites:**

```text
Identificador: aaa9990
Fileira: 0
Coluna: 6

Cadastro cancelado. Posicao invalida: fileira de 1 a 4 e coluna de 1 a 5.
```

**Resultado:** as operações inválidas foram recusadas antes de qualquer acesso à matriz, sem alteração parcial.

### 7.5. T05 — Crescimento do Vetor Dinâmico

O vetor começa com capacidade 2 (como mostra o relatório do T01). Foram cadastrados dois veículos (CALAKS7 e CALAKS8) e, em seguida, um terceiro:

```text
Identificador: CALAKS9
Descricao: azul
Categoria: 1
Fileira: 1
Coluna: 3

Veiculo CALAKS9 cadastrado na vaga F1/C3.
```

Relatório após o terceiro cadastro:

```text
Total de vagas.....: 20
Vagas ocupadas.....: 3
Vagas livres.......: 17
Percentual ocupacao: 15.00%
Carros.............: 2
Motos..............: 1
Registros no vetor.: 3
Capacidade do vetor: 4
```

A função `colecao_ampliar` usa `realloc` com ponteiro temporário, seguindo a sequência `2 -> 4 -> 8 -> 16 -> 32`.

**Resultado:** a capacidade passou de 2 para 4, e os veículos anteriores foram preservados.

### 7.6. T06 — Atualização de Veículo

```text
Identificador: bbb0002
Nova descricao: carro
Nova categoria: 2

Veiculo atualizado:
Identificador: BBB0002
Descricao....: carro
Categoria....: Moto
Posicao......: Fileira 2, Coluna 4
```

**Resultado:** descrição e categoria foram alteradas, mantendo identificador e posição, sem duplicar o registro.

### 7.7. T07 — Movimentação de Veículo

```text
Identificador: bbb0002
Nova fileira: 4
Nova coluna: 1

Veiculo movido para a vaga F4/C1.
```

**Resultado:** o sistema confirmou a movimentação; a vaga de origem foi liberada e a de destino ocupada.

### 7.8. T08 — Remoção de Veículo

```text
Identificador: bbb0002

Veiculo removido e vaga liberada.
```

Na listagem seguinte, o veículo não apareceu:

```text
ID        Categ. Vaga     Descricao
CALAKS8   Carro  F1/C5    verde
CALAKS7   Carro  F2/C2    azul
AAA0001   Moto   F1/C1    verde
AAA0006   Moto   F4/C4    preto
Total: 4 veiculo(s).
```

**Resultado:** o veículo removido deixou de aparecer na listagem e a vaga foi liberada.

### 7.9. T09 — Limite Máximo de 20 Veículos

Com as 20 vagas preenchidas, tentou-se um novo cadastro:

```text
Identificador: FGH8654
Descricao: branco
Categoria: 2
Fileira: 1
Coluna: 3

Cadastro cancelado. Estacionamento lotado: as 20 vagas estao ocupadas.
```

Relatório com o estacionamento lotado:

```text
Total de vagas.....: 20
Vagas ocupadas.....: 20
Vagas livres.......: 0
Percentual ocupacao: 100.00%
Carros.............: 9
Motos..............: 11
```

**Resultado:** o cadastro adicional foi recusado e o limite de 20 veículos foi mantido.

### 7.10. T10 — Encerramento do Programa

Ao selecionar a opção `0`:

```text
Encerrando. Memoria liberada.
```

**Resultado:** o programa liberou o vetor dinâmico com `free` e encerrou normalmente.

---

## 8. Validações e Testes Complementares

| Verificação | Exemplo utilizado | Resultado observado |
|---|---|---|
| Identificador com espaço no final | `ABC1234 ` | Entrada recusada por formato inválido |
| Identificador duplicado | `bbb0002` | Cadastro recusado |
| Identificador em letras minúsculas | `ccc0976` | Cadastrado como `CCC0976` |
| Categoria não informada | Campo vazio | Cadastro cancelado por categoria inválida |
| Fileira fora do limite | 0 ou 6 | Entrada recusada |
| Coluna fora do limite | 6 | Entrada recusada |
| Vaga ocupada | F1/C5 | Operação recusada |
| Opção inválida no menu | Texto em vez de número | Sistema informa opção inexistente |
| Cadastro com estacionamento lotado | 21º veículo | Cadastro recusado |
| Listagem de veículos | Opção 6 | Registros e posições apresentados |
| Exibição do mapa | Opção 7 | `X` para ocupada e `.` para livre |
| Relatório de ocupação | Opção 8 | Quantidades e percentual apresentados |

### 8.1. Identificador Inválido

```text
Identificador: ABC1234 

Cadastro cancelado. Identificador invalido: use exatamente 7 caracteres (A-Z e 0-9).
```

### 8.2. Categoria Inválida

```text
Categoria (1 = carro, 2 = moto):

Cadastro cancelado. Categoria invalida: use 1 (carro) ou 2 (moto).
```

### 8.3. Normalização do Identificador

```text
Identificador: ccc0976

Veiculo CCC0976 cadastrado na vaga F2/C1.
```

---

## 9. Consistência dos Dados

O sistema mantém a correspondência entre os registros do vetor dinâmico e as posições ocupadas na matriz:

**Quantidade de registros = Quantidade de vagas ocupadas**

- Ao cadastrar, um registro é adicionado e uma vaga é marcada como ocupada.
- Ao mover, a posição anterior é liberada e a nova é ocupada.
- Ao remover, o registro é excluído (sem lacunas) e a vaga é liberada.
- O relatório conta as vagas ocupadas direto na matriz e os carros/motos direto no vetor, refletindo o estado atual.
- A função `verificar_consistencia()` confere essa igualdade e se cada registro aponta para uma vaga ocupada exclusiva.

---

## 10. Distribuição das Responsabilidades e Integração das Branches

**Distribuição:** Vitoria ficou com a estrutura de dados, o menu, o cadastro, as validações e a matriz do mapa; Natasha ficou com o vetor dinâmico e `realloc`, a movimentação, a remoção e o relatório de ocupação.

**Integração das branches:**

- Branch de Vitoria: `[PREENCHER: nome da branch]` — [PREENCHER: o que foi desenvolvido nela].
- Branch de Natasha: `[PREENCHER: nome da branch]` — [PREENCHER: o que foi desenvolvido nela].
- As branches foram integradas à `main` por `[PREENCHER: merge ou pull request]` antes da entrega.
- [PREENCHER: informar se houve conflito de merge e como foi resolvido; se não houve, escrever "Não houve conflitos de merge".]

---

## 11. Limitações Conhecidas

- Os dados existem apenas durante a execução; ao encerrar, tudo é perdido (persistência não faz parte do escopo).
- A listagem não é ordenada; a ordem muda após remoções, pois os registros seguintes são deslocados.
- A capacidade do vetor não diminui após remoções (apenas cresce) e chega a no máximo 32, embora o limite físico seja de 20 veículos.
- A descrição pode ter espaços internos, mas não pode ser vazia nem conter apenas espaços.
- Na atualização, texto não numérico no campo de categoria é tratado como categoria inválida (a atualização é cancelada).
- Os testes foram feitos manualmente, sem testes automatizados.

---

## 12. Declaração Individual de Autoria e Uso de IA

### Declaração de Vitoria Sudre de Melo

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando na estrutura de dados, no menu principal, no cadastro de veículos, nas validações e na matriz do mapa de vagas. Utilizamos o ChatGPT como ferramenta de inteligência artificial para obter uma base inicial e orientações para a estruturação do projeto. As sugestões foram analisadas, adaptadas e revisadas durante o desenvolvimento.

### Declaração de Natasha Novaes dos Santos

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando no vetor dinâmico, na realocação de memória com `realloc`, na movimentação, na remoção de veículos e nos relatórios de ocupação. Também utilizamos o ChatGPT como apoio para construir a base inicial do código, esclarecer dúvidas e auxiliar na organização das funcionalidades. O código foi revisado e estudado por nós durante a realização do trabalho.
