#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estacionamento.h"

/* ===== Leitura segura de entrada ===== */

StatusLeitura ler_linha(const char *prompt, char *buf, size_t tam)
{
    size_t n;
    int longa = 0;
    int ch;

    if (prompt != NULL) {
        fputs(prompt, stdout);
        fflush(stdout);
    }
    if (fgets(buf, (int)tam, stdin) == NULL) {
        buf[0] = '\0';
        return LEITURA_EOF;
    }

    n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') {
        buf[--n] = '\0';
    } else if (n == tam - 1) {
        /* buffer cheio sem '\n': ou a linha tem exatamente esse tamanho,
           ou ainda sobrou texto no teclado, que precisa ser descartado. */
        ch = getchar();
        if (ch != '\n' && ch != EOF) {
            longa = 1;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* descarta o restante */
            }
        }
    }
    if (n > 0 && buf[n - 1] == '\r') {
        buf[--n] = '\0';
    }

    if (longa) {
        return LEITURA_LONGA;
    }
    return (n == 0) ? LEITURA_VAZIA : LEITURA_OK;
}

StatusLeitura ler_inteiro(const char *prompt, int *valor)
{
    char buf[32];
    char *fim;
    long v;
    StatusLeitura st;

    *valor = 0;
    st = ler_linha(prompt, buf, sizeof buf);
    if (st == LEITURA_EOF || st == LEITURA_VAZIA) {
        return st;
    }
    if (st == LEITURA_LONGA) {
        return LEITURA_INVALIDA;
    }

    errno = 0;
    v = strtol(buf, &fim, 10);
    if (fim == buf || errno == ERANGE) {
        return LEITURA_INVALIDA;
    }
    while (*fim != '\0' && isspace((unsigned char)*fim)) {
        fim++;
    }
    if (*fim != '\0' || v < INT_MIN || v > INT_MAX) {
        return LEITURA_INVALIDA;
    }
    *valor = (int)v;
    return LEITURA_OK;
}

/* ===== Colecao dinamica ===== */

/* Aloca o bloco inicial (capacidade 2). Retorna 1 se deu certo. */
int colecao_iniciar(Colecao *c)
{
    Veiculo *p;

    c->dados = NULL;
    c->quantidade = 0;
    c->capacidade = 0;

    p = malloc(CAPACIDADE_INICIAL * sizeof *p);
    if (p == NULL) {
        return 0;
    }
    c->dados = p;
    c->capacidade = CAPACIDADE_INICIAL;
    return 1;
}

/* Dobra a capacidade. O resultado do realloc e guardado em um ponteiro
   temporario: se falhar, c->dados continua valido e nada e perdido. */
int colecao_ampliar(Colecao *c)
{
    int nova = (c->capacidade > 0) ? c->capacidade * 2 : CAPACIDADE_INICIAL;
    Veiculo *tmp = realloc(c->dados, (size_t)nova * sizeof *tmp);

    if (tmp == NULL) {
        return 0;
    }
    c->dados = tmp;
    c->capacidade = nova;
    return 1;
}

void colecao_liberar(Colecao *c)
{
    free(c->dados);
    c->dados = NULL;
    c->quantidade = 0;
    c->capacidade = 0;
}

/* Valida o identificador (7 caracteres, so A-Z e 0-9) e grava em saida a
   versao em maiusculas. Retorna 1 se valido, 0 se invalido. */
int normalizar_id(const char *entrada, char saida[TAM_ID + 1])
{
    size_t i;

    if (strlen(entrada) != TAM_ID) {
        return 0;
    }
    for (i = 0; i < TAM_ID; i++) {
        char ch = entrada[i];

        if (ch >= 'a' && ch <= 'z') {
            ch = (char)(ch - 'a' + 'A');
        }
        if (!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) {
            return 0;
        }
        saida[i] = ch;
    }
    saida[TAM_ID] = '\0';
    return 1;
}

/* Descricao: nao vazia (nem so espacos) e com no maximo 60 caracteres. */
int descricao_valida(const char *descricao)
{
    size_t n = strlen(descricao);
    size_t i;

    if (n == 0 || n > MAX_DESC) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (descricao[i] != ' ' && descricao[i] != '\t') {
            return 1;
        }
    }
    return 0;
}

