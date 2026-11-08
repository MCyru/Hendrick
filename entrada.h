#ifndef ENTRADA_H
#define ENTRADA_H

#include "dados.h"

int ler_inteiro(const char *mensagem, int *valor);
int ler_real(const char *mensagem, float *valor);
int ler_texto(const char *mensagem, char texto[], int tamanho);
int ler_data(const char *mensagem, char data[TAM_DATA]);
int data_valida(const char data[TAM_DATA]);
int comparar_datas(const char primeira[TAM_DATA], const char segunda[TAM_DATA]);

#endif
