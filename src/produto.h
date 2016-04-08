#ifndef PRODUTO_T 
#define PRODUTO_T 

typedef struct produto* Produto;

Produto criaProduto(char* codigoProduto);
void removeProduto(Produto p);
char* obterCodigoProduto(Produto prod);
char inicioCodigoProduto(Produto prod);

#endif
