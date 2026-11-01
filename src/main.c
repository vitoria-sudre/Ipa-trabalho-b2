#include <stdio.h>
#include <stdlib.h>

#include "estacionamento.h"

/* Exibir menu principal */
void exibir_menu(void) {
    printf("\n=========================================\n");
    printf(" SISTEMA DE GESTÃO DE ESTACIONAMENTO\n");
    printf("=========================================\n");
    printf("1 - Cadastrar veículo\n");
    printf("2 - Consultar veículo por identificador\n");
    printf("3 - Atualizar descrição ou categoria\n");
    printf("4 - Mover veículo para outra vaga\n");
    printf("5 - Remover veículo\n");
    printf("6 - Listar veículos cadastrados\n");
    printf("7 - Exibir mapa de vagas\n");
    printf("8 - Exibir relatório da ocupação\n");
    printf("0 - Encerrar\n");
    printf("=========================================\n");
    printf("Escolha uma opção: ");
}

/* Função principal */
int main(void) {
    Estacionamento est;

    /* Inicializar estacionamento */
    if (!inicializar_estacionamento(&est)) {
        fprintf(stderr,
                "Erro fatal ao inicializar o sistema de estacionamento.\n");
        return EXIT_FAILURE;
    }

    int opcao = -1;

    char buffer_id[TAM_ID + 10];
    char buffer_desc[MAX_DESC + 50];

    int cat, fileira, coluna;

    /* Loop principal do sistema */
    while (opcao != 0) {
        exibir_menu();

        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opção inválida! Digite um número.\n");
            limpar_buffer_entrada();
            continue;
        }

        limpar_buffer_entrada();

        switch (opcao) {

            /* Cadastrar veículo */
            case 1:
                printf("\n--- CADASTRO DE VEÍCULO ---\n");

                printf("Identificador (7 caracteres A-Z/0-9): ");
                ler_string(buffer_id, sizeof(buffer_id));

                printf("Descrição (até %d caracteres): ", MAX_DESC);
                ler_string(buffer_desc, sizeof(buffer_desc));

                printf("Categoria (1 = Carro, 2 = Moto): ");

                if (scanf("%d", &cat) != 1) {
                    printf("\n[ERRO] Entrada inválida para categoria.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                printf("Fileira (1 a %d): ", FILEIRAS);

                if (scanf("%d", &fileira) != 1) {
                    printf("\n[ERRO] Entrada inválida para fileira.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                printf("Coluna (1 a %d): ", COLUNAS);

                if (scanf("%d", &coluna) != 1) {
                    printf("\n[ERRO] Entrada inválida para coluna.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                cadastrar_veiculo(
                    &est,
                    buffer_id,
                    buffer_desc,
                    cat,
                    fileira,
                    coluna
                );

                break;

            /* Consultar veículo */
            case 2:
                printf("\n--- CONSULTA DE VEÍCULO ---\n");

                printf("Digite o identificador do veículo: ");
                ler_string(buffer_id, sizeof(buffer_id));

                consultar_veiculo(&est, buffer_id);

                break;

            /* Atualizar veículo */
            case 3:
                printf("\n--- ATUALIZAÇÃO DE VEÍCULO ---\n");

                printf("Digite o identificador do veículo: ");
                ler_string(buffer_id, sizeof(buffer_id));

                printf("Nova descrição (até %d caracteres): ", MAX_DESC);
                ler_string(buffer_desc, sizeof(buffer_desc));

                printf("Nova categoria (1 = Carro, 2 = Moto): ");

                if (scanf("%d", &cat) != 1) {
                    printf("\n[ERRO] Entrada inválida para categoria.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                atualizar_veiculo(
                    &est,
                    buffer_id,
                    buffer_desc,
                    cat
                );

                break;

            /* Mover veículo */
            case 4:
                printf("\n--- MOVIMENTAÇÃO DE VEÍCULO ---\n");

                printf("Digite o identificador do veículo: ");
                ler_string(buffer_id, sizeof(buffer_id));

                printf("Nova fileira (1 a %d): ", FILEIRAS);

                if (scanf("%d", &fileira) != 1) {
                    printf("\n[ERRO] Entrada inválida para fileira.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                printf("Nova coluna (1 a %d): ", COLUNAS);

                if (scanf("%d", &coluna) != 1) {
                    printf("\n[ERRO] Entrada inválida para coluna.\n");
                    limpar_buffer_entrada();
                    break;
                }

                limpar_buffer_entrada();

                mover_veiculo(
                    &est,
                    buffer_id,
                    fileira,
                    coluna
                );

                break;

            /* Remover veículo */
            case 5:
                printf("\n--- REMOÇÃO DE VEÍCULO ---\n");

                printf("Digite o identificador do veículo a remover: ");
                ler_string(buffer_id, sizeof(buffer_id));

                remover_veiculo(&est, buffer_id);

                break;

            /* Listar veículos */
            case 6:
                listar_veiculos(&est);
                break;

            /* Exibir mapa */
            case 7:
                exibir_mapa_vagas(&est);
                break;

            /* Relatório de ocupação */
            case 8:
                exibir_relatorio_ocupacao(&est);
                break;

            /* Encerrar sistema */
            case 0:
                printf("\nEncerrando o sistema e liberando recursos da memória...\n");

                liberar_estacionamento(&est);

                printf("Sistema encerrado com sucesso.\n");
                break;

            /* Opção inválida */
            default:
                printf("\n[ERRO] Opção %d inválida! Escolha de 0 a 8.\n",
                       opcao);
                break;
        }
    }

    return EXIT_SUCCESS;

}
