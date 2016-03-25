enum fatorBalanco {esq, eq, dir};

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	void * valor;
	fatorBalanco equilibrio;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
} TCD_AVL;
