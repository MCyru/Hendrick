#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "reservas.h"
#include "clientes.h"
#include "quadras.h"
#include "entrada.h"
#include "sistema.h"

int buscar_reserva_por_id(const Sistema *sistema, int id) {
    size_t i;
    for (i = 0; i < sistema->total_reservas; i++) {
        if (sistema->reservas[i].id == id) return (int)i;
    }
    return -1;
}

static int horario_disponivel(const Sistema *sistema, int id_quadra,
                              const char data[TAM_DATA], int hora, int ignorar_id) {
    size_t i;
    for (i = 0; i < sistema->total_reservas; i++) {
        const Reserva *reserva = &sistema->reservas[i];
        if (reserva->id != ignorar_id && reserva->id_quadra == id_quadra &&
            reserva->hora == hora && strcmp(reserva->data, data) == 0) {
            return 0;
        }
    }
    return 1;
}

static int ler_dados_reserva(Sistema *sistema, Reserva *reserva, int id_ignorado) {
    int id_cliente;
    int id_quadra;

    listar_clientes(sistema);
    if (!ler_inteiro("ID do cliente: ", &id_cliente) ||
        buscar_cliente_por_id(sistema, id_cliente) < 0) {
        printf("[ERRO] Cliente nao encontrado.\n");
        return 0;
    }

    listar_quadras(sistema);
    if (!ler_inteiro("ID da quadra: ", &id_quadra) ||
        buscar_quadra_por_id(sistema, id_quadra) < 0) {
        printf("[ERRO] Quadra nao encontrada.\n");
        return 0;
    }
    if (!ler_data("Data (DD/MM/AAAA): ", reserva->data) ||
        !ler_inteiro("Horario (0 a 23): ", &reserva->hora)) {
        return 0;
    }
    if (reserva->hora < 0 || reserva->hora > 23) {
        printf("[ERRO] Horario deve estar entre 0 e 23.\n");
        return 0;
    }
    if (!horario_disponivel(sistema, id_quadra, reserva->data,
                            reserva->hora, id_ignorado)) {
        printf("[ERRO] Quadra ja reservada nessa data e horario.\n");
        return 0;
    }
    reserva->id_cliente = id_cliente;
    reserva->id_quadra = id_quadra;
    return 1;
}

int cadastrar_reserva(Sistema *sistema) {
    Reserva nova = {0};

    if (sistema->proximo_id_reserva == INT_MAX) {
        printf("[ERRO] Limite de identificadores de reservas atingido.\n");
        return 0;
    }
    if (!ler_dados_reserva(sistema, &nova, -1) ||
        !sistema_ajustar_capacidade(sistema, 3, sistema->total_reservas + 1)) {
        return 0;
    }
    nova.id = sistema->proximo_id_reserva++;
    sistema->reservas[sistema->total_reservas++] = nova;
    printf("[OK] Reserva cadastrada com ID %d.\n", nova.id);
    return 1;
}

static int escolher_reserva(const Sistema *sistema) {
    int opcao;
    int id;
    char data[TAM_DATA];
    int id_cliente;
    int id_quadra;
    size_t i;

    printf("\nLocalizar reserva por:\n1 - ID\n2 - Data\n3 - ID do cliente\n4 - ID da quadra\n0 - Cancelar\n");
    if (!ler_inteiro("Opcao: ", &opcao)) return -1;
    if (opcao == 1) {
        if (!ler_inteiro("ID da reserva: ", &id)) return -1;
        return buscar_reserva_por_id(sistema, id);
    }
    if (opcao == 2) {
        if (!ler_data("Data (DD/MM/AAAA): ", data)) return -1;
        for (i = 0; i < sistema->total_reservas; i++) {
            if (strcmp(sistema->reservas[i].data, data) == 0) return (int)i;
        }
    } else if (opcao == 3) {
        if (!ler_inteiro("ID do cliente: ", &id_cliente)) return -1;
        for (i = 0; i < sistema->total_reservas; i++) {
            if (sistema->reservas[i].id_cliente == id_cliente) return (int)i;
        }
    } else if (opcao == 4) {
        if (!ler_inteiro("ID da quadra: ", &id_quadra)) return -1;
        for (i = 0; i < sistema->total_reservas; i++) {
            if (sistema->reservas[i].id_quadra == id_quadra) return (int)i;
        }
    }
    return -1;
}

