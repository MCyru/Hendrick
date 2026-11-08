#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "persistencia.h"
#include "sistema.h"
#include "entrada.h"

#define VERSAO_ARQUIVO 1
#define TAM_ASSINATURA 8
#define MAX_REGISTROS_ARQUIVO 1000000U

static int escrever_arquivo(const char *nome, const char assinatura[TAM_ASSINATURA],
                            const void *registros, size_t quantidade,
                            size_t tamanho_registro, int proximo_id) {
    FILE *arquivo;
    uint32_t versao = VERSAO_ARQUIVO;
    uint32_t qtd = (uint32_t)quantidade;
    int sucesso;

    arquivo = fopen(nome, "wb");
    if (arquivo == NULL) {
        perror(nome);
        return 0;
    }

    sucesso = fwrite(assinatura, 1, TAM_ASSINATURA, arquivo) == TAM_ASSINATURA &&
              fwrite(&versao, sizeof(versao), 1, arquivo) == 1 &&
              fwrite(&qtd, sizeof(qtd), 1, arquivo) == 1 &&
              fwrite(&proximo_id, sizeof(proximo_id), 1, arquivo) == 1;
    if (sucesso && quantidade > 0) {
        sucesso = fwrite(registros, tamanho_registro, quantidade, arquivo) == quantidade;
    }
    if (fclose(arquivo) != 0) {
        perror(nome);
        sucesso = 0;
    }
    if (!sucesso) {
        fprintf(stderr, "[ERRO] Falha ao gravar arquivo %s.\n", nome);
    }
    return sucesso;
}

static int ler_arquivo(const char *nome, const char assinatura[TAM_ASSINATURA],
                       void **registros, size_t *quantidade,
                       size_t tamanho_registro, int *proximo_id) {
    FILE *arquivo;
    char assinatura_lida[TAM_ASSINATURA];
    uint32_t versao;
    uint32_t qtd;
    int resultado = 1;

    arquivo = fopen(nome, "rb");
    if (arquivo == NULL) {
        if (errno == ENOENT) return 0;
        perror(nome);
        return -1;
    }

    if (fread(assinatura_lida, 1, TAM_ASSINATURA, arquivo) != TAM_ASSINATURA ||
        fread(&versao, sizeof(versao), 1, arquivo) != 1 ||
        fread(&qtd, sizeof(qtd), 1, arquivo) != 1 ||
        fread(proximo_id, sizeof(*proximo_id), 1, arquivo) != 1) {
        fprintf(stderr, "[ERRO] Cabecalho incompleto em %s.\n", nome);
        resultado = -1;
    } else if (memcmp(assinatura_lida, assinatura, TAM_ASSINATURA) != 0 ||
               versao != VERSAO_ARQUIVO || qtd > MAX_REGISTROS_ARQUIVO ||
               *proximo_id < 1) {
        fprintf(stderr, "[ERRO] Formato ou versao incompativel em %s.\n", nome);
        resultado = -1;
    } else {
        *quantidade = (size_t)qtd;
        if (qtd > 0) {
            *registros = malloc((size_t)qtd * tamanho_registro);
            if (*registros == NULL) {
                fprintf(stderr, "[ERRO] Memoria insuficiente ao carregar %s.\n", nome);
                resultado = -1;
            } else if (fread(*registros, tamanho_registro, qtd, arquivo) != qtd) {
                fprintf(stderr, "[ERRO] Registros incompletos em %s.\n", nome);
                free(*registros);
                *registros = NULL;
                resultado = -1;
            }
        }
        if (resultado == 1 && (fgetc(arquivo) != EOF || ferror(arquivo))) {
            fprintf(stderr, "[ERRO] Dados inesperados no final de %s.\n", nome);
            resultado = -1;
        }
    }

    if (fclose(arquivo) != 0) {
        perror(nome);
        resultado = -1;
    }
    return resultado;
}

static int localizar_cliente(const Sistema *sistema, int id) {
    size_t i;
    for (i = 0; i < sistema->total_clientes; i++) {
        if (sistema->clientes[i].id == id) return (int)i;
    }
    return -1;
}

static int localizar_quadra(const Sistema *sistema, int id) {
    size_t i;
    for (i = 0; i < sistema->total_quadras; i++) {
        if (sistema->quadras[i].id == id) return (int)i;
    }
    return -1;
}

