#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 5

// ============================================================================
// 1. ESTRUTURAS DE DADOS (STRUCTS)
// ============================================================================

// Estrutura 1: Cliente
typedef struct {
    int id;
    char nome[50];
    char telefone[20];
    int ativo; // 1 = Registro Ativo | 0 = Removido (Exclusao Logica)
} Cliente;

// Estrutura 2: Quadra
typedef struct {
    int id;
    char nome[50];
    float preco_hora;
    int ativa; // 1 = Registro Ativo | 0 = Removida (Exclusao Logica)
} Quadra;

// Estrutura 3: Reserva (Relacao de 2 Variaveis: Cliente x Quadra -> Horario)
typedef struct {
    int id;
    int id_cliente;
    int id_quadra;
    int hora;  // (ex: 18 para 18:00)
    int ativa; // 1 = Confirmada | 0 = Cancelada
} Reserva;

// ============================================================================
// 2. BANCO DE DADOS EM MEMORIA (VETORES GLOBAIS DE TAMANHO 5)
// ============================================================================

Cliente clientes[TAM_MAX];
int total_clientes = 0;

Quadra quadras[TAM_MAX];
int total_quadras = 0;

Reserva reservas[TAM_MAX];
int total_reservas = 0;

// ============================================================================
// 3. CRUD DA ESTRUTURA 1: CLIENTES
// ============================================================================

// [CREATE] Cadastra um novo cliente no vetor
void criar_cliente() {
    // LOGICA: Verifica se o vetor de 5 posicoes ja esta cheio
    if (total_clientes >= TAM_MAX) {
        printf("\n[ERRO] Capacidade maxima de clientes (5) atingida!\n");
        return;
    }

    Cliente c;
    c.id = total_clientes + 1; // ID gerado automaticamente (1, 2, 3...)
    
    printf("\n--- Novo Cadastro de Cliente ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", c.nome);
    printf("Telefone: ");
    scanf(" %[^\n]", c.telefone);
    c.ativo = 1; // Define como ativo por padrao

    clientes[total_clientes] = c;
    total_clientes++;

    printf(">> Cliente '%s' cadastrado com sucesso! (ID: %d)\n", c.nome, c.id);
}

// [READ] Lista todos os clientes ativos
void listar_clientes() {
    printf("\n--- Lista de Clientes ---\n");
    int encontrados = 0;

    // LOGICA: Percorre o vetor ate o total cadastrado, filtrando apenas os ativos
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].ativo == 1) {
            printf("[%d] ID: %d | Nome: %s | Tel: %s\n", i + 1, clientes[i].id, clientes[i].nome, clientes[i].telefone);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum cliente ativo encontrado.\n");
    }
}

// [UPDATE] Atualiza os dados de um cliente existente
void atualizar_cliente() {
    int id;
    printf("\nInforme o ID do cliente que deseja atualizar: ");
    scanf("%d", &id);

    // LOGICA: Busca linear no vetor pelo ID informado
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].id == id && clientes[i].ativo == 1) {
            printf("Cliente localizado: %s\n", clientes[i].nome);
            printf("Digite o Novo Nome: ");
            scanf(" %[^\n]", clientes[i].nome);
            printf("Digite o Novo Telefone: ");
            scanf(" %[^\n]", clientes[i].telefone);

            printf(">> Cadastro de cliente atualizado com sucesso!\n");
            return;
        }
    }
    printf("[ERRO] Cliente com ID %d nao foi encontrado ou esta inativo.\n", id);
}

// [DELETE] Remove um cliente do sistema (Exclusão Lógica)
void deletar_cliente() {
    int id;
    printf("\nInforme o ID do cliente a ser removido: ");
    scanf("%d", &id);

    // LOGICA: Em vez de apagar do vetor, alteramos o status 'ativo' para 0
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].id == id && clientes[i].ativo == 1) {
            clientes[i].ativo = 0;
            printf(">> Cliente '%s' removido com sucesso!\n", clientes[i].nome);
            return;
        }
    }
    printf("[ERRO] Cliente com ID %d nao foi encontrado.\n", id);
}

// ============================================================================
// 4. CRUD DA ESTRUTURA 2: QUADRAS
// ============================================================================

