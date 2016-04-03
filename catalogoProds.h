#include <avl.h>

typedef struct prods * CatProds;

typedef struct prod * Produto;

typedef struct listprods * Lista_Prods;

CatProds initCatProds();
CatProds insereProduto(CatProds, Produto);
Boolean existeProduto(CatProds, Produto);
int totalProdutos(CatProds);
int totalProdutosLetra(CatProds, char l);
void removeCatProds(CatProds);
