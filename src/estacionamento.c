#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estacionamento.h"
int inicializar_estacionamento(Estacionamento *est) {
if (est == NULL) return 0;
for (int f = 0; f < FILEIRAS; f++) {
for (int c = 0; c < COLUNAS; c++) {
est->mapa[f][c] = 0;
}
}
est->veiculos = (Veiculo *) malloc(CAPACIDADE_INICIAL * sizeof(Veiculo));
if (est->veiculos == NULL) {
perror("Erro ao alocar memória inicial");
return 0;
}
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
if (strcmp(est->veiculos[i].identificador, id_norm) == 0) {
return (int) i;
}
}
return -1;
}
int cadastrar_veiculo(Estacionamento *est, const char *id, const char *desc, int cat, int fileira, int coluna
) {
if (est == NULL) return 0;
if (est->quantidade >= TOTAL_VAGAS) {
printf("\n[ERRO] Estacionamento totalmente lotado (%d/%d vagas).\n", TOTAL_VAGAS, TOTAL_VAGAS);
return 0;
}
if (!validar_identificador(id)) {
printf("\n[ERRO] Identificador inválido! Deve conter exatamente 7 caracteres alfanuméricos.\n");
return 0;
}
char id_norm[TAM_ID + 1];
normalizar_identificador(id_norm, id);
if (buscar_veiculo_por_id(est, id_norm) != -1) {
printf("\n[ERRO] Veículo com o identificador '%s' já está cadastrado.\n", id_norm);
return 0;
}
if (desc == NULL || strlen(desc) == 0 || strlen(desc) > MAX_DESC) {
printf("\n[ERRO] Descrição inválida.\n");
return 0;
}
if (cat != CATEGORIA_CARRO && cat != CATEGORIA_MOTO) {
printf("\n[ERRO] Categoria inválida (1 = Carro, 2 = Moto).\n");
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
if (temp == NULL) {
printf("\n[ERRO CRÍTICO] Falha ao alocar memória.\n");
return 0;
}
est->veiculos = temp;
est->capacidade = nova_capacidade;
printf("\n[INFO] Capacidade do vetor ampliada para %zu registros.\n", est->capacidade);
}
Veiculo *v = &est->veiculos[est->quantidade];
strcpy(v->identificador, id_norm);
strcpy(v->descricao, desc);
v->categoria = cat;
v->fileira = fileira;
v->coluna = coluna;
est->mapa[f_idx][c_idx] = 1;
est->quantidade++;
printf("\n[SUCESSO] Veículo '%s' cadastrado na vaga F%d/C%d.\n", v->identificador, v->fileira, v->coluna)
;
return 1;
}
int consultar_veiculo(const Estacionamento *est, const char *id) {
if (est == NULL || !validar_identificador(id)) {
printf("\n[ERRO] Identificador inválido.\n");
return 0;
}
char id_norm[TAM_ID + 1];
normalizar_identificador(id_norm, id);
int idx = buscar_veiculo_por_id(est, id_norm);
if (idx == -1) {
printf("\n[INFO] Veículo não localizado.\n");
return 0;
}
const Veiculo *v = &est->veiculos[idx];
printf("\n=== DETALHES DO VEÍCULO %s ===\n", v->identificador);
printf("Identificador: %s\n", v->identificador);
printf("Descrição : %s\n", v->descricao);
printf("Categoria : %s (%d)\n", (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto", v->categoria);
printf("Localização : Fileira %d, Coluna %d (F%d/C%d)\n", v->fileira, v->coluna, v->fileira, v->coluna);
printf("===================================\n");
return 1;
}
int atualizar_veiculo(Estacionamento *est, const char *id, const char *nova_desc, int nova_cat) {
if (est == NULL || !validar_identificador(id)) return 0;
char id_norm[TAM_ID + 1];
normalizar_identificador(id_norm, id);
int idx = buscar_veiculo_por_id(est, id_norm);
if (idx == -1) {
printf("\n[ERRO] Veículo '%s' não localizado.\n", id_norm);
return 0;
}
if (nova_desc == NULL || strlen(nova_desc) == 0 || strlen(nova_desc) > MAX_DESC ||
(nova_cat != CATEGORIA_CARRO && nova_cat != CATEGORIA_MOTO)) {
printf("\n[ERRO] Dados inválidos para atualização.\n");
return 0;
}
Veiculo *v = &est->veiculos[idx];
strcpy(v->descricao, nova_desc);
v->categoria = nova_cat;
printf("\n[SUCESSO] Veículo '%s' atualizado.\n", v->identificador);
return 1;
}
int mover_veiculo(Estacionamento *est, const char *id, int nova_fileira, int nova_coluna) {
if (est == NULL || !validar_identificador(id)) return 0;
char id_norm[TAM_ID + 1];
normalizar_identificador(id_norm, id);
int idx = buscar_veiculo_por_id(est, id_norm);
if (idx == -1) return 0;
if (nova_fileira < 1 || nova_fileira > FILEIRAS || nova_coluna < 1 || nova_coluna > COLUNAS) return 0;
Veiculo *v = &est->veiculos[idx];
if (v->fileira == nova_fileira && v->coluna == nova_coluna) {
printf("\n[AVISO] Veículo já está nesta vaga.\n");
return 1;
}
int dest_f = nova_fileira - 1;
int dest_c = nova_coluna - 1;
if (est->mapa[dest_f][dest_c] != 0) {
printf("\n[ERRO] Vaga de destino ocupada.\n");
return 0;
}
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
printf("\n[SUCESSO] Veículo '%s' removido.\n", id_norm);
return 1;
}
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
char vaga_str[16];
snprintf(vaga_str, sizeof(vaga_str), "F%d/C%d", v->fileira, v->coluna);
printf("%-5zu | %-10s | %-10s | %-8s | %-30s\n",
i + 1, v->identificador, (v->categoria == CATEGORIA_CARRO) ? "Carro" : "Moto",
vaga_str, v->descricao);
}
printf("================================================----------------------\n");
}
void exibir_mapa_vagas(const Estacionamento *est) {
if (est == NULL) return;
printf("\n=== MAPA DE VAGAS ===\n |");
for (int c = 1; c <= COLUNAS; c++) printf(" C%d ", c);
printf("\n-----+-------------------\n");
for (int f = 0; f < FILEIRAS; f++) {
printf(" F%d |", f + 1);
for (int c = 0; c < COLUNAS; c++) {
printf(est->mapa[f][c] ? " X " : " . ");
}
printf("\n");
}
printf("---------------------\n");
printf("Legenda: X = Ocupada | . = Livre\n");
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
printf("\n=== RELATÓRIO DE OCUPAÇÃO ===\n");
printf("Total de Vagas Físicas : %d\n", TOTAL_VAGAS);
printf("Vagas Ocupadas : %d\n", ocupadas);
printf("Vagas Livres : %d\n", TOTAL_VAGAS - ocupadas);
printf("Percentual de Ocupação : %.2f%%\n", ((double)ocupadas / TOTAL_VAGAS) * 100.0);
printf("Quantidade de Carros : %d\n", carros);
printf("Quantidade de Motos : %d\n", motos);
printf("Capacidade Atual Vetor : %zu registros\n", est->capacidade);
printf("Consistência Registros/Vagas: %s\n",
((size_t)ocupadas == est->quantidade) ? "OK (Consistente)" : "ERRO (Inconsistente)");
printf("========================================\n");
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
