#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "quadras.h"
#include "entrada.h"
#include "sistema.h"

int buscar_quadra_por_id(const Sistema *sistema, int id) {
    size_t i;
    for (i = 0; i < sistema->total_quadras; i++) {
        if (sistema->quadras[i].id == id) return (int)i;
    }
    return -1;
}

int buscar_quadra_por_codigo(const Sistema *sistema, const char *codigo) {
    size_t i;
    for (i = 0; i < sistema->total_quadras; i++) {
        if (strcmp(sistema->quadras[i].codigo, codigo) == 0) return (int)i;
    }
    return -1;
}

static int preparar_quadra(Sistema *sistema, Quadra *quadra, int nova) {
    if (!ler_texto("Codigo unico da quadra: ", quadra->codigo, TAM_CODIGO_QUADRA) ||
        !ler_texto("Nome/tipo: ", quadra->nome, TAM_NOME) ||
        !ler_real("Preco por hora: R$ ", &quadra->preco_hora)) {
        return 0;
    }
    {
        int encontrado = buscar_quadra_por_codigo(sistema, quadra->codigo);
        if (encontrado >= 0 &&
            (nova || sistema->quadras[encontrado].id != quadra->id)) {
            printf("[ERRO] Ja existe quadra com esse codigo.\n");
            return 0;
        }
    }
    return 1;
}

int cadastrar_quadra(Sistema *sistema) {
    Quadra nova = {0};
    if (sistema->proximo_id_quadra == INT_MAX) {
        printf("[ERRO] Limite de identificadores de quadras atingido.\n");
        return 0;
    }
    printf("\n+---------------- NOVA QUADRA -----------------+\n");
    if (!preparar_quadra(sistema, &nova, 1) ||
        !sistema_ajustar_capacidade(sistema, 2, sistema->total_quadras + 1)) {
        return 0;
    }
    nova.id = sistema->proximo_id_quadra++;
    sistema->quadras[sistema->total_quadras++] = nova;
    printf("[OK] Quadra cadastrada com ID %d.\n", nova.id);
    return 1;
}

void listar_quadras(const Sistema *sistema) {
    size_t i;
    printf("\n+---------------- QUADRAS ---------------------+\n");
    if (sistema->total_quadras == 0) {
        printf("Nenhuma quadra cadastrada.\n");
        return;
    }
    for (i = 0; i < sistema->total_quadras; i++) {
        const Quadra *quadra = &sistema->quadras[i];
        printf("ID: %d | Codigo: %s | Nome: %s | Preco/hora: R$ %.2f\n",
               quadra->id, quadra->codigo, quadra->nome, quadra->preco_hora);
    }
}

static int escolher_quadra(const Sistema *sistema) {
    int opcao;
    int id;
    char valor[TAM_NOME];

    printf("\nBuscar quadra por:\n1 - ID\n2 - Codigo\n3 - Nome exato\n0 - Cancelar\n");
    if (!ler_inteiro("Opcao: ", &opcao)) return -1;
    switch (opcao) {
        case 1:
            if (!ler_inteiro("ID: ", &id)) return -1;
            return buscar_quadra_por_id(sistema, id);
        case 2:
            if (!ler_texto("Codigo: ", valor, TAM_NOME)) return -1;
            return buscar_quadra_por_codigo(sistema, valor);
        case 3: {
            size_t i;
            if (!ler_texto("Nome exato: ", valor, TAM_NOME)) return -1;
            for (i = 0; i < sistema->total_quadras; i++) {
                if (strcmp(sistema->quadras[i].nome, valor) == 0) return (int)i;
            }
            return -1;
        }
        default:
            return -1;
    }
}

int consultar_quadra(const Sistema *sistema) {
    int indice = escolher_quadra(sistema);
    if (indice < 0) {
        printf("[INFO] Quadra nao encontrada ou consulta cancelada.\n");
        return 0;
    }
    {
        const Quadra *quadra = &sistema->quadras[indice];
        printf("ID: %d | Codigo: %s | Nome: %s | Preco/hora: R$ %.2f\n",
               quadra->id, quadra->codigo, quadra->nome, quadra->preco_hora);
    }
    return 0;
}

int atualizar_quadra(Sistema *sistema) {
    int indice = escolher_quadra(sistema);
    Quadra atualizada;
    if (indice < 0) {
        printf("[ERRO] Quadra nao encontrada.\n");
        return 0;
    }
    atualizada = sistema->quadras[indice];
    printf("Informe os novos dados (codigo deve continuar unico).\n");
    if (!preparar_quadra(sistema, &atualizada, 0)) return 0;
    sistema->quadras[indice] = atualizada;
    printf("[OK] Quadra atualizada.\n");
    return 1;
}

int remover_quadra(Sistema *sistema) {
    int indice = escolher_quadra(sistema);
    size_t i;
    if (indice < 0) {
        printf("[ERRO] Quadra nao encontrada.\n");
        return 0;
    }
    for (i = 0; i < sistema->total_reservas; i++) {
        if (sistema->reservas[i].id_quadra == sistema->quadras[indice].id) {
            printf("[ERRO] Quadra associada a reservas; remova-as antes.\n");
            return 0;
        }
    }
    if (!sistema_ajustar_capacidade(sistema, 2, sistema->total_quadras - 1)) {
        printf("[ERRO] Remocao cancelada: nao foi possivel ajustar a memoria.\n");
        return 0;
    }
    memmove(&sistema->quadras[indice], &sistema->quadras[indice + 1],
            (sistema->total_quadras - (size_t)indice - 1) * sizeof(Quadra));
    sistema->total_quadras--;
    printf("[OK] Quadra removida.\n");
    return 1;
}
