#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

/* --- CONSTANTES DO SISTEMA --- */
#define FILEIRAS 4
#define COLUNAS 5
#define TOTAL_VAGAS (FILEIRAS * COLUNAS)
#define CAPACIDADE_INICIAL 2
#define TAM_ID 7
#define MAX_DESC 60

#define CATEGORIA_CARRO 1
#define CATEGORIA_MOTO 2

/* --- ESTRUTURAS DE DADOS --- */
typedef struct {
    char identificador[TAM_ID + 1]; /* 7 caracteres alfanuméricos + '\0' */
    char descricao[MAX_DESC + 1];   /* Até 60 caracteres + '\0' */
    int categoria;                  /* 1 = Carro, 2 = Moto */
    int fileira;                    /* Posição física (1 a 4) */
    int coluna;                     /* Posição física (1 a 5) */
} Veiculo;

typedef struct {
    int mapa[FILEIRAS][COLUNAS];    /* Matriz 4x5: 0 = livre, 1 = ocupada */
    Veiculo *veiculos;              /* Vetor dinâmico de veículos */
    size_t quantidade;              /* Veículos ativos cadastrados */
    size_t capacidade;              /* Capacidade alocada no vetor */
} Estacionamento;

/* --- PROTÓTIPOS DAS FUNÇÕES --- */
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

/* --- IMPLEMENTAÇÃO DAS FUNÇÕES --- */

int inicializar_estacionamento(Estacionamento *est) {
    if (est == NULL) return 0;

    for (int f = 0; f < FILEIRAS; f++) {
        for (int c = 0; c < COLUNAS; c++) {
            est->mapa[f][c] = 0;
        }
    }

    est->veiculos = (Veiculo *) malloc(CAPACIDADE_INICIAL * sizeof(Veiculo));
    if (est->veiculos == NULL) return 0;

    est->quantidade = 0;
    est->capacidade = CAPACIDADE_INICIAL;
    return 1;
}

void liberar_estacionamento(Estacionamento *est) {
    if (est != NULL && est->veiculos != NULL) {
        free(est->veiculos);
        est->veiculos = NULL;
        est->quantidade = 0;
        est->capacidade = 0;
    }
}

int validar_identificador(const char *id) {
    if (id == NULL || strlen(id) != TAM_ID) return 0;
    for (int i = 0; i < TAM_ID; i++) {
        if (!isalnum((unsigned char)id[i])) return 0;
    }
    return 1;
}

void normalizar_identificador(char *dest, const char *src) {
    if (dest == NULL || src == NULL) return;
    for (int i = 0; i < TAM_ID; i++) {
        dest[i] = (char) toupper((unsigned char)src[i]);
    }
    dest[TAM_ID] = '\0';
}

int buscar_veiculo_por_id(const Estacionamento *est, const char *id_norm) {
    if (est == NULL || id_norm == NULL) return -1;
    for (size_t i = 0; i < est->quantidade; i++) {
        if (strcmp(est->veiculos[i].identificador, id_norm) == 0) return (int) i;
    }
    return -1;
}

int cadastrar_veiculo(Estacionamento *est, const char *id, const char *desc, int cat, int fileira, int coluna) {
    if (est == NULL) return 0;

    if (est->quantidade >= TOTAL_VAGAS) {
        printf("\n[ERRO] Estacionamento totalmente lotado (%d/%d vagas).\n", TOTAL_VAGAS, TOTAL_VAGAS);
        return 0;
    }

    if (!validar_identificador(id)) {
        printf("\n[ERRO] ID inválido! Deve conter exatamente 7 caracteres alfanuméricos.\n");
        return 0;
    }

    char id_norm[TAM_ID + 1];
    normalizar_identificador(id_norm, id);

    if (buscar_veiculo_por_id(est, id_norm) != -1) {
        printf("\n[ERRO] Veículo com o ID '%s' já está cadastrado.\n", id_norm);
        return 0;
    }

    if (desc == NULL || strlen(desc) == 0 || strlen(desc) > MAX_DESC) {
        printf("\n[ERRO] Descrição inválida.\n");
        return 0;
    }

    if (cat != CATEGORIA_CARRO && cat != CATEGORIA_MOTO) {
        printf("\n[ERRO] Categoria inválida.\n");
        return 0;
    }

    if (fileira < 1 || fileira > FILEIRAS || coluna < 1 || coluna > COLUNAS) {
        printf("\n[ERRO] Posição inválida.\n");
        return 0;
    }

    int f_idx = fileira - 1;
    int c_idx = coluna - 1;

    if (est->mapa[f_idx][c_idx] != 0) {
        printf("\n[ERRO] Vaga F%d/C%d já está ocupada.\n", fileira, coluna);
        return 0;
    }

    if (est->quantidade >= est->capacidade) {
        size_t nova_capacidade = est->capacidade * 2;
        Veiculo *temp = (Veiculo *) realloc(est->veiculos, nova_capacidade * sizeof(Veiculo));
        if (temp == NULL) return 0;
        est->veiculos = temp;
        est->capacidade = nova_capacidade;
    }

    Veiculo *v = &est->veiculos[est->quantidade];
    strcpy(v->identificador, id_norm);
    strcpy(v->descricao, desc);
    v->categoria = cat;
    v->fileira = fileira;
    v->coluna = coluna;

    est->mapa[f_idx][c_idx] = 1;
    est->quantidade++;

    printf("\n[SUCESSO] Veículo '%s' cadastrado na vaga F%d/C%d.\n", v->identificador, v->fileira, v->coluna);
    return 1;
}

