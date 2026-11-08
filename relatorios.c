#include <stdio.h>
#include <time.h>
#include "relatorios.h"
#include "clientes.h"
#include "quadras.h"
#include "entrada.h"

static void escrever_reserva(FILE *arquivo, const Sistema *sistema,
                             const Reserva *reserva) {
    int indice_cliente = buscar_cliente_por_id(sistema, reserva->id_cliente);
    int indice_quadra = buscar_quadra_por_id(sistema, reserva->id_quadra);
    const Cliente *cliente;
    const Quadra *quadra;

    if (indice_cliente < 0 || indice_quadra < 0) {
        fprintf(arquivo, "ERRO: reserva %d com relacionamento invalido.\n\n",
                reserva->id);
        return;
    }

    cliente = &sistema->clientes[indice_cliente];
    quadra = &sistema->quadras[indice_quadra];
    fprintf(arquivo,
            "RESERVA #%d\n"
            "  Data: %s\n"
            "  Horario: %02dh00\n"
            "  Cliente relacionado:\n"
            "    ID: %d | CPF: %s | Nome: %s | Telefone: %s | Idade: %d\n"
            "  Quadra relacionada:\n"
            "    ID: %d | Codigo: %s | Nome/tipo: %s | Preco por hora: R$ %.2f\n"
            "-----------------------------------\n",
            reserva->id, reserva->data, reserva->hora,
            cliente->id, cliente->cpf, cliente->nome,
            cliente->telefone, cliente->idade,
            quadra->id, quadra->codigo, quadra->nome, quadra->preco_hora);
}

static int data_atual(char data[TAM_DATA]) {
    time_t agora = time(NULL);
    struct tm *data_local = localtime(&agora);
    int dia;
    int mes;
    int ano;

    if (data_local == NULL) return 0;
    dia = data_local->tm_mday;
    mes = data_local->tm_mon + 1;
    ano = data_local->tm_year + 1900;
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || ano > 9999) {
        return 0;
    }

    data[0] = (char)('0' + dia / 10);
    data[1] = (char)('0' + dia % 10);
    data[2] = '/';
    data[3] = (char)('0' + mes / 10);
    data[4] = (char)('0' + mes % 10);
    data[5] = '/';
    data[6] = (char)('0' + ano / 1000);
    data[7] = (char)('0' + (ano / 100) % 10);
    data[8] = (char)('0' + (ano / 10) % 10);
    data[9] = (char)('0' + ano % 10);
    data[10] = '\0';
    return 1;
}

void gerar_relatorio_reservas(const Sistema *sistema) {
    int opcao;
    int id = 0;
    int indice = -1;
    char data_inicio[TAM_DATA];
    char hoje[TAM_DATA];
    char filtro[TAM_CAMPO];
    const char *titulo;
    FILE *arquivo;
    size_t i;
    int encontrados = 0;
    int falha_escrita = 0;

    printf("\n+------------- RELATORIOS ----------------------+\n");
    printf("1 - Reservas por data (data inicial ate hoje)\n");
    printf("2 - Reservas por cliente\n");
    printf("3 - Reservas por quadra\n");
    if (!ler_inteiro("Opcao: ", &opcao)) return;
    if (opcao < 1 || opcao > 3) {
        printf("[ERRO] Opcao invalida.\n");
        return;
    }

    if (opcao == 1) {
        if (!ler_data("Data inicial (DD/MM/AAAA): ", data_inicio) ||
            !data_atual(hoje)) {
            printf("[ERRO] Nao foi possivel definir o periodo do relatorio.\n");
            return;
        }
        titulo = "Reservas por periodo";
        snprintf(filtro, sizeof(filtro), "de %s ate %s", data_inicio, hoje);
    } else if (opcao == 2) {
        if (!ler_inteiro("ID do cliente: ", &id)) return;
        indice = buscar_cliente_por_id(sistema, id);
        if (indice < 0) {
            printf("[ERRO] Cliente nao encontrado. Relatorio nao gerado.\n");
            return;
        }
        titulo = "Reservas por cliente";
        snprintf(filtro, sizeof(filtro), "cliente ID %d - %s",
                 id, sistema->clientes[indice].nome);
    } else {
        if (!ler_inteiro("ID da quadra: ", &id)) return;
        indice = buscar_quadra_por_id(sistema, id);
        if (indice < 0) {
            printf("[ERRO] Quadra nao encontrada. Relatorio nao gerado.\n");
            return;
        }
        titulo = "Reservas por quadra";
        snprintf(filtro, sizeof(filtro), "quadra ID %d - %s",
                 id, sistema->quadras[indice].nome);
    }

    arquivo = fopen("relatorio_reservas.txt", "w");
    if (arquivo == NULL) {
        perror("relatorio_reservas.txt");
        return;
    }
    fprintf(arquivo, "ARENA H.O.S. - RELATORIO DE RESERVAS\n");
    fprintf(arquivo, "====================================\n");
    fprintf(arquivo, "%s\nFiltro: %s\n\n", titulo, filtro);

    for (i = 0; i < sistema->total_reservas; i++) {
        const Reserva *reserva = &sistema->reservas[i];
        int corresponde = 0;
        if (opcao == 1) {
            corresponde = comparar_datas(reserva->data, data_inicio) >= 0 &&
                          comparar_datas(reserva->data, hoje) <= 0;
        } else if (opcao == 2) {
            corresponde = reserva->id_cliente == id;
        } else {
            corresponde = reserva->id_quadra == id;
        }
        if (corresponde) {
            escrever_reserva(arquivo, sistema, reserva);
            encontrados++;
        }
    }
    if (!encontrados) {
        fprintf(arquivo, "Nenhuma reserva corresponde ao filtro.\n");
    }

    if (ferror(arquivo)) {
        fprintf(stderr, "[ERRO] Falha durante a escrita do relatorio.\n");
        falha_escrita = 1;
    }
    if (fclose(arquivo) != 0) {
        perror("relatorio_reservas.txt");
        return;
    }
    if (falha_escrita) return;
    printf("[OK] Relatorio gravado em relatorio_reservas.txt.\n");
}
