#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stddef.h>

/* ===== Constantes e tipos ===== */

/* Constantes do estacionamento */
#define FILEIRAS 4
#define COLUNAS 5
#define TOTAL_VAGAS (FILEIRAS * COLUNAS)

#define TAM_ID 7              /* identificador: exatamente 7 caracteres */
#define MAX_DESC 60           /* descricao: ate 60 caracteres */
#define CAPACIDADE_INICIAL 2  /* capacidade inicial do vetor dinamico */

#define CATEGORIA_CARRO 1
#define CATEGORIA_MOTO 2

#define VAGA_LIVRE 0
#define VAGA_OCUPADA 1

/* Registro de um veiculo. fileira e coluna sao guardadas de 1 a 4 / 1 a 5
   (mesmos valores que o usuario enxerga); a conversao para indice da matriz
   (valor - 1) e feita sempre no momento de acessar o mapa. */
typedef struct {
    char id[TAM_ID + 1];
    char descricao[MAX_DESC + 1];
    int categoria;
    int fileira;
    int coluna;
} Veiculo;

/* Colecao dinamica de veiculos ativos.
   quantidade = registros em uso; capacidade = espaco reservado. */
typedef struct {
    Veiculo *dados;
    int quantidade;
    int capacidade;
} Colecao;

/* Dados do relatorio de ocupacao */
typedef struct {
    int total;
    int ocupadas;
    int livres;
    double percentual;
    int carros;
    int motos;
} Relatorio;

/* Resultado das operacoes que podem ser recusadas */
typedef enum {
    RES_OK = 0,
    RES_ID_INVALIDO,
    RES_ID_DUPLICADO,
    RES_DESC_INVALIDA,
    RES_CATEGORIA_INVALIDA,
    RES_POSICAO_INVALIDA,
    RES_VAGA_OCUPADA,
    RES_LOTADO,
    RES_SEM_MEMORIA,
    RES_NAO_ENCONTRADO,
    RES_MESMA_VAGA,
    RES_NADA_A_ALTERAR
} Resultado;

/* ===== Leitura segura de entrada ===== */

typedef enum {
    LEITURA_OK = 0,
    LEITURA_VAZIA,    /* usuario apenas apertou Enter */
    LEITURA_LONGA,    /* digitou mais do que cabe no buffer (resto descartado) */
    LEITURA_INVALIDA, /* ler_inteiro: texto nao e um numero inteiro */
    LEITURA_EOF       /* fim da entrada */
} StatusLeitura;

/* Le uma linha com fgets (nunca estoura o buffer), remove o '\n' e descarta
   o excesso. */
StatusLeitura ler_linha(const char *prompt, char *buf, size_t tam);

/* Le um inteiro. Em qualquer status diferente de LEITURA_OK, *valor = 0
   (valor que nenhum campo do sistema aceita como valido). */
StatusLeitura ler_inteiro(const char *prompt, int *valor);

/* ===== Colecao dinamica ===== */

/* Gerenciamento da colecao dinamica (malloc / realloc / free) */
int colecao_iniciar(Colecao *c);   /* 1 = ok, 0 = falha de memoria */
int colecao_ampliar(Colecao *c);   /* dobra a capacidade; 1 = ok, 0 = falha */
void colecao_liberar(Colecao *c);

/* Validacao e normalizacao de campos */
int normalizar_id(const char *entrada, char saida[TAM_ID + 1]);
int descricao_valida(const char *descricao);
int categoria_valida(int categoria);

/* Busca e remocao */
int indice_por_id(const Colecao *c, const char *id_normalizado);
Veiculo *colecao_buscar(const Colecao *c, const char *id_bruto);
void colecao_remover_indice(Colecao *c, int indice);

/* Saida */
const char *nome_categoria(int categoria);
void colecao_listar(const Colecao *c);
void veiculo_exibir(const Veiculo *v);

/* ===== Mapa de vagas e relatorio ===== */

void mapa_inicializar(int mapa[FILEIRAS][COLUNAS]);
int posicao_valida(int fileira, int coluna);
int mapa_contar_ocupadas(int mapa[FILEIRAS][COLUNAS]);
void mapa_exibir(int mapa[FILEIRAS][COLUNAS]);

void calcular_relatorio(const Colecao *c, int mapa[FILEIRAS][COLUNAS],
                        Relatorio *r);
void exibir_relatorio(const Relatorio *r);

/* Confere: registros ativos == celulas com 1 == vagas ocupadas, e que cada
   registro aponta para uma celula ocupada exclusiva. 1 = consistente. */
int verificar_consistencia(const Colecao *c, int mapa[FILEIRAS][COLUNAS]);

/* ===== Operacoes ===== */

/* Todas as operacoes validam ANTES de alterar qualquer coisa; se retornarem
   algo diferente de RES_OK, o mapa e a colecao ficam exatamente como estavam. */

/* operacoes.c */
Resultado cadastrar_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                            const char *id_bruto, const char *descricao,
                            int categoria, int fileira, int coluna);

/* nova_descricao == NULL mantem a descricao; nova_categoria == 0 mantem a
   categoria. */
Resultado atualizar_veiculo(Colecao *c, const char *id_bruto,
                            const char *nova_descricao, int nova_categoria);

const char *mensagem_resultado(Resultado r);

/* movimentacao.c */
Resultado mover_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                        const char *id_bruto, int nova_fileira,
                        int nova_coluna);

Resultado remover_veiculo(Colecao *c, int mapa[FILEIRAS][COLUNAS],
                          const char *id_bruto);

#endif