int consultar_veiculo(const Estacionamento *est, const char *id) {
    if (est == NULL || !validar_identificador(id)) return 0;
    char id_norm[TAM_ID + 1];
    normalizar_identificador(id_norm, id);
    int idx = buscar_veiculo_por_id(est, id_norm);
    if (idx == -1) {
        printf("\n[INFO] Veículo não localizado.\n");
        return 0;
    }
    const Veiculo *v = &est->veiculos[idx];
    printf("\n=== DETALHES DO VEÍCULO %s ===\n", v->identificador);
    printf("Identificador: %s\nDescrição    : %s\nCategoria    : %s\nPosição      : F%d/C%d\n",
           v->identificador, v->descricao, (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto", v->fileira, v->coluna);
    return 1;
}

int atualizar_veiculo(Estacionamento *est, const char *id, const char *nova_desc, int nova_cat) {
    if (est == NULL || !validar_identificador(id)) return 0;
    char id_norm[TAM_ID + 1];
    normalizar_identificador(id_norm, id);
    int idx = buscar_veiculo_por_id(est, id_norm);
    if (idx == -1) return 0;
    Veiculo *v = &est->veiculos[idx];
    strcpy(v->descricao, nova_desc);
    v->categoria = nova_cat;
    printf("\n[SUCESSO] Veículo atualizado.\n");
    return 1;
}

int mover_veiculo(Estacionamento *est, const char *id, int nova_fileira, int nova_coluna) {
    if (est == NULL || !validar_identificador(id)) return 0;
    char id_norm[TAM_ID + 1];
    normalizar_identificador(id_norm, id);
    int idx = buscar_veiculo_por_id(est, id_norm);
    if (idx == -1) return 0;
    Veiculo *v = &est->veiculos[idx];
    int dest_f = nova_fileira - 1, dest_c = nova_coluna - 1;
    if (est->mapa[dest_f][dest_c] != 0) return 0;
    est->mapa[v->fileira - 1][v->coluna - 1] = 0;
    est->mapa[dest_f][dest_c] = 1;
    v->fileira = nova_fileira;
    v->coluna = nova_coluna;
    printf("\n[SUCESSO] Veículo movido para F%d/C%d.\n", v->fileira, v->coluna);
    return 1;
}

int remover_veiculo(Estacionamento *est, const char *id) {
    if (est == NULL || !validar_identificador(id)) return 0;
    char id_norm[TAM_ID + 1];
    normalizar_identificador(id_norm, id);
    int idx = buscar_veiculo_por_id(est, id_norm);
    if (idx == -1) return 0;
    Veiculo *v = &est->veiculos[idx];
    est->mapa[v->fileira - 1][v->coluna - 1] = 0;
    for (size_t i = (size_t)idx; i < est->quantidade - 1; i++) {
        est->veiculos[i] = est->veiculos[i + 1];
    }
    est->quantidade--;
    printf("\n[SUCESSO] Veículo removido.\n");
    return 1;
}

/* FUNÇÃO CORRIGIDA: vaga_str agora é um vetor char vaga_str[16] */
void listar_veiculos(const Estacionamento *est) {
    if (est == NULL) return;

    if (est->quantidade == 0) {
        printf("\n[INFO] Nenhum veículo cadastrado no momento.\n");
        return;
    }

    printf("\n================ LISTA DE VEÍCULOS (%zu) ================\n", est->quantidade);
    printf("%-5s | %-10s | %-10s | %-8s | %-30s\n", "Nº", "ID", "CATEGORIA", "VAGA", "DESCRIÇÃO");
    printf("----------------------------------------------------------------------\n");

    for (size_t i = 0; i < est->quantidade; i++) {
        const Veiculo *v = &est->veiculos[i];
        char vaga_str[16]; /* Correção: Vetor de caracteres reservado */
        snprintf(vaga_str, sizeof(vaga_str), "F%d/C%d", v->fileira, v->coluna);
        printf("%-5zu | %-10s | %-10s | %-8s | %-30s\n",
               i + 1, v->identificador, (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto",
               vaga_str, v->descricao);
    }
    printf("================================================----------------------\n");
}

void exibir_mapa_vagas(const Estacionamento *est) {
    if (est == NULL) return;
    printf("\n=== MAPA DE VAGAS ===\n     |");
    for (int c = 1; c <= COLUNAS; c++) printf("  C%d ", c);
    printf("\n-----+-------------------\n");
    for (int f = 0; f < FILEIRAS; f++) {
        printf(" F%d  |", f + 1);
        for (int c = 0; c < COLUNAS; c++) {
            printf(est->mapa[f][c] ? "  X  " : "  .  ");
        }
        printf("\n");
    }
}

void exibir_relatorio_ocupacao(const Estacionamento *est) {
    if (est == NULL) return;
    int ocupadas = 0, carros = 0, motos = 0;
    for (int f = 0; f < FILEIRAS; f++) {
        for (int c = 0; c < COLUNAS; c++) {
            if (est->mapa[f][c] == 1) ocupadas++;
        }
    }
    for (size_t i = 0; i < est->quantidade; i++) {
        if (est->veiculos[i].categoria == CATEGORIA_CARRO) carros++;
        else if (est->veiculos[i].categoria == CATEGORIA_MOTO) motos++;
    }
    printf("\n=== RELATÓRIO DE OCUPAÇÃO ===\nTotal: %d | Ocupadas: %d | Livres: %d\nPercentual: %.2f%%\nCarros: %d | Motos: %d\n",
           TOTAL_VAGAS, ocupadas, TOTAL_VAGAS - ocupadas, ((double)ocupadas/TOTAL_VAGAS)*100.0, carros, motos);
}

void limpar_buffer_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void ler_string(char *buffer, size_t tamanho) {
    if (fgets(buffer, (int)tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        else limpar_buffer_entrada();
    }
}

int main(void) {
    Estacionamento est;
    if (!inicializar_estacionamento(&est)) return EXIT_FAILURE;

    int opcao = -1;
    char buffer_id[TAM_ID + 10], buffer_desc[MAX_DESC + 50];
    int cat, fileira, coluna;

    while (opcao != 0) {
        printf("\n=========================================\n");
        printf("   SISTEMA DE GESTÃO DE ESTACIONAMENTO   \n");
        printf("=========================================\n");
        printf("1 - Cadastrar veículo\n2 - Consultar veículo\n3 - Atualizar veículo\n4 - Mover veículo\n5 - Remover veículo\n6 - Listar veículos\n7 - Exibir mapa de vagas\n8 - Exibir relatório\n0 - Encerrar\nOpção: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer_entrada();
            continue;
        }
        limpar_buffer_entrada();

        switch (opcao) {
            case 1:
                printf("ID (7 chars): "); ler_string(buffer_id, sizeof(buffer_id));
                printf("Descrição: "); ler_string(buffer_desc, sizeof(buffer_desc));
                printf("Categoria (1=Carro, 2=Moto): "); scanf("%d", &cat); limpar_buffer_entrada();
                printf("Fileira (1-4): "); scanf("%d", &fileira); limpar_buffer_entrada();
                printf("Coluna (1-5): "); scanf("%d", &coluna); limpar_buffer_entrada();
                cadastrar_veiculo(&est, buffer_id, buffer_desc, cat, fileira, coluna);
                break;
            case 2:
                printf("ID: "); ler_string(buffer_id, sizeof(buffer_id));
                consultar_veiculo(&est, buffer_id);
                break;
            case 3:
                printf("ID: "); ler_string(buffer_id, sizeof(buffer_id));
                printf("Nova Descrição: "); ler_string(buffer_desc, sizeof(buffer_desc));
                printf("Nova Categoria: "); scanf("%d", &cat); limpar_buffer_entrada();
                atualizar_veiculo(&est, buffer_id, buffer_desc, cat);
                break;
            case 4:
                printf("ID: "); ler_string(buffer_id, sizeof(buffer_id));
                printf("Nova Fileira: "); scanf("%d", &fileira); limpar_buffer_entrada();
                printf("Nova Coluna: "); scanf("%d", &coluna); limpar_buffer_entrada();
                mover_veiculo(&est, buffer_id, fileira, coluna);
                break;
            case 5:
                printf("ID a remover: "); ler_string(buffer_id, sizeof(buffer_id));
                remover_veiculo(&est, buffer_id);
                break;
            case 6: listar_veiculos(&est); break;
            case 7: exibir_mapa_vagas(&est); break;
            case 8: exibir_relatorio_ocupacao(&est); break;
            case 0:
                liberar_estacionamento(&est);
                printf("\nEncerrando o programa.\n");
                break;
        }
    }
    return EXIT_SUCCESS;
}
📝 2. Trecho Corrigido para a Seção 3 do README.md
Substitua a Seção 3 no seu arquivo README.md por este bloco com as structs devidamente declaradas:
## 3. Estruturas de Dados Utilizadas

```c
/* Struct que representa o registro individual de um veículo */
typedef struct {
    char identificador[8]; /* 7 caracteres alfanuméricos + '\0' */
    char descricao[61];    /* Descrição com até 60 caracteres */
    int categoria;         /* 1 = Carro, 2 = Moto */
    int fileira;           /* Posição física: 1 a 4 */
    int coluna;            /* Posição física: 1 a 5 */
} Veiculo;

/* Struct principal para controle do estacionamento */
typedef struct {
    int mapa[4][5];       /* Matriz 4x5: 0 = livre, 1 = ocupada */
    Veiculo *veiculos;    /* Vetor dinâmico de veículos */
    size_t quantidade;    /* Quantidade de veículos ativos */
    size_t capacidade;    /* Capacidade alocada no vetor */
} Estacionamento;
