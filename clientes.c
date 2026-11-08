#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"
#include "entrada.h"
#include "sistema.h"

int buscar_cliente_por_id(const Sistema *sistema, int id) {
    size_t i;
    for (i = 0; i < sistema->total_clientes; i++) {
        if (sistema->clientes[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

int buscar_cliente_por_cpf(const Sistema *sistema, const char *cpf) {
    size_t i;
    for (i = 0; i < sistema->total_clientes; i++) {
        if (strcmp(sistema->clientes[i].cpf, cpf) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static int preparar_cliente(Sistema *sistema, Cliente *cliente, int novo) {
    size_t i;
    if (!ler_texto("CPF (somente digitos): ", cliente->cpf, TAM_CPF) ||
        !ler_texto("Nome completo: ", cliente->nome, TAM_NOME) ||
        !ler_texto("Telefone: ", cliente->telefone, TAM_TELEFONE) ||
        !ler_inteiro("Idade: ", &cliente->idade)) {
        return 0;
    }
    if (cliente->idade < 0 || cliente->idade > 120) {
        printf("[ERRO] Idade deve estar entre 0 e 120.\n");
        return 0;
    }
    if (strlen(cliente->cpf) != 11) {
        printf("[ERRO] CPF deve conter exatamente 11 digitos.\n");
        return 0;
    }
    for (i = 0; i < 11; i++) {
        if (cliente->cpf[i] < '0' || cliente->cpf[i] > '9') {
            printf("[ERRO] CPF deve conter somente digitos.\n");
            return 0;
        }
    }

    {
        int encontrado = buscar_cliente_por_cpf(sistema, cliente->cpf);
        if (encontrado >= 0 &&
            (novo || sistema->clientes[encontrado].id != cliente->id)) {
            printf("[ERRO] Ja existe cliente cadastrado com esse CPF.\n");
            return 0;
        }
    }
    return 1;
}

int cadastrar_cliente(Sistema *sistema) {
    Cliente novo = {0};

    if (sistema->proximo_id_cliente == INT_MAX) {
        printf("[ERRO] Limite de identificadores de clientes atingido.\n");
        return 0;
    }
    printf("\n+---------------- NOVO CLIENTE ----------------+\n");
    if (!preparar_cliente(sistema, &novo, 1)) {
        return 0;
    }
    if (!sistema_ajustar_capacidade(sistema, 1, sistema->total_clientes + 1)) {
        return 0;
    }

    novo.id = sistema->proximo_id_cliente;
    sistema->clientes[sistema->total_clientes++] = novo;
    sistema->proximo_id_cliente++;
    printf("[OK] Cliente cadastrado com ID %d.\n", novo.id);
    return 1;
}

void listar_clientes(const Sistema *sistema) {
    size_t i;
    printf("\n+---------------- CLIENTES ---------------------+\n");
    if (sistema->total_clientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (i = 0; i < sistema->total_clientes; i++) {
        const Cliente *cliente = &sistema->clientes[i];
        printf("ID: %d | CPF: %s | Nome: %s | Telefone: %s | Idade: %d\n",
               cliente->id, cliente->cpf, cliente->nome,
               cliente->telefone, cliente->idade);
    }
}

static int escolher_cliente(const Sistema *sistema) {
    int opcao;
    int id;
    char valor[TAM_NOME];

    printf("\nBuscar cliente por:\n");
    printf("1 - ID\n2 - CPF\n3 - Nome exato\n0 - Cancelar\n");
    if (!ler_inteiro("Opcao: ", &opcao)) {
        return -1;
    }
    switch (opcao) {
        case 1:
            if (!ler_inteiro("ID: ", &id)) return -1;
            return buscar_cliente_por_id(sistema, id);
        case 2:
            if (!ler_texto("CPF: ", valor, TAM_NOME)) return -1;
            return buscar_cliente_por_cpf(sistema, valor);
        case 3: {
            size_t i;
            if (!ler_texto("Nome exato: ", valor, TAM_NOME)) return -1;
            for (i = 0; i < sistema->total_clientes; i++) {
                if (strcmp(sistema->clientes[i].nome, valor) == 0) return (int)i;
            }
            return -1;
        }
        default:
            return -1;
    }
}

int consultar_cliente(const Sistema *sistema) {
    int indice = escolher_cliente(sistema);
    if (indice < 0) {
        printf("[INFO] Cliente nao encontrado ou consulta cancelada.\n");
        return 0;
    }
    {
        const Cliente *cliente = &sistema->clientes[indice];
        printf("ID: %d | CPF: %s | Nome: %s | Telefone: %s | Idade: %d\n",
               cliente->id, cliente->cpf, cliente->nome,
               cliente->telefone, cliente->idade);
    }
    return 0;
}

int atualizar_cliente(Sistema *sistema) {
    int indice;
    Cliente atualizado;

    indice = escolher_cliente(sistema);
    if (indice < 0) {
        printf("[ERRO] Cliente nao encontrado.\n");
        return 0;
    }
    atualizado = sistema->clientes[indice];
    printf("Informe os novos dados (CPF deve continuar unico).\n");
    if (!preparar_cliente(sistema, &atualizado, 0)) {
        return 0;
    }
    sistema->clientes[indice] = atualizado;
    printf("[OK] Cliente atualizado.\n");
    return 1;
}

int remover_cliente(Sistema *sistema) {
    int indice;
    size_t i;

    indice = escolher_cliente(sistema);
    if (indice < 0) {
        printf("[ERRO] Cliente nao encontrado.\n");
        return 0;
    }
    for (i = 0; i < sistema->total_reservas; i++) {
        if (sistema->reservas[i].id_cliente == sistema->clientes[indice].id) {
            printf("[ERRO] Cliente associado a reservas; remova-as antes.\n");
            return 0;
        }
    }
    if (!sistema_ajustar_capacidade(sistema, 1, sistema->total_clientes - 1)) {
        printf("[ERRO] Remocao cancelada: nao foi possivel ajustar a memoria.\n");
        return 0;
    }
    memmove(&sistema->clientes[indice], &sistema->clientes[indice + 1],
            (sistema->total_clientes - (size_t)indice - 1) * sizeof(Cliente));
    sistema->total_clientes--;
    printf("[OK] Cliente removido.\n");
    return 1;
}
