#include <stdio.h>
#include <string.h>
#include "entrada.h"

static void descartar_resto_linha(void) {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

int ler_inteiro(const char *mensagem, int *valor) {
    char linha[100];
    char extra;
    int resultado;

    while (1) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return 0;
        }

        if (strchr(linha, '\n') == NULL && !feof(stdin)) {
            descartar_resto_linha();
        }

        resultado = sscanf(linha, " %d %c", valor, &extra);
        if (resultado == 1) {
            return 1;
        }

        printf("[ERRO] Digite um numero inteiro valido.\n");
    }
}

int ler_real(const char *mensagem, float *valor) {
    char linha[100];
    char extra;
    int resultado;

    while (1) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return 0;
        }

        if (strchr(linha, '\n') == NULL && !feof(stdin)) {
            descartar_resto_linha();
        }

        resultado = sscanf(linha, " %f %c", valor, &extra);
        if (resultado == 1 && *valor >= 0.0f && *valor <= 100000000.0f) {
            return 1;
        }

        printf("[ERRO] Digite um valor valido e nao negativo (use ponto decimal).\n");
    }
}

int ler_texto(const char *mensagem, char texto[], int tamanho) {
    size_t comprimento;

    while (1) {
        printf("%s", mensagem);
        if (fgets(texto, tamanho, stdin) == NULL) {
            return 0;
        }

        comprimento = strlen(texto);
        if (comprimento > 0 && texto[comprimento - 1] == '\n') {
            texto[comprimento - 1] = '\0';
        } else if (!feof(stdin)) {
            descartar_resto_linha();
            printf("[ERRO] Texto muito longo. Tente novamente.\n");
            continue;
        }

        if (texto[0] != '\0') {
            return 1;
        }

        printf("[ERRO] O campo nao pode ficar vazio.\n");
    }
}

static int todos_digitos(const char texto[], int inicio, int quantidade) {
    int i;

    for (i = inicio; i < inicio + quantidade; i++) {
        if (texto[i] < '0' || texto[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int data_valida(const char data[TAM_DATA]) {
    int dia;
    int mes;
    int ano;
    int dias_no_mes;

    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/' ||
        !todos_digitos(data, 0, 2) ||
        !todos_digitos(data, 3, 2) ||
        !todos_digitos(data, 6, 4)) {
        return 0;
    }

    dia = (data[0] - '0') * 10 + data[1] - '0';
    mes = (data[3] - '0') * 10 + data[4] - '0';
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 +
          (data[8] - '0') * 10 + data[9] - '0';

    if (ano < 1900 || mes < 1 || mes > 12) {
        return 0;
    }

    dias_no_mes = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        dias_no_mes = 30;
    } else if (mes == 2) {
        dias_no_mes = 28;
        if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0)) {
            dias_no_mes = 29;
        }
    }

    return dia >= 1 && dia <= dias_no_mes;
}

int ler_data(const char *mensagem, char data[TAM_DATA]) {
    char entrada[TAM_DATA + 2];
    while (1) {
        if (!ler_texto(mensagem, entrada, sizeof(entrada))) {
            return 0;
        }
        if (data_valida(entrada)) {
            memcpy(data, entrada, TAM_DATA);
            return 1;
        }
        printf("[ERRO] Use uma data valida no formato DD/MM/AAAA.\n");
    }
}

int comparar_datas(const char primeira[TAM_DATA], const char segunda[TAM_DATA]) {
    int ano1;
    int ano2;
    int mes1;
    int mes2;
    int dia1;
    int dia2;

    ano1 = (primeira[6] - '0') * 1000 + (primeira[7] - '0') * 100 +
           (primeira[8] - '0') * 10 + primeira[9] - '0';
    ano2 = (segunda[6] - '0') * 1000 + (segunda[7] - '0') * 100 +
           (segunda[8] - '0') * 10 + segunda[9] - '0';
    if (ano1 != ano2) {
        return ano1 < ano2 ? -1 : 1;
    }

    mes1 = (primeira[3] - '0') * 10 + primeira[4] - '0';
    mes2 = (segunda[3] - '0') * 10 + segunda[4] - '0';
    if (mes1 != mes2) {
        return mes1 < mes2 ? -1 : 1;
    }

    dia1 = (primeira[0] - '0') * 10 + primeira[1] - '0';
    dia2 = (segunda[0] - '0') * 10 + segunda[1] - '0';
    if (dia1 == dia2) {
        return 0;
    }
    return dia1 < dia2 ? -1 : 1;
}
