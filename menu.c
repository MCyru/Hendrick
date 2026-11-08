#include <stdio.h>
#include "menu.h"
#include "clientes.h"
#include "quadras.h"
#include "reservas.h"
#include "relatorios.h"
#include "entrada.h"
#include "sistema.h"

static void salvar_se_alterado(Sistema *sistema, int alterado) {
    if (alterado) {
        (void)sistema_salvar_apos_alteracao(sistema);
    }
}

static void mostrar_cabecalho(void) {
    printf("\n");
    printf("==========================================================\n");
    printf("               ARENA H.O.S - GESTAO DE QUADRAS            \n");
    printf("==========================================================\n");
}

static void menu_clientes(Sistema *sistema) {
    int opcao;
    do {
        printf("\n+------------------ CLIENTES ----------------------------+\n");
        printf("| 1 - Cadastrar | 2 - Listar todos | 3 - Consultar       |\n");
        printf("| 4 - Alterar   | 5 - Remover      | 0 - Voltar          |\n");
        printf("+--------------------------------------------------------+\n");
        if (!ler_inteiro("Opcao: ", &opcao)) return;
        switch (opcao) {
            case 1: salvar_se_alterado(sistema, cadastrar_cliente(sistema)); break;
            case 2: listar_clientes(sistema); break;
            case 3: (void)consultar_cliente(sistema); break;
            case 4: salvar_se_alterado(sistema, atualizar_cliente(sistema)); break;
            case 5: salvar_se_alterado(sistema, remover_cliente(sistema)); break;
            case 0: break;
            default: printf("[ERRO] Opcao invalida.\n");
        }
    } while (opcao != 0);
}

static void menu_quadras(Sistema *sistema) {
    int opcao;
    do {
        printf("\n+------------------- QUADRAS ----------------------------+\n");
        printf("| 1 - Cadastrar | 2 - Listar todas | 3 - Consultar      |\n");
        printf("| 4 - Alterar   | 5 - Remover      | 0 - Voltar         |\n");
        printf("+--------------------------------------------------------+\n");
        if (!ler_inteiro("Opcao: ", &opcao)) return;
        switch (opcao) {
            case 1: salvar_se_alterado(sistema, cadastrar_quadra(sistema)); break;
            case 2: listar_quadras(sistema); break;
            case 3: (void)consultar_quadra(sistema); break;
            case 4: salvar_se_alterado(sistema, atualizar_quadra(sistema)); break;
            case 5: salvar_se_alterado(sistema, remover_quadra(sistema)); break;
            case 0: break;
            default: printf("[ERRO] Opcao invalida.\n");
        }
    } while (opcao != 0);
}

static void menu_reservas(Sistema *sistema) {
    int opcao;
    do {
        printf("\n+------------------- RESERVAS ---------------------------+\n");
        printf("| 1 - Cadastrar | 2 - Listar todas | 3 - Consultar      |\n");
        printf("| 4 - Alterar   | 5 - Remover      | 6 - Filtrar        |\n");
        printf("| 0 - Voltar                                             |\n");
        printf("+--------------------------------------------------------+\n");
        if (!ler_inteiro("Opcao: ", &opcao)) return;
        switch (opcao) {
            case 1: salvar_se_alterado(sistema, cadastrar_reserva(sistema)); break;
            case 2: listar_reservas(sistema); break;
            case 3: (void)consultar_reserva(sistema); break;
            case 4: salvar_se_alterado(sistema, atualizar_reserva(sistema)); break;
            case 5: salvar_se_alterado(sistema, remover_reserva(sistema)); break;
            case 6: filtrar_reservas(sistema); break;
            case 0: break;
            default: printf("[ERRO] Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void executar_menu(Sistema *sistema) {
    int opcao;
    do {
        mostrar_cabecalho();
        printf("| 1 - Gerenciar clientes                                 |\n");
        printf("| 2 - Gerenciar quadras                                  |\n");
        printf("| 3 - Gerenciar reservas                                 |\n");
        printf("| 4 - Gerar relatorio de reservas                        |\n");
        printf("| 0 - Salvar e sair                                      |\n");
        printf("==========================================================\n");
        if (!ler_inteiro("Opcao: ", &opcao)) opcao = 0;
        switch (opcao) {
            case 1: menu_clientes(sistema); break;
            case 2: menu_quadras(sistema); break;
            case 3: menu_reservas(sistema); break;
            case 4: gerar_relatorio_reservas(sistema); break;
            case 0: printf("Encerrando o Arena H.O.S.\n"); break;
            default: printf("[ERRO] Opcao invalida.\n");
        }
    } while (opcao != 0);
}
