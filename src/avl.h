typedef struct TCD_AVL* AVL;  
typedef void* ValorNodo;

/* Cria uma AVL vazia, que vai utilizar a função de comparação 'compar' */
AVL criaAVL(int (*compar)(const void *,const void *));
/* Insere um valor na AVL 'arvore' */
AVL insere(AVL arvore, ValorNodo val);
/* Testa se o valor 'val' ocorre na AVL 'arvore' */
int existeAVL(const AVL arvore, ValorNodo val);
/* Apaga uma AVL (i.e.: liberta toda a memória alocada para a mesma) */
AVL apagaAVL(AVL arvore);
/* Devolve o número de nodos de uma AVL */
int tamanho(const AVL arvore);
/* Devolve a altura de uma AVL */
int altura(const AVL arvore);
