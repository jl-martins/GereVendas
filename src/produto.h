#ifndef PRODUTO_T 
#define PRODUTO_T 

#define MAX_CODIGO_PROD 8

typedef struct produto* Produto;

Produto criaProduto(char* codigoProduto);
void removeProduto(Produto p);
char* obterCodigoProduto(Produto prod);
char inicioCodigoProduto(Produto prod);
int comparaCodigosProduto(Produto p1, Produto p2);

#endif
