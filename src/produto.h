#ifndef PRODUTO_T 
#define PRODUTO_T 

typedef struct produto * Produto;

Produto criaProduto(char * codigoProduto);
char* obterCodigoProduto(Produto prod);

#endif
