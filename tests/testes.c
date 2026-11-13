
#include <stdio.h>
#include <string.h>
#include "../src/estacionamento.h"

static int verificacoes = 0;
static int falhas = 0;

#define TESTAR(condicao, nome) do { \
    verificacoes++; \
    if (condicao) { printf("[OK] %s\n", nome); } \
    else { printf("[FALHOU] %s\n", nome); falhas++; } \
} while (0)

int main(void)
{
    Colecao c;
    int mapa[FILEIRAS][COLUNAS];
    char id[TAM_ID + 1];
    char gerado[8];
    int i, categoria;
    Relatorio rel;
    Veiculo *v;

    TESTAR(normalizar_id("abc1234", id) && strcmp(id, "ABC1234") == 0,
           "Normaliza identificador para maiusculas");
    TESTAR(!normalizar_id("ABC123", id), "Recusa identificador curto");
    TESTAR(!normalizar_id("ABC12-4", id), "Recusa caractere especial");
    TESTAR(descricao_valida("carro azul"), "Aceita descricao valida");
    TESTAR(!descricao_valida(""), "Recusa descricao vazia");
    TESTAR(!descricao_valida("   "), "Recusa descricao so com espacos");
    TESTAR(categoria_valida(CATEGORIA_CARRO) &&
           categoria_valida(CATEGORIA_MOTO) && !categoria_valida(3),
           "Valida categorias");

    TESTAR(colecao_iniciar(&c), "Inicializa vetor dinamico");
    TESTAR(c.capacidade == 2 && c.quantidade == 0,
           "Capacidade inicial igual a 2");
    mapa_inicializar(mapa);
    TESTAR(mapa_contar_ocupadas(mapa) == 0, "Mapa inicia vazio");
    TESTAR(posicao_valida(1, 1) && posicao_valida(4, 5) &&
           !posicao_valida(0, 1) && !posicao_valida(1, 6),
           "Valida limites das vagas");

    TESTAR(cadastrar_veiculo(&c, mapa, "abc1234", "carro azul",
           CATEGORIA_CARRO, 1, 1) == RES_OK,
           "Cadastra veiculo valido");
    TESTAR(strcmp(c.dados[0].id, "ABC1234") == 0,
           "Normaliza identificador");
    TESTAR(cadastrar_veiculo(&c, mapa, "ABC1234", "duplicado",
           CATEGORIA_MOTO, 1, 2) == RES_ID_DUPLICADO,
           "Recusa identificador duplicado");
    TESTAR(cadastrar_veiculo(&c, mapa, "DEF5678", "moto",
           CATEGORIA_MOTO, 1, 1) == RES_VAGA_OCUPADA,
           "Recusa vaga ocupada");
    TESTAR(cadastrar_veiculo(&c, mapa, "GHI9012", "moto",
           CATEGORIA_MOTO, 5, 1) == RES_POSICAO_INVALIDA,
           "Recusa posicao invalida");

    TESTAR(cadastrar_veiculo(&c, mapa, "DEF5678", "moto",
           CATEGORIA_MOTO, 1, 2) == RES_OK,
           "Cadastra segundo veiculo");
    TESTAR(cadastrar_veiculo(&c, mapa, "GHI9012", "moto",
           CATEGORIA_MOTO, 1, 3) == RES_OK,
           "Cadastra terceiro veiculo");
    TESTAR(c.capacidade == 4, "Amplia vetor de 2 para 4");

    TESTAR(atualizar_veiculo(&c, "abc1234", "carro vermelho",
           CATEGORIA_MOTO) == RES_OK,
           "Atualiza veiculo");
    v = colecao_buscar(&c, "ABC1234");
    TESTAR(v != NULL && strcmp(v->descricao, "carro vermelho") == 0,
           "Confere atualizacao");

    TESTAR(mover_veiculo(&c, mapa, "ABC1234", 4, 5) == RES_OK,
           "Move veiculo");
    TESTAR(mapa[3][4] == VAGA_OCUPADA,
           "Confere nova vaga");

    TESTAR(remover_veiculo(&c, mapa, "ABC1234") == RES_OK,
           "Remove veiculo");
    TESTAR(colecao_buscar(&c, "ABC1234") == NULL,
           "Confere remocao");

    TESTAR(verificar_consistencia(&c, mapa),
           "Confere consistencia dos dados");

    calcular_relatorio(&c, mapa, &rel);
    TESTAR(rel.total == 20 && rel.ocupadas == 2 && rel.livres == 18,
           "Confere relatorio");

    colecao_liberar(&c);
    TESTAR(c.dados == NULL && c.quantidade == 0,
           "Libera memoria");

    TESTAR(colecao_iniciar(&c), "Reinicia colecao");
    mapa_inicializar(mapa);

    for (i = 0; i < TOTAL_VAGAS; i++) {
        snprintf(gerado, sizeof gerado, "ID%05d", i);
        categoria = (i % 2) ? CATEGORIA_MOTO : CATEGORIA_CARRO;
        if (cadastrar_veiculo(&c, mapa, gerado, "teste", categoria,
                              (i / COLUNAS) + 1, (i % COLUNAS) + 1) != RES_OK) {
            break;
        }
    }

    TESTAR(i == TOTAL_VAGAS, "Preenche as 20 vagas");
    TESTAR(c.capacidade == 32, "Vetor cresce ate capacidade 32");
    TESTAR(cadastrar_veiculo(&c, mapa, "ULT9999", "extra",
           CATEGORIA_CARRO, 1, 1) == RES_LOTADO,
           "Recusa o 21o veiculo");

    colecao_liberar(&c);

    printf("\nVerificacoes: %d | Falhas: %d\n", verificacoes, falhas);
    return falhas == 0 ? 0 : 1;
}