int categoria_valida(int categoria)
{
    return categoria == CATEGORIA_CARRO || categoria == CATEGORIA_MOTO;
}

/* Procura pelo identificador JA normalizado. Retorna o indice ou -1. */
int indice_por_id(const Colecao *c, const char *id_normalizado)
{
    int i;

    for (i = 0; i < c->quantidade; i++) {
        if (strcmp(c->dados[i].id, id_normalizado) == 0) {
            return i;
        }
    }
    return -1;
}

/* Busca a partir do texto digitado (normaliza antes). NULL se nao achar. */
Veiculo *colecao_buscar(const Colecao *c, const char *id_bruto)
{
    char id[TAM_ID + 1];
    int indice;

    if (!normalizar_id(id_bruto, id)) {
        return NULL;
    }
    indice = indice_por_id(c, id);
    if (indice < 0) {
        return NULL;
    }
    return &c->dados[indice];
}

/* Remove o registro do indice, deslocando os seguintes uma posicao para a
   esquerda, para nao deixar lacunas. Nao libera o bloco de memoria. */
void colecao_remover_indice(Colecao *c, int indice)
{
    int i;

    if (indice < 0 || indice >= c->quantidade) {
        return;
    }
    for (i = indice; i < c->quantidade - 1; i++) {
        c->dados[i] = c->dados[i + 1];
    }
    c->quantidade--;
}

const char *nome_categoria(int categoria)
{
    if (categoria == CATEGORIA_CARRO) {
        return "Carro";
    }
    if (categoria == CATEGORIA_MOTO) {
        return "Moto";
    }
    return "?";
}

void veiculo_exibir(const Veiculo *v)
{
    printf("Identificador: %s\n", v->id);
    printf("Descricao....: %s\n", v->descricao);
    printf("Categoria....: %s\n", nome_categoria(v->categoria));
    printf("Posicao......: Fileira %d, Coluna %d\n", v->fileira, v->coluna);
}

void colecao_listar(const Colecao *c)
{
    const Veiculo *p;
    int i;

    if (c->quantidade == 0) {
        printf("Nenhum veiculo cadastrado.\n");
        return;
    }
    printf("%-9s %-6s %-8s %s\n", "ID", "Categ.", "Vaga", "Descricao");
    for (i = 0; i < c->quantidade; i++) {
        p = c->dados + i;
        printf("%-9s %-6s F%d/C%-4d %s\n", p->id, nome_categoria(p->categoria),
               p->fileira, p->coluna, p->descricao);
    }
    printf("Total: %d veiculo(s).\n", c->quantidade);
}

/* ===== Mapa de vagas e relatorio ===== */

void mapa_inicializar(int mapa[FILEIRAS][COLUNAS])
{
    int i, j;

    for (i = 0; i < FILEIRAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            mapa[i][j] = VAGA_LIVRE;
        }
    }
}

/* Valores de 1 a 4 (fileira) e 1 a 5 (coluna), como o usuario informa. */
int posicao_valida(int fileira, int coluna)
{
    return fileira >= 1 && fileira <= FILEIRAS && coluna >= 1 &&
           coluna <= COLUNAS;
}

int mapa_contar_ocupadas(int mapa[FILEIRAS][COLUNAS])
{
    int i, j, total = 0;

    for (i = 0; i < FILEIRAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == VAGA_OCUPADA) {
                total++;
            }
        }
    }
    return total;
}

void mapa_exibir(int mapa[FILEIRAS][COLUNAS])
{
    int i, j;

    printf("   ");
    for (j = 0; j < COLUNAS; j++) {
        printf("C%d  ", j + 1);
    }
    printf("\n");
    for (i = 0; i < FILEIRAS; i++) {
        printf("F%d ", i + 1);
        for (j = 0; j < COLUNAS; j++) {
            printf("%c   ", mapa[i][j] == VAGA_OCUPADA ? 'X' : '.');
        }
        printf("\n");
    }
    printf("Legenda: X = ocupada; . = livre\n");
}

