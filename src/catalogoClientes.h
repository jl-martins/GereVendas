#ifndef CATALOGO_CLIENTES_H
#define CATALOGO_CLIENTES_H

#include "cliente.h" /* definição do tipo Cliente */
#include "bool.h"
#include "LStrings.h"

typedef struct catClientes* CatClientes;

/** Aloca espaco e inicializa um novo Catalogo de Clientes */
CatClientes criaCatClientes();

/** Insere um cliente num catálogo de clientes */
CatClientes insereCliente(CatClientes, Cliente);

/** Testa se um cliente existe num catálogo de clientes */
bool existeCliente(CatClientes, Cliente);

/* Devolve o número total de clientes de um catálogo de clientes */
int totalClientes(CatClientes);

/** Devolve o número total de clientes de um catálogo 
  * de clientes cujo código começa pela letra especificada */
int totalClientesLetra(CatClientes, char);

/** Liberta a memória alocada para um catálogo de clientes */
CatClientes apagaCatClientes(CatClientes);

/** Devolve uma lista com todos os clientes do catálogo. */
Cliente* todosClientes(CatClientes catC);

/** Devolve uma lista de strings com os códigos 
  * dos clientes começados pela letra especificada */
LStrings clientesPorLetra(CatClientes, char);

#endif
