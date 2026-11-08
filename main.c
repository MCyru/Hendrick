#include <stdio.h>
#include "menu.h"
#include "persistencia.h"
#include "sistema.h"

int main(void) {
    Sistema sistema;
    int sucesso;

    if (!sistema_inicializar(&sistema)) {
        sistema_liberar(&sistema);
        return 1;
    }

    if (!carregar_dados(&sistema)) {
        sistema_liberar(&sistema);
        return 1;
    }

    executar_menu(&sistema);
    sucesso = salvar_dados(&sistema);
    sistema_liberar(&sistema);
    return sucesso ? 0 : 1;
}
