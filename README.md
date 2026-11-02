
# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos (LPA)  
**Semestre:** 2026/2  
**Linguagem:** C (Padrão C11)  
**Modalidade:** Dupla

---

## 1. Integrantes e Identificação

* **Integrante 1:** Vitoria Sudre de Melo — GitHub: @vitoriasudre
  *Responsabilidade:* Estrutura de dados, menu principal, cadastro de veículos, validações alfanuméricas e matriz do mapa de vagas.

* **Integrante 2:** Natasha Novaes dos Santos — GitHub: @natashanovaes
  *Responsabilidade:* Vetor dinâmico, realocação de memória (realloc), movimentação, remoção de registros e relatório de ocupação.

---

## 2. Descrição do Cenário e Escopo

O **Sistema de Gestão de Estacionamento** é uma aplicação de terminal escrita em C11 para gerenciar dinamicamente 20 vagas físicas de estacionamento, organizadas em uma matriz de 4 fileiras por 5 colunas.

### Premissas e Regras de Negócio

1. **Dados em Memória:** Os registros permanecem na memória durante a execução.
2. **Critério de Consistência:** O número de veículos cadastrados deve ser igual ao número de vagas ocupadas na matriz.
3. **Gestão Dinâmica:** O vetor dinâmico inicia com capacidade 2 e dobra de tamanho utilizando `realloc` quando necessário.
4. **Limite Físico:** O sistema recusa novos cadastros ao atingir 20 vagas ocupadas.

---

## 3. Estruturas de Dados Utilizadas

```c
typedef struct {
    char identificador[8];
    char descricao[61];
    int categoria;
    int fileira;
    int coluna;
} Veiculo;

typedef struct {
    int mapa[4][5];
    Veiculo *veiculos;
    size_t quantidade;
    size_t capacidade;
} Estacionamento;
```

---

## 4. Funcionalidades e Menu

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

---

## 5. Organização dos Arquivos

O projeto está organizado da seguinte forma:

- `src/estacionamento.h`: Definições das estruturas, constantes e protótipos das funções.
- `src/estacionamento.c`: Implementação das operações de cadastro, consulta, atualização, movimentação, remoção, listagem, mapa e relatório.
- `src/main.c`: Função principal, menu interativo e controle do programa.

---

## 6. Instruções de Compilação e Execução

### Compilação (GCC C11)

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento
```

### Execução

```bash
./estacionamento
```

---

## 7. Casos de Teste Documentados

| Teste | Descrição | Resultado Esperado | Situação |
|---|---|---|---|
| T01 | Estacionamento vazio | 20 vagas livres | Aprovado |
| T02 | Cadastro válido | Veículo cadastrado | Aprovado |
| T03 | Identificador duplicado | Cadastro rejeitado | Aprovado |
| T04 | Vaga ocupada | Operação recusada | Aprovado |
| T05 | Crescimento dinâmico | Vetor expande de 2 para 4 | Aprovado |
| T06 | Atualização | Descrição atualizada | Aprovado |
| T07 | Movimentação | Veículo muda de vaga | Aprovado |
| T08 | Remoção | Registro removido e vaga liberada | Aprovado |
| T09 | Lotação máxima | 21º cadastro recusado | Aprovado |
| T10 | Encerramento | Memória liberada com `free()` | Aprovado |

---

## 8. Colaboração no GitHub e Branches

- **main:** Branch principal e estável do projeto.
- **feature/cadastro-mapa:** Desenvolvimento do cadastro, menu e mapa de vagas por Vitoria Sudre de Melo.
- **feature/expansao-relatorio:** Desenvolvimento do vetor dinâmico, movimentação, remoção e relatórios por Natasha Novaes dos Santos.

---

## 9. Declaração Individual de Autoria e Uso de IA

* **Declaração de Vitoria Sudre de Melo:** Declaro que atuei no desenvolvimento do cadastro de veículos, controle da matriz de vagas e validações alfanuméricas. Utilizei a ferramenta de IA generativa Gemini Notebook para auxiliar na revisão da lógica de tratamento de strings e formatação da documentação. Todo o código submetido foi revisado, testado e compreendido por mim.

* **Declaração de Natasha Novaes dos Santos:** Declaro que atuei na implementação do vetor dinâmico de veículos, expansão segura com `realloc`, remoção sem lacunas e geração de relatórios. Utilizei a IA generativa Gemini Notebook para suporte na estruturação dos casos de teste e verificação do gerenciamento de memória com `free()`. Todo o código foi implementado e validado por mim.

