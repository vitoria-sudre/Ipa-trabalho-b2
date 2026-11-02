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

