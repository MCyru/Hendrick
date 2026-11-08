# Arena H.O.S.

## Sistema de gestao de quadras esportivas

| Propriedade | Especificacao |
|---|---|
| Produto | Arena Hendrick Organization System (Arena H.O.S.) |
| Categoria | Aplicacao de console para gestao de quadras e reservas |
| Versao | `1.0.0` |
| Plataforma | Windows ou Linux |
| Linguagem | C99 |
| Persistencia | Tres arquivos binarios locais |
| Interface | Menus de console em ASCII |

## Visao geral

O Arena H.O.S. gerencia clientes, quadras esportivas e reservas. Uma reserva relaciona um cliente a uma quadra em uma data e horario. Os dados sao mantidos entre execucoes por arquivos binarios separados.

## Funcionalidades

### Clientes

- Cadastro com CPF unico, nome, telefone e idade.
- Listagem, consulta por ID, CPF ou nome, alteracao e remocao.
- Bloqueio de remocao enquanto houver reservas vinculadas.

### Quadras

- Cadastro com codigo unico, nome/tipo e preco por hora.
- Listagem, consulta por ID, codigo ou nome, alteracao e remocao.
- Bloqueio de remocao enquanto houver reservas vinculadas.

### Reservas

- CRUD completo com associacao a cliente e quadra existentes.
- Validacao de data (`DD/MM/AAAA`) e horario (`0` a `23`).
- Bloqueio de conflito para a mesma quadra, data e horario.
- Consulta por ID, data, cliente ou quadra.
- Filtros por data minima, nome do cliente ou nome da quadra.
- Tres relatorios textuais: por periodo, cliente ou quadra. Cada registro do relatorio inclui todos os campos da reserva e das estruturas relacionadas.

## Regras e armazenamento

- Os IDs sao gerados pelo sistema, persistidos e nunca reutilizados.
- CPF de cliente e codigo de quadra nao podem se repetir.
- Os vetores de estruturas usam alocacao dinamica. A capacidade cresce em blocos de cinco e e reduzida quando necessario para nao manter mais de cinco posicoes livres.
- Nao ha variaveis globais; o estado e mantido na estrutura `Sistema` inicializada em `main` e passada por parametro.
- Nenhum registro de cliente ou quadra pode ser removido enquanto uma reserva relacionada existir.
- A remocao de uma reserva libera a relacao, permitindo remover posteriormente cliente ou quadra.

Os arquivos `clientes.bin`, `quadras.bin` e `reservas.bin` sao criados no diretorio de trabalho do programa. Em primeira execucao, quando os tres ainda nao existem, o sistema inicia vazio. Um conjunto incompleto, truncado ou incompativel e reportado como erro. Os arquivos devem ser mantidos juntos e nao devem ser editados manualmente. A aplicacao gera `relatorio_reservas.txt` quando um relatorio e solicitado.

## Componentes

| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Inicializa o sistema, carrega os dados, inicia os menus e libera memoria |
| `dados.h` | Estruturas, limites de campos e estado do sistema |
| `sistema.c` / `sistema.h` | Alocacao, redimensionamento e liberacao dos vetores |
| `clientes.c` / `clientes.h` | CRUD, busca e regras de clientes |
| `quadras.c` / `quadras.h` | CRUD, busca e regras de quadras |
| `reservas.c` / `reservas.h` | CRUD, filtros e regras de reservas |
| `persistencia.c` / `persistencia.h` | Leitura e gravacao dos tres arquivos binarios |
| `relatorios.c` / `relatorios.h` | Geracao do relatorio textual de reservas |
| `menu.c` / `menu.h` | Navegacao e interface do usuario |
| `entrada.c` / `entrada.h` | Leitura, validacao de campos e comparacao de datas |
| `.vscode/tasks.json` | Tarefa de compilacao para o Visual Studio Code |

## Compilacao

Com GCC, a partir da pasta que contem os arquivos-fonte:

gcc -std=c99 -Wall -Wextra -pedantic main.c menu.c entrada.c clientes.c quadras.c reservas.c relatorios.c persistencia.c sistema.c -o arena

No Windows, o executavel gerado pode ser `arena.exe`.

## Arquivos de dados para entrega

A pasta de entrega inclui os arquivos binarios pre-preenchidos gerados pela execucao do sistema, com pelo menos dez clientes, dez quadras e dez reservas. Para consultar como as funcoes se relacionam e como continuar o desenvolvimento, veja [CONTINUIDADE_DO_PROJETO.md](./CONTINUIDADE_DO_PROJETO.md).
