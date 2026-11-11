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
5. Os identificadores são convertidos para letras maiúsculas.
6. Não é permitido cadastrar identificadores duplicados.
7. O número de veículos cadastrados deve corresponder ao número de vagas ocupadas na matriz.
8. O vetor dinâmico começa com capacidade para 2 veículos e dobra de tamanho utilizando `realloc` quando necessário.
9. O sistema permite no máximo 20 veículos cadastrados.
10. Não é permitido cadastrar dois veículos na mesma vaga.
11. Ao remover um veículo, a vaga correspondente deve ser liberada.
12. Operações inválidas devem ser recusadas sem modificar os dados do estacionamento.

---

## 3. Estruturas de Dados Utilizadas

O projeto utiliza uma estrutura para armazenar os dados dos veículos, uma matriz para representar as vagas e um vetor dinâmico para armazenar os registros.

### 3.1. Estrutura Veiculo

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

### 3.2. Matriz do Estacionamento

A matriz possui 4 fileiras e 5 colunas, totalizando 20 vagas.

- `0`: vaga livre.
- `1`: vaga ocupada.

### 3.3. Vetor Dinâmico

O vetor armazena os registros dos veículos e utiliza alocação dinâmica de memória.

Sua capacidade inicial é de 2 registros. Quando a quantidade de veículos atinge a capacidade disponível, o sistema utiliza `realloc` para dobrar o espaço reservado.

Exemplo de crescimento:

| Etapa | Capacidade |
|---|---:|
| Inicial | 2 |
| Primeira expansão | 4 |
| Segunda expansão | 8 |
| Terceira expansão | 16 |
| Quarta expansão | 32 |

Mesmo que a capacidade interna chegue a 32, o estacionamento continua limitado a 20 veículos, devido à quantidade de vagas físicas.

---

## 4. Funcionalidades e Menu

O sistema apresenta as seguintes opções:

```text
=========================================
         ESTACIONAMENTO
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

- **Cadastrar veículo:** registra um novo veículo em uma vaga disponível, verificando os dados e a disponibilidade.
- **Consultar veículo:** localiza um veículo pelo identificador e apresenta seus dados completos.
- **Atualizar veículo:** permite modificar a descrição ou a categoria do veículo.
- **Mover veículo:** transfere um veículo para outra vaga livre, atualizando sua posição.
- **Remover veículo:** exclui o cadastro e libera a vaga correspondente.
- **Listar veículos:** apresenta os veículos cadastrados e suas informações.
- **Exibir mapa:** mostra a situação das 20 vagas, utilizando `X` para ocupada e `.` para livre.
- **Relatório de ocupação:** apresenta o total de vagas, vagas ocupadas, vagas livres, percentual de ocupação, quantidade de carros e motos, quantidade de registros e capacidade do vetor, conforme apresentado pela implementação.

---

## 5. Organização dos Arquivos

O projeto está organizado da seguinte forma:

```text
lpa-trabalho-b2/
├── src/
│   ├── estacionamento.c
│   ├── estacionamento.h
│   └── main.c
├── .gitignore
├── compilar.bat
├── Licença
└── README.md
```

### Responsabilidade dos arquivos

- `main.c`: contém a função principal, o menu interativo e o controle da execução do programa.
- `estacionamento.h`: apresenta as estruturas, constantes e protótipos das funções.
- `estacionamento.c`: contém a implementação das funcionalidades do sistema.
- `README.md`: apresenta a documentação, as funcionalidades, as instruções de execução e os resultados dos testes.
- `.gitignore`: define os arquivos que não devem ser enviados ao repositório.
- `Licença`: contém as informações de licença do projeto.

### Principais funções

- `inicializar_estacionamento()`: inicializa os dados do estacionamento.
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

É necessário ter o compilador GCC instalado e configurado no sistema.

### 6.1 Compilação e execução automática no Windows (recomendado)
```bash
compilar.bat
```
### 6.2. Compilação e execução manual no Windows (caso o automático não funcione)

Execute o seguinte comando na pasta principal do projeto:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento.exe
```

