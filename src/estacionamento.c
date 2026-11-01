#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estacionamento.h"

/* Inicializar estacionamento */
int inicializar_estacionamento(Estacionamento *est) {
    if (est == NULL) {
        return 0;
    }

    /* Inicializar mapa de vagas com 0 (todas livres) */
    for (int f = 0; f < FILEIRAS; f++) {
        for (int c = 0; c < COLUNAS; c++) {
            est->mapa[f][c] = 0;
        }
    }

    /* Alocação inicial dinâmica da coleção de veículos */
    est->veiculos = (Veiculo *) malloc(CAPACIDADE_INICIAL * sizeof(Veiculo));

    if (est->veiculos == NULL) {
        perror("Erro ao alocar memória inicial");
        est->quantidade = 0;
        est->capacidade = 0;
        return 0;
    }

    est->quantidade = 0;
    est->capacidade = CAPACIDADE_INICIAL;

    return 1;
}

/* Liberar memória do estacionamento */
void liberar_estacionamento(Estacionamento *est) {
    if (est != NULL && est->veiculos != NULL) {
        free(est->veiculos);
        est->veiculos = NULL;
        est->quantidade = 0;
        est->capacidade = 0;
    }
}

/* Validar identificador */
int validar_identificador(const char *id) {
    if (id == NULL) {
        return 0;
    }

    /* Deve ter exatamente 7 caracteres */
    if (strlen(id) != TAM_ID) {
        return 0;
    }

    /* Somente letras e números são aceitos */
    for (int i = 0; i < TAM_ID; i++) {
        if (!isalnum((unsigned char) id[i])) {
            return 0;
        }
    }

    return 1;
}

/* Normalizar identificador para letras maiúsculas */
void normalizar_identificador(char *dest, const char *src) {
    if (dest == NULL || src == NULL) {
        return;
    }

    for (int i = 0; i < TAM_ID; i++) {
        dest[i] = (char) toupper((unsigned char) src[i]);
    }

    dest[TAM_ID] = '\0';
}

/* Buscar veículo pelo identificador */
int buscar_veiculo_por_id(const Estacionamento *est, const char *id_norm) {
    if (est == NULL || id_norm == NULL) {
        return -1;
    }

    for (size_t i = 0; i < est->quantidade; i++) {
        if (strcmp(est->veiculos[i].identificador, id_norm) == 0) {
            return (int) i;
        }
    }

    return -1;
}