static void mostrar_reserva(const Sistema *sistema, const Reserva *reserva) {
    int indice_cliente = buscar_cliente_por_id(sistema, reserva->id_cliente);
    int indice_quadra = buscar_quadra_por_id(sistema, reserva->id_quadra);
    printf("Reserva ID: %d | Data: %s | Horario: %02dh00 | Cliente: %s (ID %d) | "
           "Quadra: %s (ID %d)\n",
           reserva->id, reserva->data, reserva->hora,
           indice_cliente >= 0 ? sistema->clientes[indice_cliente].nome : "INVALIDO",
           reserva->id_cliente,
           indice_quadra >= 0 ? sistema->quadras[indice_quadra].nome : "INVALIDO",
           reserva->id_quadra);
}

int consultar_reserva(const Sistema *sistema) {
    int indice = escolher_reserva(sistema);
    if (indice < 0) {
        printf("[INFO] Reserva nao encontrada.\n");
        return 0;
    }
    mostrar_reserva(sistema, &sistema->reservas[indice]);
    return 0;
}

int atualizar_reserva(Sistema *sistema) {
    int indice = escolher_reserva(sistema);
    Reserva atualizada;
    if (indice < 0) {
        printf("[ERRO] Reserva nao encontrada.\n");
        return 0;
    }
    atualizada = sistema->reservas[indice];
    printf("Informe os novos dados do relacionamento:\n");
    if (!ler_dados_reserva(sistema, &atualizada, atualizada.id)) return 0;
    sistema->reservas[indice] = atualizada;
    printf("[OK] Reserva atualizada.\n");
    return 1;
}

int remover_reserva(Sistema *sistema) {
    int indice = escolher_reserva(sistema);
    if (indice < 0) {
        printf("[ERRO] Reserva nao encontrada.\n");
        return 0;
    }
    if (!sistema_ajustar_capacidade(sistema, 3, sistema->total_reservas - 1)) {
        printf("[ERRO] Remocao cancelada: nao foi possivel ajustar a memoria.\n");
        return 0;
    }
    memmove(&sistema->reservas[indice], &sistema->reservas[indice + 1],
            (sistema->total_reservas - (size_t)indice - 1) * sizeof(Reserva));
    sistema->total_reservas--;
    printf("[OK] Reserva removida.\n");
    return 1;
}

void listar_reservas(const Sistema *sistema) {
    size_t i;
    printf("\n+---------------- RESERVAS --------------------+\n");
    if (sistema->total_reservas == 0) {
        printf("Nenhuma reserva cadastrada.\n");
        return;
    }
    for (i = 0; i < sistema->total_reservas; i++) {
        mostrar_reserva(sistema, &sistema->reservas[i]);
    }
}

void filtrar_reservas(const Sistema *sistema) {
    int opcao;
    char valor[TAM_NOME];
    char data[TAM_DATA];
    size_t i;
    int encontrados = 0;

    printf("\nFiltrar reservas por:\n1 - Data a partir de\n2 - Nome do cliente\n3 - Nome da quadra\n");
    if (!ler_inteiro("Opcao: ", &opcao)) return;
    if (opcao == 1) {
        if (!ler_data("Data minima (DD/MM/AAAA): ", data)) return;
    } else if (opcao == 2) {
        if (!ler_texto("Nome do cliente: ", valor, TAM_NOME)) return;
    } else if (opcao == 3) {
        if (!ler_texto("Nome da quadra: ", valor, TAM_NOME)) return;
    } else {
        printf("[ERRO] Opcao invalida.\n");
        return;
    }

    for (i = 0; i < sistema->total_reservas; i++) {
        const Reserva *reserva = &sistema->reservas[i];
        int cliente = buscar_cliente_por_id(sistema, reserva->id_cliente);
        int quadra = buscar_quadra_por_id(sistema, reserva->id_quadra);
        int corresponde = 0;

        if (opcao == 1) {
            corresponde = comparar_datas(reserva->data, data) >= 0;
        } else if (opcao == 2 && cliente >= 0) {
            corresponde = strcmp(sistema->clientes[cliente].nome, valor) == 0;
        } else if (opcao == 3 && quadra >= 0) {
            corresponde = strcmp(sistema->quadras[quadra].nome, valor) == 0;
        }

        if (corresponde) {
            mostrar_reserva(sistema, reserva);
            encontrados++;
        }
    }
    if (!encontrados) printf("Nenhuma reserva corresponde ao filtro.\n");
}
