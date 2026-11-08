# Arena H.O.S. — Logica e continuidade

Este documento descreve a implementacao completa, os contratos entre os modulos e os cuidados para que o projeto possa ser mantido por seus integrantes atuais ou por outra equipe.

## Principios da implementacao

- O sistema e escrito exclusivamente em C (C99).
- Nao ha variaveis globais. O estado fica em uma unica estrutura `Sistema`, criada em `main` e passada explicitamente as funcoes.
- Os vetores de clientes, quadras e reservas sao dinamicos.
- A interface e textual e utiliza ASCII.
- Os dados persistem em tres arquivos binarios separados, um para cada vetor de estrutura.

## Modelo de dados

As estruturas ficam em `dados.h`:

- `Cliente`: ID interno, CPF unico, nome, telefone e idade.
- `Quadra`: ID interno, codigo unico, nome/tipo e preco por hora.
- `Reserva`: ID interno, ID do cliente, ID da quadra, data e horario.

Cada registro possui ID gerado automaticamente. CPF e codigo da quadra tambem sao unicos. Uma reserva relaciona as duas estruturas independentes por seus IDs; seus dados associados sao procurados nos vetores quando exibidos ou incluidos em relatorios.

## Estado e alocacao

`Sistema` guarda ponteiros para cada vetor, quantidade de registros, capacidade alocada e proximo ID de cada tipo. `sistema_inicializar` cria os vetores e os contadores; `sistema_liberar` libera todas as alocacoes.

`sistema_ajustar_capacidade` cresce os vetores em incrementos definidos por `INCREMENTO_CAPACIDADE`. Antes de uma inclusao, garante espaco. Antes de uma remocao, reduz a capacidade quando a remocao deixaria mais de cinco posicoes livres. Assim, quantidade e capacidade permanecem coordenadas sem variaveis globais.

O cadastro prepara e valida os campos antes de alterar a quantidade ou consumir um ID. Uma remocao de cliente ou quadra verifica as reservas relacionadas antes de alterar o vetor. Depois de uma remocao valida, os registros seguintes sao deslocados para manter os elementos contiguos.

## Fluxo de inicializacao e encerramento

1. `main` inicializa `Sistema`.
2. `carregar_dados` le os tres arquivos, aloca espaco conforme a quantidade de registros e valida IDs, campos unicos e relacionamentos.
3. Se nenhum arquivo existir, o programa inicia um banco novo. Se apenas parte do conjunto existir ou algum arquivo estiver invalido, a inicializacao falha explicitamente.
4. `executar_menu` recebe `Sistema *` e encaminha as escolhas para os modulos.
5. Operacoes que alteram o banco chamam `salvar_dados` imediatamente.
6. Ao sair, `main` tenta salvar novamente e libera os vetores.

Cada arquivo possui assinatura e versao, seguido da quantidade, do proximo ID e dos registros. O formato e binario e depende da definicao das estruturas e do compilador utilizado; antes de alterar estruturas ou formato, mantenha copia de seguranca e planeje uma migracao ou uma nova versao dos arquivos.

## Modulos e logica das funcoes

### `clientes.c`

- `buscar_cliente_por_id` e `buscar_cliente_por_cpf` retornam o indice ou `-1`.
- `cadastrar_cliente` le CPF, nome, telefone e idade; valida CPF com onze digitos e unicidade; ajusta o vetor; atribui ID e insere.
- `listar_clientes` mostra todos os registros.
- `consultar_cliente` e `atualizar_cliente` permitem localizar por ID, CPF ou nome exato.
- `remover_cliente` usa a mesma selecao de busca e rejeita a operacao se existir reserva vinculada.

### `quadras.c`

