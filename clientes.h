#ifndef CLIENTES_H
#define CLIENTES_H

#include "dados.h"

int buscar_cliente_por_id(const Sistema *sistema, int id);
int buscar_cliente_por_cpf(const Sistema *sistema, const char *cpf);
int cadastrar_cliente(Sistema *sistema);
int consultar_cliente(const Sistema *sistema);
int atualizar_cliente(Sistema *sistema);
int remover_cliente(Sistema *sistema);
void listar_clientes(const Sistema *sistema);

#endif
