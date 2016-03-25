typedef struct TCD_AVL* AVL;  
typedef ValorNodo void*;

/* API para trabalhar com AVL's */
/* necessario passar funcao de comparacao entre elementos da arvore */
AVL criaAVL(int (*compar)(const void *, const void*));
AVL insere(AVL arvore, ValorNodo val);
AVL apagaAVL(AVL arvore);
int existeAVL(AVL arv, ValorNodo val);
ValorNodo criaValorNodo(void * val);

