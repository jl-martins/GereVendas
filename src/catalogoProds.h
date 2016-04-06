#include <avl.h>

typedef Cprods* CatProds;

typedef Prod* Produto;

typedef Prods* Lista_Prods;

typedef Prods* Conjunto_Prods;

CatProds initCatProds();
CatProds insereProduto(CatProds, Produto);
Boolean existeProduto(CatProds, Produto);
int totalProdutos(CatProds);
int totalProdutosLetra(CatProds, char l);
void removeCatProds(CatProds);
Conjunto_Prods conjProdutos(CatProds, char l);
Lista_Prods listaProdutos(CatProds, char l);