// [CREATE] Cadastra uma nova quadra
void criar_quadra() {
    if (total_quadras >= TAM_MAX) {
        printf("\n[ERRO] Capacidade maxima de quadras (5) atingida!\n");
        return;
    }

    Quadra q;
    q.id = total_quadras + 1;

    printf("\n--- Novo Cadastro de Quadra ---\n");
    printf("Nome/Tipo da Quadra (ex: Society A, Areia 1): ");
    scanf(" %[^\n]", q.nome);
    printf("Preco da Hora (R$): ");
    scanf("%f", &q.preco_hora);
    q.ativa = 1;

    quadras[total_quadras] = q;
    total_quadras++;

    printf(">> Quadra '%s' cadastrada com sucesso! (ID: %d)\n", q.nome, q.id);
}

// [READ] Lista todas as quadras ativas
void listar_quadras() {
    printf("\n--- Lista de Quadras ---\n");
    int encontradas = 0;

    for (int i = 0; i < total_quadras; i++) {
        if (quadras[i].ativa == 1) {
            printf("[%d] ID: %d | Quadra: %s | Tarifa/Hora: R$ %.2f\n", i + 1, quadras[i].id, quadras[i].nome, quadras[i].preco_hora);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma quadra ativa cadastrada.\n");
    }
}

// [UPDATE] Atualiza dados da quadra
void atualizar_quadra() {
    int id;
    printf("\nInforme o ID da quadra que deseja atualizar: ");
    scanf("%d", &id);

    for (int i = 0; i < total_quadras; i++) {
        if (quadras[i].id == id && quadras[i].ativa == 1) {
            printf("Quadra localizada: %s\n", quadras[i].nome);
            printf("Digite o Novo Nome/Tipo: ");
            scanf(" %[^\n]", quadras[i].nome);
            printf("Digite o Novo Preco por Hora: ");
            scanf("%f", &quadras[i].preco_hora);

            printf(">> Cadastro da quadra atualizado com sucesso!\n");
            return;
        }
    }
    printf("[ERRO] Quadra com ID %d nao foi encontrada.\n", id);
}

// [DELETE] Remove uma quadra (Exclusao Logica)
void deletar_quadra() {
    int id;
    printf("\nInforme o ID da quadra a ser removida: ");
    scanf("%d", &id);

    for (int i = 0; i < total_quadras; i++) {
        if (quadras[i].id == id && quadras[i].ativa == 1) {
            quadras[i].ativa = 0;
            printf(">> Quadra '%s' removida com sucesso!\n", quadras[i].nome);
            return;
        }
    }
    printf("[ERRO] Quadra com ID %d nao foi encontrada.\n", id);
}

// ============================================================================
// 5. FUNCOES DA ESTRUTURA 3: RESERVAS E VALIDACAO DE CONFLITO
// ============================================================================

// ALGORITMO: Checa se a quadra ja possui reserva ativa no mesmo horario
int verificar_disponibilidade(int id_quadra, int hora) {
    // LÓGICA: Percorre todas as reservas gravadas
    for (int i = 0; i < total_reservas; i++) {
        // Se a reserva estiver ativa E for na mesma quadra E no mesmo horario -> CONFLITO!
        if (reservas[i].ativa == 1 && reservas[i].id_quadra == id_quadra && reservas[i].hora == hora) {
            return 0; // Ocupado
        }
    }
    return 1; // Disponivel
}

// Funcao inicial para registrar nova reserva (Cliente x Quadra)
void criar_reserva() {
    if (total_reservas >= TAM_MAX) {
        printf("\n[ERRO] Limite maximo de reservas (5) atingido!\n");
        return;
    }

    if (total_clientes == 0 || total_quadras == 0) {
        printf("\n[AVISO] E necessario ter pelo menos 1 cliente e 1 quadra cadastrados antes de reservar.\n");
        return;
    }

    int id_cli, id_quad, hora;

    printf("\n--- Nova Reserva de Quadra ---\n");
    listar_clientes();
    printf("Informe o ID do Cliente: ");
    scanf("%d", &id_cli);

    listar_quadras();
    printf("Informe o ID da Quadra: ");
    scanf("%d", &id_quad);

    printf("Informe o Horario do jogo (ex: 18 para 18:00): ");
    scanf("%d", &hora);

    // LOGICA: Executa o algoritmo de validacao antes de confirmar a reserva
    if (verificar_disponibilidade(id_quad, hora) == 1) {
        Reserva r;
        r.id = total_reservas + 1;
        r.id_cliente = id_cli;
        r.id_quadra = id_quad;
        r.hora = hora;
        r.ativa = 1;

        reservas[total_reservas] = r;
        total_reservas++;

        printf(">> RESERVA CONFIRMADA COM SUCESSO! (Reserva #%d)\n", r.id);
    } else {
        printf("\n[ERRO DE CONFLITO] A quadra solicitada ja esta ocupada as %dh00!\n", hora);
    }
}

// Lista as reservas associando com os nomes dos clientes e das quadras
void listar_reservas() {
    printf("\n--- Grade de Reservas ---\n");
    int encontradas = 0;

    for (int i = 0; i < total_reservas; i++) {
        if (reservas[i].ativa == 1) {
            // LOGICA: Busca os nomes nos vetores de clientes e quadras usando o ID
            char nome_cliente[50] = "Desconhecido";
            char nome_quadra[50] = "Desconhecida";

            for (int c = 0; c < total_clientes; c++) {
                if (clientes[c].id == reservas[i].id_cliente) {
                    strcpy(nome_cliente, clientes[c].nome);
                    break;
                }
            }

            for (int q = 0; q < total_quadras; q++) {
                if (quadras[q].id == reservas[i].id_quadra) {
                    strcpy(nome_quadra, quadras[q].nome);
                    break;
                }
            }

            printf("Reserva #%d | Quadra: %s | Horario: %dh00 | Cliente: %s\n",
                   reservas[i].id, nome_quadra, reservas[i].hora, nome_cliente);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma reserva ativa cadastrada.\n");
    }
}

// ============================================================================
// 6. ESTRUTURA DE MENUS INTERATIVOS (NAVEGACAO COM SWITCH-CASE)
// ============================================================================

void menu_clientes() {
    int opcao;
    do {
        printf("\n=================================\n");
        printf("       GESTAO DE CLIENTES (CRUD)  \n");
        printf("=================================\n");
        printf("1. Cadastrar Cliente (Create)\n");
        printf("2. Listar Clientes (Read)\n");
        printf("3. Atualizar Cliente (Update)\n");
        printf("4. Remover Cliente (Delete)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: criar_cliente(); break;
            case 2: listar_clientes(); break;
            case 3: atualizar_cliente(); break;
            case 4: deletar_cliente(); break;
            case 0: printf("Retornando...\n"); break;
            default: printf("[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void menu_quadras() {
    int opcao;
    do {
        printf("\n=================================\n");
        printf("       GESTAO DE QUADRAS (CRUD)   \n");
        printf("=================================\n");
        printf("1. Cadastrar Quadra (Create)\n");
        printf("2. Listar Quadras (Read)\n");
        printf("3. Atualizar Quadra (Update)\n");
        printf("4. Remover Quadra (Delete)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: criar_quadra(); break;
            case 2: listar_quadras(); break;
            case 3: atualizar_quadra(); break;
            case 4: deletar_quadra(); break;
            case 0: printf("Retornando...\n"); break;
            default: printf("[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

void menu_reservas() {
    int opcao;
    do {
        printf("\n=================================\n");
        printf("       GESTAO DE RESERVAS        \n");
        printf("=================================\n");
        printf("1. Agendar Horario (Cliente x Quadra)\n");
        printf("2. Listar Agendamentos\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: criar_reserva(); break;
            case 2: listar_reservas(); break;
            case 0: printf("Retornando...\n"); break;
            default: printf("[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// ============================================================================
// 7. FUNCAO PRINCIPAL (MAIN)
// ============================================================================

int main() {
    int opcao;

    do {
        printf("\n=========================================\n");
        printf("     ARENA OS - SISTEMA DE GESTAO (C)    \n");
        printf("=========================================\n");
        printf("1. Menu de Clientes (CRUD 1)\n");
        printf("2. Menu de Quadras (CRUD 2)\n");
        printf("3. Menu de Reservas (Estrutura 3)\n");
        printf("0. Sair do Programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menu_clientes(); break;
            case 2: menu_quadras(); break;
            case 3: menu_reservas(); break;
            case 0: printf("\nPrograma encerrado com sucesso.\n"); break;
            default: printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}