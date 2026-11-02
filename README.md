# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos (LPA)  
**Semestre:** 2026/2  
**Linguagem:** C (Padrão C11)  
**Modalidade:** Dupla

---

## 1. Integrantes e Identificação

**Integrante 1:** Vitoria Sudre de Melo  
**GitHub:** [@vitoriasudre](https://github.com/vitoriasudre)

**Responsabilidades:** Estrutura de dados, menu principal, cadastro de veículos, validações alfanuméricas e matriz do mapa de vagas.

**Integrante 2:** Natasha Novaes dos Santos  
**GitHub:** [@natashanovaes](https://github.com/natashanovaes)

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
- **Relatório de ocupação:** apresenta a quantidade de vagas ocupadas e livres, além das informações sobre as categorias dos veículos.

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
- `liberar_estacionamento()`: libera a memória utilizada pelo vetor dinâmico.
- `cadastrar_veiculo()`: realiza o cadastro de veículos.
- `consultar_veiculo()`: consulta um veículo pelo identificador.
- `atualizar_veiculo()`: atualiza os dados do veículo.
- `mover_veiculo()`: altera a posição do veículo.
- `remover_veiculo()`: remove um veículo cadastrado.
- `listar_veiculos()`: apresenta os veículos cadastrados.
- `exibir_mapa_vagas()`: apresenta o mapa do estacionamento.
- `exibir_relatorio_ocupacao()`: apresenta o relatório de ocupação.

---

## 6. Instruções de Compilação e Execução

Para compilar o programa, é necessário ter o compilador GCC instalado.

### Compilação (GCC C11)

Execute o seguinte comando na pasta principal do projeto:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
```

### Execução no Linux ou macOS

```bash
./estacionamento
```

### Execução no Windows

```bash
estacionamento.exe
```

---

## 7. Casos de Teste Documentados

Os testes têm como objetivo verificar as funcionalidades, as validações e as regras de negócio do sistema.

| Teste | Entrada ou operação | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|
| T01 | Consultar o mapa sem veículos cadastrados | Apresentar 20 vagas livres | Preencher após executar | Pendente |
| T02 | Cadastrar veículo com dados válidos | Cadastro realizado com sucesso | Preencher após executar | Pendente |
| T03 | Cadastrar identificador já existente | Cadastro rejeitado | Preencher após executar | Pendente |
| T04 | Tentar cadastrar veículo em vaga ocupada | Operação recusada | Preencher após executar | Pendente |
| T05 | Cadastrar o terceiro veículo | Vetor expandido de 2 para 4 posições | Preencher após executar | Pendente |
| T06 | Atualizar descrição ou categoria | Dados atualizados | Preencher após executar | Pendente |
| T07 | Mover veículo para uma vaga livre | Nova posição registrada | Preencher após executar | Pendente |
| T08 | Remover veículo cadastrado | Registro removido e vaga liberada | Preencher após executar | Pendente |
| T09 | Tentar cadastrar o 21º veículo | Cadastro recusado por lotação máxima | Preencher após executar | Pendente |
| T10 | Encerrar o programa | Memória do vetor liberada com `free()` | Preencher após executar | Pendente |

**Observação:** os resultados observados e a situação dos testes devem ser preenchidos depois que cada teste for executado. Um teste deve ser marcado como aprovado somente quando o resultado corresponder ao esperado.

---

## 8. Colaboração no GitHub e Branches

O desenvolvimento foi organizado por meio de branches para separar as funcionalidades implementadas por cada integrante.

- **main:** branch principal do projeto.
- **feature/cadastro-mapa:** desenvolvimento do cadastro, menu e mapa de vagas por Vitoria Sudre de Melo.
- **feature/expansao-relatorio:** desenvolvimento do vetor dinâmico, movimentação, remoção e relatórios por Natasha Novaes dos Santos.

As alterações desenvolvidas nas branches devem ser integradas à branch principal.

Cada integrante deve possuir pelo menos quatro commits significativos, demonstrando sua participação no desenvolvimento do projeto.

---

## 9. Declaração Individual de Autoria e Uso de IA

### Declaração de Vitoria Sudre de Melo

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando na estrutura de dados, no menu principal, no cadastro de veículos, nas validações e na matriz do mapa de vagas. Utilizamos o ChatGPT como ferramenta de inteligência artificial para obter uma base inicial e orientações para a estruturação do projeto. As sugestões foram analisadas, adaptadas e revisadas durante o desenvolvimento.

### Declaração de Natasha Novaes dos Santos

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando no vetor dinâmico, na realocação de memória com `realloc`, na movimentação, na remoção de veículos e nos relatórios de ocupação. Também utilizamos o ChatGPT como apoio para construir a base inicial do código, esclarecer dúvidas e auxiliar na organização das funcionalidades. O código foi revisado e estudado por nós durante a realização do trabalho.

---

**Trabalho do 2º Bimestre — Lógica de Programação e Algoritmos (LPA)**  
**Semestre: 2026/2**