/* Cadastrar veículo */
int cadastrar_veiculo(
    Estacionamento *est,
    const char *id,
    const char *desc,
    int cat,
    int fileira,
    int coluna
) {
    if (est == NULL) {
        return 0;
    }

    /* 1. Verificar lotação física */
    if (est->quantidade >= TOTAL_VAGAS) {
        printf("\n[ERRO] Estacionamento totalmente lotado (%d/%d vagas ocupadas).\n",
               TOTAL_VAGAS, TOTAL_VAGAS);
        return 0;
    }

    /* 2. Validar identificador */
    if (!validar_identificador(id)) {
        printf("\n[ERRO] Identificador inválido! Deve conter exatamente 7 caracteres alfanuméricos.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];

    normalizar_identificador(id_norm, id);

    /* 3. Verificar duplicidade */
    if (buscar_veiculo_por_id(est, id_norm) != -1) {
        printf("\n[ERRO] Veículo com o identificador '%s' já está cadastrado.\n",
               id_norm);
        return 0;
    }

    /* 4. Validar descrição */
    if (desc == NULL || strlen(desc) == 0 || strlen(desc) > MAX_DESC) {
        printf("\n[ERRO] Descrição inválida! Limite máximo de %d caracteres.\n",
               MAX_DESC);
        return 0;
    }

    /* 5. Validar categoria */
    if (cat != CATEGORIA_CARRO && cat != CATEGORIA_MOTO) {
        printf("\n[ERRO] Categoria inválida! Use 1 para Carro ou 2 para Moto.\n");
        return 0;
    }

    /* 6. Validar fileira e coluna */
    if (fileira < 1 || fileira > FILEIRAS ||
        coluna < 1 || coluna > COLUNAS) {
        printf("\n[ERRO] Posição inválida! Fileira de 1 a %d e coluna de 1 a %d.\n",
               FILEIRAS, COLUNAS);
        return 0;
    }

    /* Converter para índices da matriz */
    int f_idx = fileira - 1;
    int c_idx = coluna - 1;

    /* 7. Verificar se a vaga está livre */
    if (est->mapa[f_idx][c_idx] != 0) {
        printf("\n[ERRO] Vaga F%d/C%d já está ocupada.\n",
               fileira, coluna);
        return 0;
    }

    /* 8. Ampliar coleção dinâmica se necessário */
    if (est->quantidade >= est->capacidade) {
        size_t nova_capacidade = est->capacidade * 2;

        Veiculo *temp = (Veiculo *) realloc(
            est->veiculos,
            nova_capacidade * sizeof(Veiculo)
        );

        if (temp == NULL) {
            printf("\n[ERRO CRÍTICO] Falha ao ampliar memória dinâmica.\n");
            return 0;
        }

        est->veiculos = temp;
        est->capacidade = nova_capacidade;

        printf("\n[INFO] Capacidade ampliada para %zu registros.\n",
               est->capacidade);
    }

    /* 9. Inserir veículo */
    Veiculo *v = &est->veiculos[est->quantidade];

    strcpy(v->identificador, id_norm);
    strcpy(v->descricao, desc);

    v->categoria = cat;
    v->fileira = fileira;
    v->coluna = coluna;

    /* 10. Atualizar mapa de vagas */
    est->mapa[f_idx][c_idx] = 1;
    est->quantidade++;

    printf("\n[SUCESSO] Veículo '%s' (%s) cadastrado na vaga F%d/C%d.\n",
           v->identificador,
           (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto",
           v->fileira,
           v->coluna);

    return 1;
}

/* Consultar veículo */
int consultar_veiculo(const Estacionamento *est, const char *id) {
    if (est == NULL || !validar_identificador(id)) {
        printf("\n[ERRO] Identificador informado é inválido.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];

    normalizar_identificador(id_norm, id);

    int idx = buscar_veiculo_por_id(est, id_norm);

    if (idx == -1) {
        printf("\n[INFO] Veículo '%s' não localizado.\n", id_norm);
        return 0;
    }

    const Veiculo *v = &est->veiculos[idx];

    printf("\n=== DETALHES DO VEÍCULO %s ===\n", v->identificador);
    printf("Identificador: %s\n", v->identificador);
    printf("Descrição    : %s\n", v->descricao);
    printf("Categoria    : %s (%d)\n",
           (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto",
           v->categoria);
    printf("Localização  : Fileira %d, Coluna %d (F%d/C%d)\n",
           v->fileira, v->coluna, v->fileira, v->coluna);
    printf("===================================\n");

    return 1;
}

/* Atualizar veículo */
int atualizar_veiculo(
    Estacionamento *est,
    const char *id,
    const char *nova_desc,
    int nova_cat
) {
    if (est == NULL || !validar_identificador(id)) {
        printf("\n[ERRO] Identificador informado é inválido.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];

    normalizar_identificador(id_norm, id);

    int idx = buscar_veiculo_por_id(est, id_norm);

    if (idx == -1) {
        printf("\n[ERRO] Veículo '%s' não localizado para atualização.\n",
               id_norm);
        return 0;
    }

    if (nova_desc == NULL ||
        strlen(nova_desc) == 0 ||
        strlen(nova_desc) > MAX_DESC) {
        printf("\n[ERRO] Nova descrição inválida! Limite de %d caracteres.\n",
               MAX_DESC);
        return 0;
    }

    if (nova_cat != CATEGORIA_CARRO &&
        nova_cat != CATEGORIA_MOTO) {
        printf("\n[ERRO] Categoria inválida! Use 1 para Carro ou 2 para Moto.\n");
        return 0;
    }

    Veiculo *v = &est->veiculos[idx];

    strcpy(v->descricao, nova_desc);
    v->categoria = nova_cat;

    printf("\n[SUCESSO] Veículo '%s' atualizado com sucesso.\n",
           v->identificador);
    printf("Nova descrição: %s\n", v->descricao);
    printf("Categoria: %s\n",
           (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto");

    return 1;
}

/* Mover veículo de vaga */
int mover_veiculo(
    Estacionamento *est,
    const char *id,
    int nova_fileira,
    int nova_coluna
) {
    if (est == NULL || !validar_identificador(id)) {
        printf("\n[ERRO] Identificador informado é inválido.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];

    normalizar_identificador(id_norm, id);

    int idx = buscar_veiculo_por_id(est, id_norm);

    if (idx == -1) {
        printf("\n[ERRO] Veículo '%s' não localizado.\n", id_norm);
        return 0;
    }

    if (nova_fileira < 1 || nova_fileira > FILEIRAS ||
        nova_coluna < 1 || nova_coluna > COLUNAS) {
        printf("\n[ERRO] Posição de destino inválida!\n");
        return 0;
    }

    Veiculo *v = &est->veiculos[idx];

    /* Verificar se a posição é a mesma */
    if (v->fileira == nova_fileira &&
        v->coluna == nova_coluna) {
        printf("\n[AVISO] O veículo já está nessa vaga.\n");
        return 1;
    }

    int dest_f = nova_fileira - 1;
    int dest_c = nova_coluna - 1;

    /* Verificar se a vaga de destino está ocupada */
    if (est->mapa[dest_f][dest_c] != 0) {
        printf("\n[ERRO] Vaga F%d/C%d já está ocupada.\n",
               nova_fileira, nova_coluna);
        return 0;
    }

    /* Atualizar mapa */
    int orig_f = v->fileira - 1;
    int orig_c = v->coluna - 1;

    est->mapa[orig_f][orig_c] = 0;
    est->mapa[dest_f][dest_c] = 1;

    /* Atualizar localização do veículo */
    int f_antiga = v->fileira;
    int c_antiga = v->coluna;

    v->fileira = nova_fileira;
    v->coluna = nova_coluna;

    printf("\n[SUCESSO] Veículo '%s' movido de F%d/C%d para F%d/C%d.\n",
           v->identificador,
           f_antiga,
           c_antiga,
           v->fileira,
           v->coluna);

    return 1;
}

/* Remover veículo */
int remover_veiculo(Estacionamento *est, const char *id) {
    if (est == NULL || !validar_identificador(id)) {
        printf("\n[ERRO] Identificador informado é inválido.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];

    normalizar_identificador(id_norm, id);

    int idx = buscar_veiculo_por_id(est, id_norm);

    if (idx == -1) {
        printf("\n[ERRO] Veículo '%s' não localizado para remoção.\n",
               id_norm);
        return 0;
    }

    Veiculo *v = &est->veiculos[idx];

    /* Liberar vaga na matriz */
    int f_idx = v->fileira - 1;
    int c_idx = v->coluna - 1;

    est->mapa[f_idx][c_idx] = 0;

    /* Remover veículo e ajustar vetor */
    for (size_t i = (size_t) idx;
         i < est->quantidade - 1;
         i++) {
        est->veiculos[i] = est->veiculos[i + 1];
    }

    est->quantidade--;

    printf("\n[SUCESSO] Veículo '%s' removido.\n", id_norm);
    printf("Vaga F%d/C%d liberada.\n", f_idx + 1, c_idx + 1);

    return 1;
}

/* Listar todos os veículos */
void listar_veiculos(const Estacionamento *est) {
    if (est == NULL) {
        return;
    }

    if (est->quantidade == 0) {
        printf("\n[INFO] Nenhum veículo cadastrado.\n");
        return;
    }

    printf("\n================ LISTA DE VEÍCULOS (%zu) ================\n",
           est->quantidade);

    printf("%-5s | %-10s | %-10s | %-8s | %-30s\n",
           "Nº", "ID", "CATEGORIA", "VAGA", "DESCRIÇÃO");

    printf("--------------------------------------------------------------------------\n");

    for (size_t i = 0; i < est->quantidade; i++) {
        const Veiculo *v = &est->veiculos[i];

        char vaga_str[10];

        snprintf(vaga_str, sizeof(vaga_str),
                 "F%d/C%d", v->fileira, v->coluna);

        printf("%-5zu | %-10s | %-10s | %-8s | %-30s\n",
               i + 1,
               v->identificador,
               (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto",
               vaga_str,
               v->descricao);
    }

    printf("==========================================================================\n");
}

/* Exibir mapa de vagas */
void exibir_mapa_vagas(const Estacionamento *est) {
    if (est == NULL) {
        return;
    }

    printf("\n=================== MAPA DE VAGAS ===================\n");

    printf("     |");

    for (int c = 1; c <= COLUNAS; c++) {
        printf("  C%d ", c);
    }

    printf("\n-----+-----------------------------------\n");

    for (int f = 0; f < FILEIRAS; f++) {
        printf(" F%d  |", f + 1);

        for (int c = 0; c < COLUNAS; c++) {
            if (est->mapa[f][c] == 1) {
                printf("  X  ");
            } else {
                printf("  .  ");
            }
        }

        printf("\n");
    }

    printf("-----------------------------------------------------\n");
    printf("Legenda: X = Vaga Ocupada | . = Vaga Livre\n");
    printf("=====================================================\n");
}

/* Exibir relatório de ocupação */
void exibir_relatorio_ocupacao(const Estacionamento *est) {
    if (est == NULL) {
        return;
    }

    int vagas_ocupadas = 0;
    int vagas_livres = 0;
    int total_carros = 0;
    int total_motos = 0;

    /* Contagem das vagas na matriz */
    for (int f = 0; f < FILEIRAS; f++) {
        for (int c = 0; c < COLUNAS; c++) {
            if (est->mapa[f][c] == 1) {
                vagas_ocupadas++;
            } else {
                vagas_livres++;
            }
        }
    }

    /* Contagem por categoria */
    for (size_t i = 0; i < est->quantidade; i++) {
        if (est->veiculos[i].categoria == CATEGORIA_CARRO) {
            total_carros++;
        } else if (est->veiculos[i].categoria == CATEGORIA_MOTO) {
            total_motos++;
        }
    }

    double pct_ocupacao =
        ((double) vagas_ocupadas / (double) TOTAL_VAGAS) * 100.0;

    printf("\n================ RELATÓRIO DE OCUPAÇÃO ================\n");

    printf("Total de Vagas Físicas      : %d\n", TOTAL_VAGAS);
    printf("Vagas Ocupadas              : %d\n", vagas_ocupadas);
    printf("Vagas Livres                : %d\n", vagas_livres);
    printf("Percentual de Ocupação      : %.2f%%\n", pct_ocupacao);
    printf("Quantidade de Carros        : %d\n", total_carros);
    printf("Quantidade de Motos         : %d\n", total_motos);
    printf("Capacidade Atual do Vetor   : %zu registros\n",
           est->capacidade);

    printf("Consistência Registros/Vagas: %s\n",
           ((size_t) vagas_ocupadas == est->quantidade)
           ? "OK (Consistente)"
           : "ERRO (Inconsistente)");

    printf("=======================================================\n");
}

/* Limpar buffer de entrada */
void limpar_buffer_entrada(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* Limpar caracteres restantes */
    }
}

/* Ler string com segurança */
void ler_string(char *buffer, size_t tamanho) {
    if (buffer == NULL || tamanho == 0) {
        return;
    }

    if (fgets(buffer, (int) tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            limpar_buffer_entrada();
        }
    }
}