- `buscar_quadra_por_id` e `buscar_quadra_por_codigo` retornam indice ou `-1`.
- `cadastrar_quadra` valida o codigo unico, o nome/tipo e preco nao negativo antes de inserir.
- `listar_quadras` mostra todos os registros.
- `consultar_quadra` e `atualizar_quadra` permitem localizar por ID, codigo ou nome exato.
- `remover_quadra` rejeita a operacao se existir reserva vinculada.

### `reservas.c`

- `buscar_reserva_por_id` localiza uma reserva pelo ID.
- `cadastrar_reserva` confirma a existencia de cliente e quadra, valida data/horario e rejeita conflito para a mesma quadra, data e horario.
- `listar_reservas` mostra os campos da reserva e os nomes relacionados.
- `consultar_reserva`, `atualizar_reserva` e `remover_reserva` compoem o CRUD. A alteracao revalida os vinculos e a disponibilidade antes de substituir o registro.
- `filtrar_reservas` oferece tres criterios: data minima, nome do cliente e nome da quadra.

### `persistencia.c`

- `carregar_dados` carrega `clientes.bin`, `quadras.bin` e `reservas.bin`.
- `salvar_dados` grava um arquivo por vetor e preserva os contadores de ID.
- A leitura de todos os arquivos e validada antes da aplicacao utilizar as relacoes.
- Erros de leitura, formato, relacao, alocacao e gravacao sao apresentados ao usuario/terminal.

### `relatorios.c`

`gerar_relatorio_reservas` permite gerar um de tres relatorios em `relatorio_reservas.txt`: reservas por data (da data informada ate a data atual), por ID de cliente ou por ID de quadra. Cada reserva inclui todos os campos de cliente e quadra associados.

### `menu.c` e `entrada.c`

`menu.c` controla os menus e grava o banco apos uma operacao mutavel bem-sucedida. `entrada.c` concentra leitura de texto e numeros, verificacao de datas reais no formato `DD/MM/AAAA` e comparacao cronologica.

## Garantias de integridade

- CPF de cliente e codigo de quadra nao se repetem, inclusive quando alterados.
- IDs gerados e contadores persistidos evitam reutilizacao apos remocoes ou reinicializacoes.
- Reserva exige cliente e quadra existentes.
- Uma quadra nao pode ter duas reservas no mesmo dia e horario.
- Cliente/quadra com reserva nao pode ser removido; primeiro deve-se remover ou alterar a reserva.
- Alteracoes de reserva validam os novos relacionamentos e conflito antes de substituir os dados atuais.

## Relatorios e dados de demonstracao

O arquivo de relatorio e texto e pode ser aberto em qualquer editor. Os arquivos `.bin` sao dados de execucao do proprio programa, nao fontes nem executaveis. A entrega final deve manter juntos os tres arquivos binarios correspondentes ao mesmo banco e incluir ao menos dez registros de cada estrutura.

## Compilar e validar

Compile com avisos habilitados:

gcc -std=c99 -Wall -Wextra -pedantic main.c menu.c entrada.c clientes.c quadras.c reservas.c relatorios.c persistencia.c sistema.c -o arena

Ao alterar o projeto, teste:

1. cadastro acima de cinco registros para verificar realocacao;
2. remocao depois do crescimento para verificar reducao e limite de espacos livres;
3. chaves duplicadas em cadastro e alteracao;
4. reservas com IDs inexistentes e conflitos de data/horario;
5. tentativa de remover cliente ou quadra ainda referenciados;
6. as tres formas de consulta, tres filtros e tres tipos de relatorio;
7. encerramento e reinicio para verificar persistencia e proximo ID;
8. arquivos binarios ausentes, incompletos ou incompativeis.

## Evolucao posterior

O escopo descrito no enunciado inclui tambem mais validacao de entradas e testes. Se novas funcionalidades forem acrescentadas, conserve interfaces declaradas nos `.h`, mantenha o estado passado por parametros e atualize a versao/formato dos arquivos quando a estrutura binaria mudar. Criar uma copia de seguranca dos tres arquivos antes de qualquer alteracao incompativel.
