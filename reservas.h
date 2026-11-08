#ifndef RESERVAS_H
#define RESERVAS_H

#include "dados.h"

int buscar_reserva_por_id(const Sistema *sistema, int id);
int cadastrar_reserva(Sistema *sistema);
int consultar_reserva(const Sistema *sistema);
int atualizar_reserva(Sistema *sistema);
int remover_reserva(Sistema *sistema);
void listar_reservas(const Sistema *sistema);
void filtrar_reservas(const Sistema *sistema);

#endif
