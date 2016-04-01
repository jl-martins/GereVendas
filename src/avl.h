typedef struct TCD_AVL* AVL;  
typedef void* ValorNodo;

/* API para trabalhar com AVL's */
/* necessario passar funcao de comparacao entre elementos da arvore */
/* nota: se o valor for <0, o 1o argumento é o menor, se for 0 sao iguais, >0 => 2o e o menor*/
AVL criaAVL(int (*compar)(const ValorNodo, const ValorNodo));
AVL insere(AVL arvore, ValorNodo val);
AVL apagaAVL(AVL arvore);
int existeAVL(AVL arv, ValorNodo val);
ValorNodo criaValorNodo(void * val);
