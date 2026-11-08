#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sistema.h"
#include "persistencia.h"

int sistema_inicializar(Sistema *sistema) {
    sistema->clientes = NULL;
    sistema->total_clientes = 0;
    sistema->capacidade_clientes = 0;
    sistema->quadras = NULL;
    sistema->total_quadras = 0;
    sistema->capacidade_quadras = 0;
    sistema->reservas = NULL;
    sistema->total_reservas = 0;
    sistema->capacidade_reservas = 0;
    sistema->proximo_id_cliente = 1;
    sistema->proximo_id_quadra = 1;
    sistema->proximo_id_reserva = 1;

    return sistema_ajustar_capacidade(sistema, 1, INCREMENTO_CAPACIDADE) &&
           sistema_ajustar_capacidade(sistema, 2, INCREMENTO_CAPACIDADE) &&
           sistema_ajustar_capacidade(sistema, 3, INCREMENTO_CAPACIDADE);
}

void sistema_liberar(Sistema *sistema) {
    free(sistema->clientes);
    free(sistema->quadras);
    free(sistema->reservas);
    sistema->clientes = NULL;
    sistema->quadras = NULL;
    sistema->reservas = NULL;
    sistema->total_clientes = 0;
    sistema->total_quadras = 0;
    sistema->total_reservas = 0;
    sistema->capacidade_clientes = 0;
    sistema->capacidade_quadras = 0;
    sistema->capacidade_reservas = 0;
}

int sistema_ajustar_capacidade(Sistema *sistema, int estrutura, size_t necessaria) {
    size_t capacidade;
    size_t alvo;
    size_t tamanho;
    void *temporario;

    switch (estrutura) {
        case 1:
            capacidade = sistema->capacidade_clientes;
            tamanho = sizeof(Cliente);
            break;
        case 2:
            capacidade = sistema->capacidade_quadras;
            tamanho = sizeof(Quadra);
            break;
        case 3:
            capacidade = sistema->capacidade_reservas;
            tamanho = sizeof(Reserva);
            break;
        default:
            return 0;
    }

    alvo = capacidade;
    if (necessaria > capacidade) {
        while (alvo < necessaria) {
            if (alvo > (size_t)-1 - INCREMENTO_CAPACIDADE) {
                fprintf(stderr, "[ERRO] Capacidade solicitada excede o limite.\n");
                return 0;
            }
            alvo += INCREMENTO_CAPACIDADE;
        }
    } else if (capacidade > necessaria + INCREMENTO_CAPACIDADE) {
        alvo = necessaria + INCREMENTO_CAPACIDADE;
    }

    if (alvo == capacidade) return 1;
    if (tamanho != 0 && alvo > SIZE_MAX / tamanho) {
        fprintf(stderr, "[ERRO] Tamanho do vetor excede o limite do sistema.\n");
        return 0;
    }

    if (estrutura == 1) {
        temporario = realloc(sistema->clientes, alvo * tamanho);
        if (temporario == NULL) goto erro_memoria;
        sistema->clientes = temporario;
        sistema->capacidade_clientes = alvo;
    } else if (estrutura == 2) {
        temporario = realloc(sistema->quadras, alvo * tamanho);
        if (temporario == NULL) goto erro_memoria;
        sistema->quadras = temporario;
        sistema->capacidade_quadras = alvo;
    } else {
        temporario = realloc(sistema->reservas, alvo * tamanho);
        if (temporario == NULL) goto erro_memoria;
        sistema->reservas = temporario;
        sistema->capacidade_reservas = alvo;
    }
    return 1;

erro_memoria:
    fprintf(stderr, "[ERRO] Memoria insuficiente para redimensionar o vetor.\n");
    return 0;
}

int sistema_salvar_apos_alteracao(Sistema *sistema) {
    if (salvar_dados(sistema)) {
        return 1;
    }
    printf("[ERRO] Alteracao mantida na memoria, mas ainda nao foi salva.\n");
    return 0;
}