### 2. Executar o programa:

```bash
.\estacionamento.exe
```

### 6.3. Compilação e execução no Linux ou macOS

Para compilar:

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
```

Para executar:

```bash
./estacionamento
```
### Observação: o arquivo compilar.bat automatiza a compilação e execução no Windows. Nos sistemas Linux e macOS, os comandos devem ser executados manualmente. O GCC precisa estar instalado no computador.

---

## 7. Casos de Teste Documentados

Foram realizados testes para verificar o funcionamento das funcionalidades, as validações de entrada, o gerenciamento de memória e as regras de negócio.

Os exemplos abaixo foram retirados das execuções realizadas no terminal. Os casos de teste apresentam cenários de execução próprios, podendo utilizar cadastros e posições diferentes.

| Teste | Entrada ou operação | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|
| T01 | Verificar o estacionamento inicialmente vazio | Apresentar as 20 vagas livres | A execução específica do cenário vazio não está demonstrada nos registros apresentados nesta documentação | Não documentado neste registro |
| T02 | Cadastrar veículos com dados válidos | Registrar os veículos e apresentar seus dados | CALAKS7 e CALAKS8 foram cadastrados com sucesso | Aprovado |
| T03 | Tentar cadastrar identificador já existente | Recusar o cadastro duplicado | O sistema informou que já existia um veículo com esse identificador | Aprovado |
| T04 | Tentar cadastrar em vaga ocupada ou posição inválida | Recusar a operação | O sistema apresentou mensagens de vaga ocupada e posição inválida | Aprovado |
| T05 | Cadastrar o terceiro veículo após os dois primeiros | Ampliar a capacidade interna de 2 para 4 posições | CALAKS9 foi cadastrado com sucesso; a implementação de `realloc` e a duplicação da capacidade foram conferidas no código | Aprovado na implementação |
| T06 | Atualizar descrição ou categoria | Apresentar os novos dados | BBB0002 foi atualizado para descrição carro e categoria Moto | Aprovado |
| T07 | Mover veículo para vaga livre | Atualizar a posição do veículo | BBB0002 foi movido para F4/C1 | Aprovado |
| T08 | Remover veículo cadastrado | Excluir o registro e liberar a vaga | BBB0002 foi removido e não apareceu na listagem seguinte | Aprovado |
| T09 | Tentar cadastrar o 21º veículo | Recusar o cadastro quando as 20 vagas estiverem ocupadas | O sistema informou que o estacionamento estava lotado | Aprovado |
| T10 | Encerrar o programa | Finalizar a execução e liberar a memória alocada | O terminal apresentou a mensagem de encerramento e memória liberada | Aprovado na execução |

### 7.1. T02 — Cadastro de Veículos Válidos

Foram cadastrados dois veículos com informações válidas.

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

### 7.2. T03 — Identificador Duplicado

Foi realizada uma tentativa de cadastrar novamente o identificador `bbb0002`, que já havia sido cadastrado anteriormente como `BBB0002`.

**Entrada utilizada:**

```text
Identificador: bbb0002
Descricao: branco
Categoria: 1
```

**Resultado apresentado:**

```text
Cadastro cancelado. Ja existe um veiculo com esse identificador.
```

**Resultado:** o sistema reconheceu o identificador duplicado, mesmo com letras minúsculas, e recusou o cadastro.

### 7.3. T04 — Vaga Ocupada e Posição Inválida

Foi tentado cadastrar um veículo em uma vaga já ocupada.

**Entrada utilizada:**

```text
Identificador: bbb0002
Fileira: 1
Coluna: 5
```

**Resultado apresentado:**

```text
Cadastro cancelado. Vaga ocupada: operacao recusada.
```

Também foi testada uma posição fora dos limites permitidos.

**Entrada utilizada:**

```text
Identificador: aaa9990
Fileira: 0
Coluna: 6
```

**Resultado apresentado:**

```text
Cadastro cancelado. Posicao invalida:
fileira de 1 a 4 e coluna de 1 a 5.
```

**Resultado:** as operações inválidas foram recusadas.

### 7.4. T05 — Crescimento do Vetor Dinâmico

O vetor começa com capacidade para 2 veículos.

Foram cadastrados dois veículos e, em seguida, um terceiro:

```text
Identificador: CALAKS9
Descricao: azul
Categoria: 1
Fileira: 1
Coluna: 3