static int validar_banco(const Sistema *sistema) {
    size_t i;
    size_t j;

    if (sistema->proximo_id_cliente < 1 ||
        sistema->proximo_id_quadra < 1 ||
        sistema->proximo_id_reserva < 1 ||
        sistema->total_clientes >= (size_t)sistema->proximo_id_cliente ||
        sistema->total_quadras >= (size_t)sistema->proximo_id_quadra ||
        sistema->total_reservas >= (size_t)sistema->proximo_id_reserva) {
        return 0;
    }

    for (i = 0; i < sistema->total_clientes; i++) {
        const Cliente *cliente = &sistema->clientes[i];
        if (cliente->id < 1 || cliente->id >= sistema->proximo_id_cliente ||
            cliente->idade < 0 || cliente->idade > 120 ||
            memchr(cliente->cpf, '\0', TAM_CPF) == NULL ||
            memchr(cliente->nome, '\0', TAM_NOME) == NULL ||
            memchr(cliente->telefone, '\0', TAM_TELEFONE) == NULL ||
            cliente->cpf[0] == '\0' || cliente->nome[0] == '\0' ||
            cliente->telefone[0] == '\0') return 0;
        if (strlen(cliente->cpf) != 11) return 0;
        for (j = 0; j < 11; j++) {
            if (cliente->cpf[j] < '0' || cliente->cpf[j] > '9') return 0;
        }
        for (j = i + 1; j < sistema->total_clientes; j++) {
            if (cliente->id == sistema->clientes[j].id ||
                strcmp(cliente->cpf, sistema->clientes[j].cpf) == 0) return 0;
        }
    }

    for (i = 0; i < sistema->total_quadras; i++) {
        const Quadra *quadra = &sistema->quadras[i];
        if (quadra->id < 1 || quadra->id >= sistema->proximo_id_quadra ||
            !(quadra->preco_hora >= 0.0f) ||
            quadra->preco_hora > 100000000.0f ||
            memchr(quadra->codigo, '\0', TAM_CODIGO_QUADRA) == NULL ||
            memchr(quadra->nome, '\0', TAM_NOME) == NULL ||
            quadra->codigo[0] == '\0' || quadra->nome[0] == '\0') return 0;
        for (j = i + 1; j < sistema->total_quadras; j++) {
            if (quadra->id == sistema->quadras[j].id ||
                strcmp(quadra->codigo, sistema->quadras[j].codigo) == 0) return 0;
        }
    }

    for (i = 0; i < sistema->total_reservas; i++) {
        const Reserva *reserva = &sistema->reservas[i];
        if (reserva->id < 1 || reserva->id >= sistema->proximo_id_reserva ||
            reserva->hora < 0 || reserva->hora > 23 ||
            memchr(reserva->data, '\0', TAM_DATA) == NULL ||
            !data_valida(reserva->data) ||
            localizar_cliente(sistema, reserva->id_cliente) < 0 ||
            localizar_quadra(sistema, reserva->id_quadra) < 0) return 0;
        for (j = i + 1; j < sistema->total_reservas; j++) {
            if (reserva->id == sistema->reservas[j].id ||
                (reserva->id_quadra == sistema->reservas[j].id_quadra &&
                 strcmp(reserva->data, sistema->reservas[j].data) == 0 &&
                 reserva->hora == sistema->reservas[j].hora)) return 0;
        }
    }
    return 1;
}

int carregar_dados(Sistema *sistema) {
    void *clientes = NULL;
    void *quadras = NULL;
    void *reservas = NULL;
    size_t total_clientes = 0;
    size_t total_quadras = 0;
    size_t total_reservas = 0;
    int id_cliente = 1;
    int id_quadra = 1;
    int id_reserva = 1;
    int resultado_clientes;
    int resultado_quadras;
    int resultado_reservas;

    resultado_clientes = ler_arquivo("clientes.bin", "ARHOS-C1", &clientes,
                                     &total_clientes, sizeof(Cliente), &id_cliente);
    resultado_quadras = ler_arquivo("quadras.bin", "ARHOS-Q1", &quadras,
                                    &total_quadras, sizeof(Quadra), &id_quadra);
    resultado_reservas = ler_arquivo("reservas.bin", "ARHOS-R1", &reservas,
                                     &total_reservas, sizeof(Reserva), &id_reserva);

    if (resultado_clientes == 0 && resultado_quadras == 0 && resultado_reservas == 0) {
        printf("[INFO] Banco nao encontrado; iniciando sem registros.\n");
        return 1;
    }
    if (resultado_clientes != 1 || resultado_quadras != 1 || resultado_reservas != 1) {
        fprintf(stderr, "[ERRO] O banco precisa conter os tres arquivos validos.\n");
        free(clientes);
        free(quadras);
        free(reservas);
        return 0;
    }

    free(sistema->clientes);
    free(sistema->quadras);
    free(sistema->reservas);
    sistema->clientes = clientes;
    sistema->quadras = quadras;
    sistema->reservas = reservas;
    sistema->total_clientes = total_clientes;
    sistema->total_quadras = total_quadras;
    sistema->total_reservas = total_reservas;
    sistema->capacidade_clientes = total_clientes;
    sistema->capacidade_quadras = total_quadras;
    sistema->capacidade_reservas = total_reservas;
    sistema->proximo_id_cliente = id_cliente;
    sistema->proximo_id_quadra = id_quadra;
    sistema->proximo_id_reserva = id_reserva;

    if (!sistema_ajustar_capacidade(sistema, 1, total_clientes + 1) ||
        !sistema_ajustar_capacidade(sistema, 2, total_quadras + 1) ||
        !sistema_ajustar_capacidade(sistema, 3, total_reservas + 1) ||
        !validar_banco(sistema)) {
        fprintf(stderr, "[ERRO] Banco invalido ou memoria insuficiente ao carregar.\n");
        return 0;
    }

    printf("[INFO] Banco carregado: %lu clientes, %lu quadras, %lu reservas.\n",
           (unsigned long)total_clientes, (unsigned long)total_quadras,
           (unsigned long)total_reservas);
    return 1;
}

int salvar_dados(const Sistema *sistema) {
    int sucesso = 1;

    if (!validar_banco(sistema)) {
        fprintf(stderr, "[ERRO] Dados invalidos; banco nao foi salvo.\n");
        return 0;
    }
    if (!escrever_arquivo("clientes.bin", "ARHOS-C1", sistema->clientes,
                         sistema->total_clientes, sizeof(Cliente),
                         sistema->proximo_id_cliente)) sucesso = 0;
    if (!escrever_arquivo("quadras.bin", "ARHOS-Q1", sistema->quadras,
                         sistema->total_quadras, sizeof(Quadra),
                         sistema->proximo_id_quadra)) sucesso = 0;
    if (!escrever_arquivo("reservas.bin", "ARHOS-R1", sistema->reservas,
                         sistema->total_reservas, sizeof(Reserva),
                         sistema->proximo_id_reserva)) sucesso = 0;
    if (sucesso) printf("[INFO] Banco de dados salvo.\n");
    return sucesso;
}
