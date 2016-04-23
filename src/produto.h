#ifndef PRODUTO_T 
#define PRODUTO_T 

#define MAX_CODIGO_PROD 16

typedef struct produto* Produto;

/** Cria um porduto com codigo codigoProduto */
Produto criaProduto(char* codigoProduto);

/** Apaga um produto */
Produto apagaProduto(Produto p);

/** Devolve o codigo de um produto */
char* obterCodigoProduto(Produto prod);

/** Devolve a 1a letra do codigo de um produto */
char inicioCodigoProduto(Produto prod);

/** Compara os codigos de dois produtos */
int comparaCodigosProduto(Produto p1, Produto p2);

/** Cria um produto com o mesmo codigo */
Produto duplicaProduto(Produto p);

#endif
