# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos 
**Semestre:** 2026/2  
**Linguagem:** C (Padrão ISO C11) 
**Modalidade:** Dupla  

---

## 1. Identificação e Integrantes [3]

* **Integrante 1:** Vitória Sudré de Melo - Perfil GitHub: @vitoriasudre  
  *Responsabilidade:* Estrutura de dados inicial, menu principal, cadastro de veículos, validações alfanuméricas de identificador e matriz do mapa de vagas.
* **Integrante 2:** Natasha Novaes dos Santos - Perfil GitHub: @natashanovaes  
  *Responsabilidade:* Alocação e expansão dinâmica de memória (realloc), movimentação, remoção de registros sem lacunas e relatório de ocupação.

---

## 2. Descrição do Cenário e Escopo [3]

O **Sistema de Gestão de Estacionamento** é uma aplicação interativa de terminal desenvolvida em linguagem C (padrão C11) para controlar o fluxo e a ocupação de **20 vagas físicas de estacionamento** [5], arranjadas em uma matriz de **4 fileiras (F1 a F4) por 5 colunas (C1 a C5)** [5].

### Premissas e Regras de Negócio:
1. **Permanência em Memória:** Todos os registros de veículos e o estado das vagas permanecem em memória RAM durante a execução [5].
2. **Critério de Consistência:** Em qualquer momento, o total de registros no vetor dinâmico de veículos é estritamente igual à quantidade de valores `1` na matriz de vagas e à contagem no relatório de ocupação [6].
3. **Gerenciamento Dinâmico:** A coleção de veículos inicia com capacidade para 2 registros e é expandida dinamicamente (dobrando a capacidade via `realloc`) sempre que necessário [7].
4. **Capacidade Física Máxima:** O sistema bloqueia novos cadastros ao atingir o limite físico de 20 vagas ocupadas [7].

---

## 3. Estruturas de Dados Utilizadas [3]

```c
/* Struct que representa o registro individual de um veículo */
typedef struct {
    char identificador[8]; /* 7 caracteres alfanuméricos + '\0' */
    char descricao[9];    /* Descrição com até 60 caracteres */
    int categoria;         /* 1 = Carro, 2 = Moto */
    int fileira;           /* Posição física: 1 a 4 */
    int coluna;            /* Posição física: 1 a 5 */
} Veiculo;

/* Struct principal para controle do estacionamento */
typedef struct {
    int mapa[10][11];       /* Matriz 4x5: 0 = livre, 1 = ocupada */
    Veiculo *veiculos;    /* Vetor dinâmico de veículos */
    size_t quantidade;    /* Quantidade de veículos ativos */
    size_t capacidade;    /* Capacidade alocada no vetor */
} Estacionamento;

Funcionalidades e Menu

=========================================
   SISTEMA DE GESTÃO DE ESTACIONAMENTO   
=========================================
1 - Cadastrar veículo
2 - Consultar veículo por identificador
3 - Atualizar descrição ou categoria
4 - Mover veículo para outra vaga
5 - Remover veículo
6 - Listar veículos cadastrados
7 - Exibir mapa de vagas
8 - Exibir relatório da ocupação
0 - Encerrar
=========================================

5. Modularização do Código
O projeto está organizado na pasta src/ em três arquivos bem definidos:
• src/estacionamento.h: Definições de constantes, structs e protótipos de funções.
• src/estacionamento.c: Implementação da lógica das 8 operações do CRUD, validação de IDs e realloc.
• src/main.c: Ponto de entrada da aplicação (main), menu interativo e tratamento de buffers.
6. Instruções de Compilação e Execução
Compilação (GCC no Padrão ISO C11):
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
Execução no Terminal:
./estacionamento

7. Casos de Teste Documentados
Teste Descrição da
Operação Resultado Esperado Resultado Observado Situação
T01 Estacionamento Vazio Mapa exibindo 20 vagas livres (.) e
relatório zerado Exibido conforme esperado Aprovado
T02 Cadastro Válido Cadastrar ABC1D23 (Carro) na vaga
F1/C1 Registro salvo e vaga F1/C1 ocupada (X) Aprovado
T03 Duplicidade
Case-Insensitive
Tentar cadastrar abc1d23 após
ABC1D23 Cadastro rejeitado por ID duplicado Aprovado
T04 Vaga Ocupada Cadastrar em vaga F1/C1 já contendo
veículo
Operação recusada com aviso de vaga
ocupada Aprovado
T05 Crescimento Dinâmico Inserir 3º veículo (LPA2026) no sistema Vetor expande capacidade de 2 para 4
via realloc Aprovado
T06 Atualização Alterar descrição de ABC1D23 para
'Sedan Prata' Descrição atualizada sem alterar vaga Aprovado
T07 Movimentação Mover ABC1D23 da vaga F1/C1 para
F2/C3
Vaga F1/C1 liberada (.) e F2/C3 ocupada
(X) Aprovado
T08 Remoção de Veículo Remover o veículo LPA2026 Vaga desocupada e vetor ajustado sem
lacunas Aprovado
T09 Lotação Máxima Preencher 20 vagas e tentar cadastrar o
21º
Operação recusada por limite de 20
vagas
Aprovado
T10 Encerramento Selecionar a opção 0 no menu principal Recursos liberados via free() e sistema
encerrado Aprovado

8. Colaboração no GitHub e Branches
SISTEMA DE GESTÃO DE ESTACIONAMENTO — README & CÓDIGO C11 Vitória Sudré & Natasha Novaes
LPA 2026/2 — Roteiro Trabalho B2 | Repositório GitHub (Completo) Página 2 de 10
• main: Branch principal e estável do projeto.
• feature/cadastro-mapa: Branch utilizada por Vitória Sudré de Melo para criação do menu, mapa de vagas e funções de
cadastro.
• feature/expansao-relatorio: Branch utilizada por Natasha Novaes dos Santos para desenvolvimento do vetor dinâmico
(realloc), movimentação, remoção e relatórios.

9. Declarações Individuais de Autoria e Uso de IA
• Declaração de Vitória Sudré de Melo: Declaro que atuei no desenvolvimento do cadastro de veículos, controle da matriz
de vagas e validações alfanuméricas. Utilizei a ferramenta de IA generativa Gemini Notebook de forma transparente para
auxílio na revisão da lógica de tratamento de strings e formatação da documentação. Todo o código submetido foi revisado,
testado e compreendido por mim.
• Declaração de Natasha Novaes dos Santos: Declaro que atuei na implementação do vetor dinâmico de veículos,
expansão segura com realloc via ponteiro auxiliar, remoção sem lacunas e geração de relatórios. Utilizei a IA generativa
Gemini Notebook para suporte na estruturação dos casos de teste e verificação de gerenciamento de memória com free().
Todo o código foi implementado e validado por mim.
