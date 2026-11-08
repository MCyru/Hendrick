Readme.ver{0.8.4] -#prickett.def
# Arena Hendrick Organization System

## Sistema de gestão de quadras esportivas

| Propriedade | Especificação |
|---|---|
| Produto | Arena H.O.S |
| Categoria | Aplicação de console para gestão de quadras e reservas |
| Versão | `0.1.0-dev` |
| Plataforma | Windows ou Linux com compilador C |
| Linguagem | C padrão C99 |
| Interface | Console, com menus e elementos gráficos em ASCII |
| Persistência | Memória volátil durante a execução |

## Visão geral

O Arena OS é uma aplicação acadêmica de console para organizar clientes, quadras esportivas e reservas de horários. O sistema reúne operações de cadastro e consulta em menus textuais e associa cada reserva a um cliente e a uma quadra existentes.

A versão `0.1.0-dev` representa o estado de desenvolvimento correspondente à primeira entrega do projeto. A versão de entrega poderá ser identificada como `0.1.0`.

## Escopo funcional

### Clientes

- Cadastrar clientes com identificador, nome e telefone.
- Listar clientes ativos.
- Atualizar os dados de um cliente.
- Remover logicamente clientes sem reservas ativas.

### Quadras

- Cadastrar quadras com identificador, nome ou tipo e preço por hora.
- Listar quadras ativas.
- Atualizar nome ou preço.
- Remover logicamente quadras sem reservas ativas.

### Reservas

- Criar uma reserva associando um cliente ativo a uma quadra ativa.
- Registrar o horário da reserva em horas inteiras, de `0` a `23`.
- Impedir mais de uma reserva ativa para a mesma quadra e horário.
- Listar reservas com as informações relacionadas de cliente e quadra.
- Cancelar uma reserva por identificador.

## Regras de negócio{Regras de execução atual}

--------------------------------------------------Ainda não Implementado----------------------------------

## Arquitetura da aplicação

O programa é dividido por responsabilidade. As estruturas, vetores e contadores são inicializados localmente em `main` e passados às funções que os utilizam. O projeto não possui variáveis globais nem utiliza alocação dinâmica nesta versão.

-# Planejado:
| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Inicialização dos dados e entrada no sistema de menus |
| `dados.h` | Estruturas `Cliente`, `Quadra`, `Reserva` e capacidade máxima |
| `clientes.c` / `clientes.h` | Operações e busca de clientes |
| `quadras.c` / `quadras.h` | Operações e busca de quadras |
| `reservas.c` / `reservas.h` | Cadastro, consulta e cancelamento de reservas |
| `menu.c` / `menu.h` | Menus principal e de cada domínio |
| `entrada.c` / `entrada.h` | Leitura e validação de dados digitados |

## Especificação técnica

--------------------------------------------------Ainda não Implementado----------------------------------

## Limites desta versão

--------------------------------------------------Ainda não Implementado----------------------------------

## Compilação

Com GCC:

--------------------------------------------------Ainda não Implementado----------------------------------

No Windows, o executável vai ser gerado como `arena.exe`.

## Contexto acadêmico

--------------------------------------------------Ainda não Implementado----------------------------------

## Documentação de continuidade

--------------------------------------------------Ainda não Implementado----------------------------------