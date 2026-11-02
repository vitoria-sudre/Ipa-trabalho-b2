# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos (LPA)  
**Semestre:** 2026/2  
**Linguagem:** C (Padrão C11)  
**Modalidade:** Dupla

---

## 1. Integrantes e Identificação

**Integrante 1:** Vitoria Sudre de Melo  
**GitHub:** @vitoriasudre

**Responsabilidades:** Estrutura de dados, menu principal, cadastro de veículos, validações alfanuméricas e matriz do mapa de vagas.

**Integrante 2:** Natasha Novaes dos Santos  
**GitHub:** @natashanovaes

**Responsabilidades:** Vetor dinâmico, realocação de memória (`realloc`), movimentação, remoção de registros e relatório de ocupação.

---

## 2. Descrição do Cenário e Escopo

O **Sistema de Gestão de Estacionamento** é uma aplicação de terminal desenvolvida em linguagem C11 para gerenciar 20 vagas físicas de estacionamento, organizadas em uma matriz de 4 fileiras por 5 colunas.

O sistema permite cadastrar, consultar, atualizar, movimentar, remover e listar veículos, além de apresentar o mapa de vagas e um relatório de ocupação.

### Premissas e Regras de Negócio

1. Os registros permanecem na memória durante a execução do programa.
2. Cada veículo possui um identificador único de 7 caracteres alfanuméricos.
3. A descrição do veículo deve conter entre 1 e 60 caracteres.
4. As categorias são: 1 para carro e 2 para moto.
5. O número de veículos cadastrados deve corresponder ao número de vagas ocupadas na matriz.
6. O vetor dinâmico começa com capacidade para 2 veículos e dobra de tamanho utilizando `realloc` quando necessário.
7. O sistema permite no máximo 20 veículos cadastrados.
8. Não é permitido cadastrar dois veículos na mesma vaga.
9. Ao remover um veículo, a vaga correspondente deve ser liberada.

---

## 3. Estruturas de Dados Utilizadas

O projeto utiliza duas estruturas principais.

### Estrutura Veiculo

Armazena as informações de cada veículo cadastrado.

```c
typedef struct {
    char identificador[8];
    char descricao[61];
    int categoria;
    int fileira;
    int coluna;
} Veiculo;
```

### Estrutura Estacionamento

Armazena o mapa de vagas e o vetor dinâmico de veículos.

```c
typedef struct {
    int mapa[4][5];
    Veiculo *veiculos;
    size_t quantidade;
    size_t capacidade;
} Estacionamento;
```

A matriz representa as vagas físicas do estacionamento, enquanto o vetor dinâmico armazena os registros dos veículos cadastrados.

---

## 4. Funcionalidades e Menu

O sistema apresenta as seguintes opções:

```text
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
```

### Descrição das funcionalidades

- **Cadastrar veículo:** registra um novo veículo em uma vaga disponível.
- **Consultar veículo:** localiza um veículo pelo identificador.
- **Atualizar veículo:** modifica a descrição ou a categoria.
- **Mover veículo:** transfere um veículo para outra vaga livre.
- **Remover veículo:** exclui o cadastro e libera a vaga.
- **Listar veículos:** apresenta os veículos cadastrados.
- **Exibir mapa:** mostra a situação das 20 vagas.
- **Relatório de ocupação:** apresenta a quantidade de vagas ocupadas e livres e informações sobre as categorias dos veículos.

---

## 5. Organização dos Arquivos

O projeto está organizado da seguinte forma:

```text
lpa-trabalho-b2/
│
├── README.md
├── .gitignore
│
└── src/
    ├── main.c
    ├── estacionamento.h
    └── estacionamento.c
```

### Responsabilidade dos arquivos

- **main.c:** contém a função principal, o menu interativo e o controle do programa.
- **estacionamento.h:** apresenta as estruturas, constantes e protótipos das funções.
- **estacionamento.c:** contém a implementação das funcionalidades do sistema.

### Principais funções

- `inicializar_estacionamento()`: inicializa a matriz e o vetor dinâmico.
- `liberar_estacionamento()`: libera