void calcular_relatorio(const Colecao *c, int mapa[FILEIRAS][COLUNAS],
                        Relatorio *r)
{
    int i;

    r->total = TOTAL_VAGAS;
    r->ocupadas = mapa_contar_ocupadas(mapa);
    r->livres = r->total - r->ocupadas;
    r->percentual = (r->ocupadas * 100.0) / r->total;
    r->carros = 0;
    r->motos = 0;
    for (i = 0; i < c->quantidade; i++) {
        if (c->dados[i].categoria == CATEGORIA_CARRO) {
            r->carros++;
        } else if (c->dados[i].categoria == CATEGORIA_MOTO) {
            r->motos++;
        }
    }
}

void exibir_relatorio(const Relatorio *r)
{
    printf("Total de vagas.....: %d\n", r->total);
    printf("Vagas ocupadas.....: %d\n", r->ocupadas);
    printf("Vagas livres.......: %d\n", r->livres);
    printf("Percentual ocupacao: %.2f%%\n", r->percentual);
    printf("Carros.............: %d\n", r->carros);
    printf("Motos..............: %d\n", r->motos);
}

int verificar_consistencia(const Colecao *c, int mapa[FILEIRAS][COLUNAS])
{
    int visto[FILEIRAS][COLUNAS];
    int i, f, col;

    mapa_inicializar(visto);
    for (i = 0; i < c->quantidade; i++) {
        f = c->dados[i].fileira;
        col = c->dados[i].coluna;
        if (!posicao_valida(f, col)) {
            return 0;
        }
        if (mapa[f - 1][col - 1] != VAGA_OCUPADA) {
            return 0;
        }
        if (visto[f - 1][col - 1] == 1) {
            return 0; /* dois registros na mesma vaga */
        }
        visto[f - 1][col - 1] = 1;
    }
    return mapa_contar_ocupadas(mapa) == c->quantidade;
}

/* ===== Cadastro e atualizacao ===== */

/* Cadastro: todas as validacoes acontecem antes de mexer na colecao ou no
   mapa; o realloc (se necessario) e o ultimo passo que pode falhar. */
Resultado cadastrar_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                            const char *id_bruto, const char *descricao,
                            int categoria, int fileira, int coluna)
{
    char id[TAM_ID + 1];
    Veiculo *novo;

    if (c->quantidade >= TOTAL_VAGAS) {
        return RES_LOTADO;
    }
    if (!normalizar_id(id_bruto, id)) {
        return RES_ID_INVALIDO;
    }
    if (indice_por_id(c, id) >= 0) {
        return RES_ID_DUPLICADO;
    }
    if (!descricao_valida(descricao)) {
        return RES_DESC_INVALIDA;
    }
    if (!categoria_valida(categoria)) {
        return RES_CATEGORIA_INVALIDA;
    }
    if (!posicao_valida(fileira, coluna)) {
        return RES_POSICAO_INVALIDA;
    }
    if (mapa[fileira - 1][coluna - 1] != VAGA_LIVRE) {
        return RES_VAGA_OCUPADA;
    }
    if (c->quantidade >= c->capacidade && !colecao_ampliar(c)) {
        return RES_SEM_MEMORIA;
    }

    novo = &c->dados[c->quantidade];
    snprintf(novo->id, sizeof novo->id, "%s", id);
    snprintf(novo->descricao, sizeof novo->descricao, "%s", descricao);
    novo->categoria = categoria;
    novo->fileira = fileira;
    novo->coluna = coluna;
    c->quantidade++;
    mapa[fileira - 1][coluna - 1] = VAGA_OCUPADA;
    return RES_OK;
}

