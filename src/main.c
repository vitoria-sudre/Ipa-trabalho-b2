#include <stdio.h>

#include "estacionamento.h"

#define BUF 128

static void exibir_menu(void)
{
    printf("\n===== ESTACIONAMENTO =====\n");
    printf("1 - Cadastrar veiculo\n");
    printf("2 - Consultar veiculo por identificador\n");
    printf("3 - Atualizar descricao ou categoria\n");
    printf("4 - Mover veiculo para outra vaga\n");
    printf("5 - Remover veiculo\n");
    printf("6 - Listar veiculos cadastrados\n");
    printf("7 - Exibir mapa de vagas\n");
    printf("8 - Exibir relatorio da ocupacao\n");
    printf("0 - Encerrar\n");
}

static void ui_cadastrar(Colecao *veiculos, int mapa[FILEIRAS][COLUNAS])
{
    char id[BUF], desc[BUF];
    int categoria, fileira, coluna;
    Resultado r;
    const Veiculo *v;

    /* Le os cinco campos e so depois chama a funcao que valida tudo. */
    if (ler_linha("Identificador (7 caracteres, A-Z e 0-9): ", id, sizeof id) ==
        LEITURA_EOF) {
        return;
    }
    if (ler_linha("Descricao (ate 60 caracteres): ", desc, sizeof desc) ==
        LEITURA_EOF) {
        return;
    }
    if (ler_inteiro("Categoria (1 = carro, 2 = moto): ", &categoria) ==
        LEITURA_EOF) {
        return;
    }
    if (ler_inteiro("Fileira (1 a 4): ", &fileira) == LEITURA_EOF) {
        return;
    }
    if (ler_inteiro("Coluna (1 a 5): ", &coluna) == LEITURA_EOF) {
        return;
    }

    r = cadastrar_veiculo(veiculos, mapa, id, desc, categoria, fileira, coluna);
    if (r != RES_OK) {
        printf("Cadastro cancelado. %s\n", mensagem_resultado(r));
        return;
    }
    v = &veiculos->dados[veiculos->quantidade - 1];
    printf("Veiculo %s cadastrado na vaga F%d/C%d.\n", v->id, v->fileira,
           v->coluna);
}

static void ui_consultar(const Colecao *veiculos)
{
    char id[BUF];
    const Veiculo *v;

    if (ler_linha("Identificador: ", id, sizeof id) == LEITURA_EOF) {
        return;
    }
    v = colecao_buscar(veiculos, id);
    if (v == NULL) {
        printf("%s\n", mensagem_resultado(RES_NAO_ENCONTRADO));
        return;
    }
    veiculo_exibir(v);
}

static void ui_atualizar(Colecao *veiculos)
{
    char id[BUF], desc[BUF];
    int categoria;
    StatusLeitura sd, sc;
    Resultado r;
    const Veiculo *v;

    if (ler_linha("Identificador: ", id, sizeof id) == LEITURA_EOF) {
        return;
    }
    v = colecao_buscar(veiculos, id);
    if (v == NULL) {
        printf("%s\n", mensagem_resultado(RES_NAO_ENCONTRADO));
        return;
    }

    sd = ler_linha("Nova descricao (Enter para manter): ", desc, sizeof desc);
    if (sd == LEITURA_EOF) {
        return;
    }
    sc = ler_inteiro("Nova categoria (1 = carro, 2 = moto, Enter para manter): ",
                     &categoria);
    if (sc == LEITURA_EOF) {
        return;
    }
    /* Enter = manter (NULL / 0). Texto que nao e numero vira -1, que a
       funcao de atualizacao rejeita como categoria invalida. */
    if (sc == LEITURA_INVALIDA) {
        categoria = -1;
    }

    r = atualizar_veiculo(veiculos, id, (sd == LEITURA_VAZIA) ? NULL : desc,
                          categoria);
    if (r != RES_OK) {
        printf("Atualizacao cancelada. %s\n", mensagem_resultado(r));
        return;
    }
    printf("Veiculo atualizado:\n");
    veiculo_exibir(colecao_buscar(veiculos, id));
}

static void ui_mover(Colecao *veiculos, int mapa[FILEIRAS][COLUNAS])
{
    char id[BUF];
    int fileira, coluna;
    Resultado r;

    if (ler_linha("Identificador: ", id, sizeof id) == LEITURA_EOF) {
        return;
    }
    if (ler_inteiro("Nova fileira (1 a 4): ", &fileira) == LEITURA_EOF) {
        return;
    }
    if (ler_inteiro("Nova coluna (1 a 5): ", &coluna) == LEITURA_EOF) {
        return;
    }

    r = mover_veiculo(veiculos, mapa, id, fileira, coluna);
    if (r != RES_OK) {
        printf("Movimentacao recusada. %s\n", mensagem_resultado(r));
        return;
    }
    printf("Veiculo movido para a vaga F%d/C%d.\n", fileira, coluna);
}

static void ui_remover(Colecao *veiculos, int mapa[FILEIRAS][COLUNAS])
{
    char id[BUF];
    Resultado r;

    if (ler_linha("Identificador: ", id, sizeof id) == LEITURA_EOF) {
        return;
    }
    r = remover_veiculo(veiculos, mapa, id);
    if (r != RES_OK) {
        printf("Remocao nao realizada. %s\n", mensagem_resultado(r));
        return;
    }
    printf("Veiculo removido e vaga liberada.\n");
}

int main(void)
{
    int mapa[FILEIRAS][COLUNAS];
    Colecao veiculos;
    Relatorio rel;
    StatusLeitura st;
    int opcao;

    mapa_inicializar(mapa);
    if (!colecao_iniciar(&veiculos)) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return 1;
    }

    for (;;) {
        exibir_menu();
        st = ler_inteiro("Escolha uma opcao: ", &opcao);
        if (st == LEITURA_EOF) {
            printf("\n");
            break;
        }
        if (st != LEITURA_OK) {
            printf("Opcao inexistente. Tente novamente.\n");
            continue;
        }
        if (opcao == 0) {
            break;
        }

        switch (opcao) {
        case 1:
            ui_cadastrar(&veiculos, mapa);
            break;
        case 2:
            ui_consultar(&veiculos);
            break;
        case 3:
            ui_atualizar(&veiculos);
            break;
        case 4:
            ui_mover(&veiculos, mapa);
            break;
        case 5:
            ui_remover(&veiculos, mapa);
            break;
        case 6:
            colecao_listar(&veiculos);
            break;
        case 7:
            mapa_exibir(mapa);
            break;
        case 8:
            calcular_relatorio(&veiculos, mapa, &rel);
            exibir_relatorio(&rel);
            break;
        default:
            printf("Opcao inexistente. Tente novamente.\n");
            break;
        }
    }

    colecao_liberar(&veiculos);
    printf("Encerrando. Memoria liberada.\n");
    return 0;
}
