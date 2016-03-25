#include "avl.h"

typedef enum fatorBalanco {ESQ, EQ, DIR} fatorBalnco; 

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	ValorNodo valor;
	fatorBalanco equilibrio;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
} TCD_AVL;

//lembrete: por a static as funcoes privadas */
