#ifndef DADOS_H
#define DADOS_H

#include <stddef.h>

#define INCREMENTO_CAPACIDADE 5
#define TAM_NOME 60
#define TAM_TELEFONE 20
#define TAM_CPF 15
#define TAM_CODIGO_QUADRA 16
#define TAM_DATA 11
#define TAM_CAMPO 120

typedef struct {
    int id;
    char cpf[TAM_CPF];
    char nome[TAM_NOME];
    char telefone[TAM_TELEFONE];
    int idade;
} Cliente;

typedef struct {
    int id;
    char codigo[TAM_CODIGO_QUADRA];
    char nome[TAM_NOME];
    float preco_hora;
} Quadra;

typedef struct {
    int id;
    int id_cliente;
    int id_quadra;
    char data[TAM_DATA];
    int hora;
} Reserva;

typedef struct {
    Cliente *clientes;
    size_t total_clientes;
    size_t capacidade_clientes;
    Quadra *quadras;
    size_t total_quadras;
    size_t capacidade_quadras;
    Reserva *reservas;
    size_t total_reservas;
    size_t capacidade_reservas;
    int proximo_id_cliente;
    int proximo_id_quadra;
    int proximo_id_reserva;
} Sistema;

#endif
