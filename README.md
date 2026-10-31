Sistema de Gestão de Estacionamento

Disciplina: Lógica de Programação e Algoritmos (LPA)
Semestre: 2026/2
Linguagem: C (C11)
Modalidade: Dupla

1. Integrantes

Integrante 1: [Vitória Sudré de Melo] — [vitoria-sudre]
Responsável pelo menu, cadastro, validações e mapa de vagas.

Integrante 2: [Nome] — [GitHub]
Responsável pelo vetor dinâmico, realloc, movimentação, remoção e relatório.

2. Descrição do projeto

Sistema de terminal desenvolvido em C para gerenciar um estacionamento com 20 vagas, distribuídas em 4 fileiras e 5 colunas.

Os dados permanecem na memória durante a execução. O sistema mantém a consistência entre os veículos cadastrados e as vagas ocupadas.

3. Estruturas utilizadas

Matriz 4x5: representa as vagas. 0 = livre e 1 = ocupada.

Struct: armazena identificador, descrição, categoria, fileira e coluna.

Vetor dinâmico: armazena os veículos, começa com capacidade 2 e dobra utilizando realloc quando necessário.

O limite máximo é de 20 veículos.

4. Funcionalidades

1 - Cadastrar veículo
2 - Consultar veículo
3 - Atualizar descrição ou categoria
4 - Mover veículo
5 - Remover veículo
6 - Listar veículos
7 - Exibir mapa de vagas
8 - Relatório de ocupação
0 - Encerrar

5. Compilação e execução

Compilar:

gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/estacionamento.c -o estacionamento

Executar:

./estacionamento

6. Testes realizados

Teste

Operação

Resultado observado

Situação

T01

Estacionamento vazio

20 vagas livres e 0% de ocupação

Aprovado

T02

Cadastro válido

Veículo cadastrado

Aprovado

T03

ID duplicado

Cadastro recusado

Aprovado

T04

Vaga ocupada

Cadastro recusado

Aprovado

T05

Terceiro cadastro

Vetor ampliado de 2 para 4

Aprovado

T06

Atualização

Descrição alterada

Aprovado

T07

Movimentação

Origem livre e destino ocupado

Aprovado

T08

Remoção

Registro removido e vaga liberada

Aprovado

T09

Lotação máxima

21º cadastro recusado

Aprovado

T10

Encerramento

Memória liberada com free()

Aprovado

7. Colaboração no GitHub

main: branch principal.

feature/cadastro-mapa: cadastro, menu e mapa.

feature/expansao-relatorio: vetor dinâmico, movimentação, remoção e relatório.

As funcionalidades foram desenvolvidas em branches próprias e integradas à branch principal.

8. Declaração de autoria e uso de IA

Integrante 1: Participou do cadastro, validação de identificadores e mapa de vagas. Utilizou o Gemini Notebook para auxiliar na revisão das strings e da documentação.

Integrante 2: Participou do vetor dinâmico, realloc, movimentação, remoção e relatório de ocupação. [Informar se utilizou IA e para qual finalidade.]

Referências externas: [Informar as fontes utilizadas ou declarar que não foram utilizadas.]