/* Atualizacao: valida os dois novos valores e so depois altera os dois. */
Resultado atualizar_veiculo(Colecao *c, const char *id_bruto,
                            const char *nova_descricao, int nova_categoria)
{
    char id[TAM_ID + 1];
    int indice;

    if (!normalizar_id(id_bruto, id)) {
        return RES_ID_INVALIDO;
    }
    indice = indice_por_id(c, id);
    if (indice < 0) {
        return RES_NAO_ENCONTRADO;
    }
    if (nova_descricao == NULL && nova_categoria == 0) {
        return RES_NADA_A_ALTERAR;
    }
    if (nova_descricao != NULL && !descricao_valida(nova_descricao)) {
        return RES_DESC_INVALIDA;
    }
    if (nova_categoria != 0 && !categoria_valida(nova_categoria)) {
        return RES_CATEGORIA_INVALIDA;
    }

    if (nova_descricao != NULL) {
        snprintf(c->dados[indice].descricao, sizeof c->dados[indice].descricao,
                 "%s", nova_descricao);
    }
    if (nova_categoria != 0) {
        c->dados[indice].categoria = nova_categoria;
    }
    return RES_OK;
}

const char *mensagem_resultado(Resultado r)
{
    switch (r) {
    case RES_OK:
        return "Operacao realizada com sucesso.";
    case RES_ID_INVALIDO:
        return "Identificador invalido: use exatamente 7 caracteres (A-Z e 0-9).";
    case RES_ID_DUPLICADO:
        return "Ja existe um veiculo com esse identificador.";
    case RES_DESC_INVALIDA:
        return "Descricao invalida: nao pode ser vazia nem passar de 60 caracteres.";
    case RES_CATEGORIA_INVALIDA:
        return "Categoria invalida: use 1 (carro) ou 2 (moto).";
    case RES_POSICAO_INVALIDA:
        return "Posicao invalida: fileira de 1 a 4 e coluna de 1 a 5.";
    case RES_VAGA_OCUPADA:
        return "Vaga ocupada: operacao recusada.";
    case RES_LOTADO:
        return "Estacionamento lotado: as 20 vagas estao ocupadas.";
    case RES_SEM_MEMORIA:
        return "Erro: falta de memoria. Nada foi alterado.";
    case RES_NAO_ENCONTRADO:
        return "Veiculo nao localizado.";
    case RES_MESMA_VAGA:
        return "O veiculo ja esta nessa vaga. Nada foi alterado.";
    case RES_NADA_A_ALTERAR:
        return "Nenhum dado informado para alterar.";
    }
    return "Erro desconhecido.";
}

/* ===== Movimentacao e remocao ===== */

/* Movimentacao: so mexe no mapa e no registro depois de validar tudo. */
Resultado mover_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                        const char *id_bruto, int nova_fileira,
                        int nova_coluna)
{
    char id[TAM_ID + 1];
    int indice;
    Veiculo *v;

    if (!normalizar_id(id_bruto, id)) {
        return RES_ID_INVALIDO;
    }
    indice = indice_por_id(c, id);
    if (indice < 0) {
        return RES_NAO_ENCONTRADO;
    }
    if (!posicao_valida(nova_fileira, nova_coluna)) {
        return RES_POSICAO_INVALIDA;
    }

    v = &c->dados[indice];
    if (v->fileira == nova_fileira && v->coluna == nova_coluna) {
        return RES_MESMA_VAGA;
    }
    if (mapa[nova_fileira - 1][nova_coluna - 1] != VAGA_LIVRE) {
        return RES_VAGA_OCUPADA;
    }

    mapa[v->fileira - 1][v->coluna - 1] = VAGA_LIVRE;
    mapa[nova_fileira - 1][nova_coluna - 1] = VAGA_OCUPADA;
    v->fileira = nova_fileira;
    v->coluna = nova_coluna;
    return RES_OK;
}

/* Remocao: libera a vaga e tira o registro do vetor sem deixar lacuna. */
Resultado remover_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                          const char *id_bruto)
{
    char id[TAM_ID + 1];
    int indice;
    const Veiculo *v;

    if (!normalizar_id(id_bruto, id)) {
        return RES_ID_INVALIDO;
    }
    indice = indice_por_id(c, id);
    if (indice < 0) {
        return RES_NAO_ENCONTRADO;
    }

    v = &c->dados[indice];
    mapa[v->fileira - 1][v->coluna - 1] = VAGA_LIVRE;
    colecao_remover_indice(c, indice);
    return RES_OK;
}
