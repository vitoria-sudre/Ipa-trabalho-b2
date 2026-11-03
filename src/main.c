#include <stdio.h>
#include <stdlib.h>
#include "estacionamento.h"
void exibir_menu(void) {
printf("\n=========================================\n");
printf(" SISTEMA DE GESTÃO DE ESTACIONAMENTO \n");
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
int main(void) {
Estacionamento est;
if (!inicializar_estacionamento(&est)) {
fprintf(stderr, "Erro ao inicializar sistema.\n");
return EXIT_FAILURE;
}
int opcao = -1;
char buffer_id[TAM_ID + 10];
char buffer_desc[MAX_DESC + 50];
int cat, fileira, coluna;
while (opcao != 0) {
exibir_menu();
if (scanf("%d", &opcao) != 1) {
limpar_buffer_entrada();
continue;
}
limpar_buffer_entrada();
switch (opcao) {
case 1:
printf("\n--- CADASTRO DE VEÍCULO ---\n");
printf("ID (7 chars A-Z/0-9): "); ler_string(buffer_id, sizeof(buffer_id));
printf("Descrição (até %d chars): ", MAX_DESC); ler_string(buffer_desc, sizeof(buffer_desc));
printf("Categoria (1=Carro, 2=Moto): "); scanf("%d", &cat); limpar_buffer_entrada();
printf("Fileira (1-4): "); scanf("%d", &fileira); limpar_buffer_entrada();
printf("Coluna (1-5): "); scanf("%d", &coluna); limpar_buffer_entrada();
cadastrar_veiculo(&est, buffer_id, buffer_desc, cat, fileira, coluna);
break;
case 2:
printf("\n--- CONSULTA DE VEÍCULO ---\n");
printf("ID do veículo: "); ler_string(buffer_id, sizeof(buffer_id));
consultar_veiculo(&est, buffer_id);
break;
case 3:
printf("\n--- ATUALIZAÇÃO DE VEÍCULO ---\n");
printf("ID do veículo: "); ler_string(buffer_id, sizeof(buffer_id));
printf("Nova Descrição: "); ler_string(buffer_desc, sizeof(buffer_desc));
printf("Nova Categoria (1=Carro, 2=Moto): "); scanf("%d", &cat); limpar_buffer_entrada();
atualizar_veiculo(&est, buffer_id, buffer_desc, cat);
break;
case 4:
printf("\n--- MOVIMENTAÇÃO DE VEÍCULO ---\n");
printf("ID do veículo: "); ler_string(buffer_id, sizeof(buffer_id));
printf("Nova Fileira (1-4): "); scanf("%d", &fileira); limpar_buffer_entrada();
printf("Nova Coluna (1-5): "); scanf("%d", &coluna); limpar_buffer_entrada();
mover_veiculo(&est, buffer_id, fileira, coluna);
break;
case 5:
printf("\n--- REMOÇÃO DE VEÍCULO ---\n");
printf("ID do veículo a remover: "); ler_string(buffer_id, sizeof(buffer_id));
remover_veiculo(&est, buffer_id);
break;
case 6:
listar_veiculos(&est);
break;
case 7:
exibir_mapa_vagas(&est);
break;
case 8:
exibir_relatorio_ocupacao(&est);
break;
case 0:
printf("\nEncerrando e liberando memória...\n");
liberar_estacionamento(&est);
printf("Sistema encerrado com sucesso.\n");
break;
default:
printf("\n[ERRO] Opção %d inválida.\n", opcao);
break;
}
}
return EXIT_SUCCESS;
}

