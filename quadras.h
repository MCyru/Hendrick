#ifndef QUADRAS_H
#define QUADRAS_H

#include "dados.h"

int buscar_quadra_por_id(const Sistema *sistema, int id);
int buscar_quadra_por_codigo(const Sistema *sistema, const char *codigo);
int cadastrar_quadra(Sistema *sistema);
int consultar_quadra(const Sistema *sistema);
int atualizar_quadra(Sistema *sistema);
int remover_quadra(Sistema *sistema);
void listar_quadras(const Sistema *sistema);

#endif