Veiculo CALAKS9 cadastrado na vaga F1/C3.
```

Após o terceiro cadastro, o relatório apresentou:

```text
Total de vagas.....: 20
Vagas ocupadas.....: 3
Vagas livres.......: 17
Percentual ocupacao: 15.00%
Carros.............: 2
Motos..............: 1
```

A implementação utiliza `realloc` para ampliar a capacidade do vetor quando necessário, seguindo a sequência:

```text
2 -> 4 -> 8 -> 16 -> 32
```

A função utiliza um ponteiro temporário para receber o resultado de `realloc`, evitando perder a referência à memória anterior caso a realocação falhe.

**Resultado:** o terceiro cadastro foi realizado e os registros anteriores foram preservados. A lógica de expansão de 2 para 4 foi conferida no código-fonte. O relatório apresenta a ocupação física, não a capacidade interna do vetor.

### 7.5. T06 — Atualização de Veículo

Foi atualizado o veículo `BBB0002`, modificando sua descrição e categoria.

**Entrada utilizada:**

```text
Identificador: bbb0002
Nova descricao: carro
Nova categoria: 2
```

**Resultado apresentado:**

```text
Veiculo atualizado:
Identificador: BBB0002
Descricao....: carro
Categoria....: Moto
Posicao......: Fileira 2, Coluna 4
```

**Resultado:** os dados foram atualizados corretamente.

### 7.6. T07 — Movimentação de Veículo

Foi realizada a movimentação do veículo `BBB0002` para a fileira 4, coluna 1.

**Entrada utilizada:**

```text
Identificador: bbb0002
Nova fileira: 4
Nova coluna: 1
```

**Resultado apresentado:**

```text
Veiculo movido para a vaga F4/C1.
```

**Resultado:** o sistema confirmou a movimentação para a nova posição.

### 7.7. T08 — Remoção de Veículo

Foi removido o veículo `BBB0002`.

**Entrada utilizada:**

```text
Identificador: bbb0002
```

**Resultado apresentado:**

```text
Veiculo removido e vaga liberada.
```

Na listagem realizada depois da remoção, o veículo não apareceu:

```text
ID        Categ. Vaga     Descricao
CALAKS8   Carro  F1/C5    verde
CALAKS7   Carro  F2/C2    azul
AAA0001   Moto   F1/C1    verde
AAA0006   Moto   F4/C4    preto

