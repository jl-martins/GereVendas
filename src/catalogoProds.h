#ifndef CATALOGO_PRODS_H
#define CATALOGO_PRODS_H

#include "produto.h" /* definição do tipo Produto */
#include "bool.h"
#include "LStrings.h"

typedef struct catProds* CatProds;

/** Aloca espaço para um Catalogo de produtos e inicializa-o */
CatProds criaCatProds();

/** Insere um produto num catálogo de produtos */
CatProds insereProduto(CatProds, Produto);

/** Testa se um produto existe num catálogo de produtos */
bool existeProduto(CatProds, Produto);

/** Devolve o número total de produtos de um catálogo de produtos */
int totalProdutos(CatProds);

/** Devolve o número total de produtos de um catálogo de produtos
  * cujo código começa pela letra especificada */
int totalProdutosLetra(CatProds, char);

/** Liberta a memória alocada para um catálogo de produtos */
CatProds apagaCatProds(CatProds);

/** Devolve uma lista de strings com os produtos
 * cujo código começa com a letra especificada. */
LStrings prodsPorLetra(CatProds, char);

#endif
