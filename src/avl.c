#include "avl.h"
#include <stdlib.h>

typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	ValorNodo valor;
	FatorBalanco equilibrio;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao de comparacao entre nodos */
	int (*compara)(const void*, const void*);
} TCD_AVL;

//lembrete: por a static as funcoes privadas 

// ver o que fazer quando falha
AVL criaAVL(int (*compar)(const void *, const void*)){
	AVL nova = (AVL) malloc(sizeof(TCD_AVL));
	if(nova == NULL) return NULL;
	nova -> raiz = NULL;
	nova -> compara = compar;
	return nova;
}

AVL insere(AVL arvore, ValorNodo val){
	if(arvore == NULL) return NULL;
	int comparador(const void *, const void *);
	
	comparador = arvore -> compara;
			

}