Total: 4 veiculo(s).
```

**Resultado:** o veículo removido deixou de aparecer na listagem.

### 7.8. T09 — Limite Máximo de 20 Veículos

Foram realizados cadastros até preencher todas as 20 vagas.

Com o estacionamento lotado, foi tentado um novo cadastro:

```text
Identificador: FGH8654
Descricao: branco
Categoria: 2
Fileira: 1
Coluna: 3
```

**Resultado apresentado:**

```text
Cadastro cancelado. Estacionamento lotado:
as 20 vagas estao ocupadas.
```

O relatório apresentou:

```text
Total de vagas.....: 20
Vagas ocupadas.....: 20
Vagas livres.......: 0
Percentual ocupacao: 100.00%
Carros.............: 9
Motos..............: 11
```

**Resultado:** o sistema recusou o cadastro adicional e manteve o limite de 20 veículos.

### 7.9. T10 — Encerramento do Programa

Ao selecionar a opção `0`, o programa apresentou:

```text
Encerrando. Memoria liberada.
```

**Resultado:** o programa encerrou a execução e apresentou a mensagem de liberação da memória.

---

## 8. Validações e Testes Complementares

Além dos casos principais, foram verificadas situações de entrada inválida e consistência dos dados.

| Verificação | Exemplo utilizado | Resultado observado |
|---|---|---|
| Identificador com espaço no final | ABC1234 seguido de espaço | Entrada recusada por formato inválido |
| Identificador duplicado | bbb0002 | Cadastro recusado |
| Identificador em letras minúsculas | ccc0976 | Cadastrado como CCC0976 |
| Categoria não informada | Campo vazio | Cadastro cancelado por categoria inválida |
| Fileira fora do limite | 0 ou 6 | Entrada recusada |
| Coluna fora do limite | 6 | Entrada recusada |
| Vaga ocupada | F1/C5 | Operação recusada |
| Opção inválida no menu | Texto em vez de número | Sistema informa opção inexistente |
| Tentativa de cadastro com estacionamento lotado | 21º veículo | Cadastro recusado |
| Listagem de veículos | Opção 6 | Registros e posições apresentados |
| Exibição do mapa | Opção 7 | X para ocupada e . para livre |
| Relatório de ocupação | Opção 8 | Quantidades e percentual apresentados |

### 8.1. Exemplo de Identificador Inválido

Foi informado um identificador com espaço no final:

```text
Identificador: ABC1234 

Cadastro cancelado. Identificador invalido:
use exatamente 7 caracteres (A-Z e 0-9).
```

**Resultado:** o sistema recusou o identificador fora do formato permitido.

### 8.2. Exemplo de Categoria Inválida

Foi deixado o campo de categoria vazio:

```text
Categoria (1 = carro, 2 = moto):

Cadastro cancelado. Categoria invalida:
use 1 (carro) ou 2 (moto).
```

**Resultado:** o cadastro foi cancelado.

### 8.3. Exemplo de Normalização do Identificador

Foi informado o identificador em letras minúsculas:

```text
Identificador: ccc0976

Veiculo CCC0976 cadastrado na vaga F2/C1.
```

**Resultado:** o sistema converteu o identificador para letras maiúsculas.

---

## 9. Consistência dos Dados

O sistema deve manter a correspondência entre os registros armazenados no vetor dinâmico e as posições ocupadas na matriz.

A regra de consistência é:

**Quantidade de registros = Quantidade de vagas ocupadas**

Dessa forma:

- Ao cadastrar um veículo, um registro é adicionado e uma vaga é marcada como ocupada.
- Ao mover um veículo, a posição anterior é liberada e a nova posição é ocupada.
- Ao remover um veículo, o registro é excluído e a vaga é liberada.
- O relatório deve refletir a situação atual do estacionamento.

---

## 10. Declaração Individual de Autoria e Uso de IA

### Declaração de Vitoria Sudre de Melo

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando na estrutura de dados, no menu principal, no cadastro de veículos, nas validações e na matriz do mapa de vagas. Utilizamos o ChatGPT como ferramenta de inteligência artificial para obter uma base inicial e orientações para a estruturação do projeto. As sugestões foram analisadas, adaptadas e revisadas durante o desenvolvimento.

### Declaração de Natasha Novaes dos Santos

Declaro que participei do desenvolvimento do Sistema de Gestão de Estacionamento, atuando no vetor dinâmico, na realocação de memória com `realloc`, na movimentação, na remoção de veículos e nos relatórios de ocupação. Também utilizamos o ChatGPT como apoio para construir a base inicial do código, esclarecer dúvidas e auxiliar na organização das funcionalidades. O código foi revisado e estudado por nós durante a realização do trabalho.
