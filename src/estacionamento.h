#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stddef.h>

#define FILEIRAS 4
#define COLUNAS 5
#define TOTAL_VAGAS (FILEIRAS * COLUNAS)
#define CAPACIDADE_INICIAL 2
#define TAM_ID 7
#define MAX_DESC 60

/* Categorias de veículo */
#define CATEGORIA_CARRO 1
#define CATEGORIA_MOTO 2

/* Struct para registro de veículo */
typedef struct {
    char identificador[TAM_ID + 1]; /* 7 chars alfanuméricos + '\0' */
    char descricao[MAX_DESC + 1];   /* Até 60 chars + '\0' */
    int categoria;                  /* 1 = Carro, 2 = Moto */
    int fileira;                    /* 1 a 4 (exibição para usuário) */
    int coluna;                     /* 1 a 5 (exibição para usuário) */
} Veiculo;

/* Struct principal para controle do estacionamento */
typedef struct {
    int mapa[FILEIRAS][COLUNAS];  /* 0 = livre, 1 = ocupada */
    Veiculo *veiculos;             /* Vetor dinâmico de veículos */
    size_t quantidade;            /* Quantidade de veículos ativos */
    size_t capacidade;            /* Capacidade total reservada no vetor */
} Estacionamento;

/* Protótipos das funções */
int inicializar_estacionamento(Estacionamento *est);
void liberar_estacionamento(Estacionamento *est);

int validar_identificador(const char *id);
void normalizar_identificador(char *dest, const char *src);
int buscar_veiculo_por_id(const Estacionamento *est, const char *id_norm);

int cadastrar_veiculo(Estacionamento *est, const char *id, const char *desc, int cat, int fileira, int coluna);
int consultar_veiculo(const Estacionamento *est, const char *id);
int atualizar_veiculo(Estacionamento *est, const char *id, const char *nova_desc, int nova_cat);
int mover_veiculo(Estacionamento *est, const char *id, int nova_fileira, int nova_coluna);
int remover_veiculo(Estacionamento *est, const char *id);
void listar_veiculos(const Estacionamento *est);
void exibir_mapa_vagas(const Estacionamento *est);
void exibir_relatorio_ocupacao(const Estacionamento *est);

void ler_string(char *buffer, size_t tamanho);
void limpar_buffer_entrada(void);

#endif /* ESTACIONAMENTO_H */
