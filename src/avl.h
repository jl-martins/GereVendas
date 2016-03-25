typedef struct TCD_AVL* AVL;  
typedef ValorNodo void*;

/* API para trabalhar com AVL's */
AVL criaAVL();
AVL insere(AVL arvore, ValorNodo val);
AVL apagaAVL(AVL arvore);
int existeAVL(AVL arv, ValorNodo val);
ValorNodo criaValorNodo(void * val);

