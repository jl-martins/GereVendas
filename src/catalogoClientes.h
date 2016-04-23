/**
 * @file catalogoClientes.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com as funções e TADs do catálogo de clientes do projeto @verbatim gereVendas @endverbatim.
 *
 * Este ficheiro contém todas as funções e tipos abstratos de dados associados
 * ao catálogo de clientes do projeto @verbatim gereVendas @endverbatim. As funções
 * referenciadas neste .h permitem criar/apagar catálogos de clientes cujo código começa
 * começa por uma letra maiúscula, bem como inserir clientes nesse catálogo e obter várias
 * informações sobre o catálogo e os seus conteúdos.
 */

#ifndef CATALOGO_CLIENTES_H
#define CATALOGO_CLIENTES_H

#include "cliente.h" /* definição do tipo Cliente */
#include "bool.h"
#include "LStrings.h"

/** Catálogo de clientes cujo código começa por uma letra maiúscula. */
typedef struct catClientes* CatClientes;

/** @brief Aloca espaco e inicializa um novo catálogo de clientes. */
CatClientes criaCatClientes();

/** @brief Insere um cliente num catálogo de clientes. */
CatClientes insereCliente(CatClientes, Cliente);

/** @brief Testa se um cliente existe num catálogo de clientes. */
bool existeCliente(CatClientes, Cliente);

/* @brief Devolve o número total de clientes de um catálogo de clientes. */
int totalClientes(CatClientes);

/** 
 * @brief Devolve o número total de clientes de um catálogo de clientes
 * cujo código começa pela letra especificada.
 */
int totalClientesLetra(CatClientes, char);

/** @brief Liberta a memória alocada para um catálogo de clientes. */
CatClientes apagaCatClientes(CatClientes);

/** Devolve uma lista com todos os clientes do catálogo. */
Cliente* todosClientes(CatClientes catC);

/**
 * @brief Devolve uma lista de strings navegável, com os códigos 
 * dos clientes começados pela letra especificada no 2º argumento.
 */
LStrings clientesPorLetra(CatClientes, char);

#endif
