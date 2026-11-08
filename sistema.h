#ifndef SISTEMA_H
#define SISTEMA_H

#include "dados.h"

int sistema_inicializar(Sistema *sistema);
void sistema_liberar(Sistema *sistema);
int sistema_ajustar_capacidade(Sistema *sistema, int estrutura, size_t necessaria);
int sistema_salvar_apos_alteracao(Sistema *sistema);

#endif
