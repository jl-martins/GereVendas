#ifndef PRODUTO_T 
#define PRODUTO_T 

#define MAX_CODIGO_PROD 16

typedef struct produto* Produto;

Produto criaProduto(char* codigoProduto);
void apagaProduto(Produto p);
char* obterCodigoProduto(Produto prod);
char inicioCodigoProduto(Produto prod);
int comparaCodigosProduto(Produto p1, Produto p2);
Produto duplicaProduto(Produto p);

#endif
