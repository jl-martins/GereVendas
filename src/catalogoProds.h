#ifndef CATALOGO_PRODS_H
#define CATALOGO_PRODS_H

#include "produto.h"

/* tipos exportados */
typedef struct catProds* CatProds;
typedef struct conjProds* ConjuntoProds;

/* funções sobre CatProds */
CatProds criaCatProds();
CatProds insereProduto(CatProds, Produto);
bool existeProduto(CatProds, Produto);
int totalProdutos(CatProds);
int totalProdutosLetra(CatProds, char);
void removeCatProds(CatProds);
ConjuntoProds prodsPorLetra(CatProds, char);

#endif