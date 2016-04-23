/**
 * @file catalogoProds.h
 * @author LI3_Grupo1
 * @date 23 Apr 2016
 * @brief Ficheiro com as funções e TADs do catálogo de produtos do projeto @verbatim gereVendas @endverbatim.
 *
 * Este ficheiro contém todas as funções e tipos abstratos de dados associados
 * ao catálogo de produtos do projeto @verbatim gereVendas @endverbatim. As funções
 * referenciadas neste .h permitem criar/apagar catálogos de produtos cujo código começa
 * começa por uma letra maiúscula, bem como inserir produtos nesse catálogo e obter várias
 * informações sobre o catálogo e os seus conteúdos.
 */

#ifndef CATALOGO_PRODS_H
#define CATALOGO_PRODS_H

#include "produto.h" /* definição do tipo Produto */
#include "bool.h"
#include "LStrings.h"

/** Catálogo de produtos cujo código começa por uma letra maiúscula. */
typedef struct catProds* CatProds;

/** @brief Aloca espaço para um catálogo de produtos e inicializa-o */
CatProds criaCatProds();

/** @brief Insere um produto num catálogo de produtos */
CatProds insereProduto(CatProds, Produto);

/** @brief Testa se um produto existe num catálogo de produtos */
bool existeProduto(CatProds, Produto);

/** @brief Devolve o número total de produtos de um catálogo de produtos */
int totalProdutos(CatProds);

/**
 * @brief Devolve o número total de produtos de um catálogo de produtos
 * cujo código começa pela letra especificada
 */
int totalProdutosLetra(CatProds, char);

/** @brief Liberta a memória alocada para um catálogo de produtos */
CatProds apagaCatProds(CatProds);

/**
 * @brief Devolve uma lista de strings navegável, com os códigos 
 * dos produtos começados pela letra especificada no 2º argumento.
 */
LStrings prodsPorLetra(CatProds, char);

#endif
