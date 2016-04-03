#include "avl.h"
#include <stdlib.h>
/*definir funçoes para travessias!! e toString*/
typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	ValorNodo valor;
	FatorBalanco equilibrio;
	int nNodos;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao de comparacao entre nodos - se o primeiro < segundo, o valor deve ser negativo */
	int (*compara)(const ValorNodo, const ValorNodo);
} TCD_AVL;

//lembrete: por a static as funcoes privadas 

// ver o que fazer quando falha
AVL criaAVL(int (*compar)(const ValorNodo, const ValorNodo), ){
	AVL nova = (AVL) malloc(sizeof(TCD_AVL));
	if(nova == NULL) return NULL;
	nova -> nNodos = 0;
	nova -> raiz = NULL;
	nova -> compara = compar;
	return nova;
}

static AVL_NODO* rodaDireita(AVL_NODO* t){
	AVL_NODO* aux;
	if ((! t) || (! t -> esquerda)) {
		/* inserir código de tratamento de erros */
	} else {
		aux = t -> esquerda;
		t -> esquerda = aux -> direita;
		aux -> direita = t;
		t = aux;
	}
	return t;
}

static AVL_NODO* equilibraDireita(AVL_NODO * t){
	if (t -> direita -> fatorBalanco == DIR) {
		// Rotacao simples a esquerda
		t = rodaEsquerda(t);
		t -> fatorBalanco = EQ;
		t -> esquerda -> fatorBalanco = EQ;
	} else {
		//Dupla rotacao
		t -> direita = rodaDireita(t -> direita);
		t = rodaEsquerda(t);
		switch (t -> fatorBalanco) {
			case EQ:
				t -> esquerda -> fatorBalanco = EQ;
				t -> direita -> fatorBalanco = EQ;
				break;
			case ESQ:
				t -> esquerda -> fatorBalanco = EQ;
				t -> direita -> fatorBalanco = DIR;
				break;
			case DIR:
				t -> esquerda -> fatorBalanco = ESQ;
				t -> direita -> fatorBalanco = EQ;
		}
		t -> fatorBalanco = EQ;
	}
	return t;
}

static AVL_NODO* insereDireita(AVL_NODO * raiz, ValorNodo val, int (*compara) (void*, void*), int * cresceu){
	raiz -> direita = insereNodo(raiz -> direita, val, compara, cresceu);
	if(*cresceu){
		switch(raiz -> fatorBalanco){
			case ESQ:
				raiz -> fatorBalanco = EQ;
				*cresceu = 0;
				break;
			case EQ:
				raiz -> fatorBalanco = DIR;
				break;
			case DIR:
				raiz = equilibraDireita(raiz);
				*cresceu = 0;
				break;
		}
	}
	return raiz;
}

static AVL_NODO* insereNodo(AVL_NODO * raiz, ValorNodo val, int (*compara) (void *, void *), int * cresceu){
	AVL_NODO * ret;
	if(raiz == NULL){
		ret = raiz = (AVL_NODO *) malloc(sizeof(AVL_NODO));
		raiz -> valor;	
		raiz -> esquerda = raiz -> direita = NULL;
		raiz -> fatorBalanco = EQ;
		*cresceu = 1;
	}
	else if(compara(val, raiz -> valor) < 0){
		/* se raiz->valor > val */
		ret = insereEsquerda(raiz, val, compara, cresceu);
	else ret = insereDireira(raiz, val, compara, cresceu);	 
	return ret;
}	

/* ver qual deve ser o tipo de retorno */
AVL insere(AVL arvore, ValorNodo val){
	int cresceu;
	if(arvore == NULL){
		fprintf(stderr, "Necessário inicializar árvore");
		return -1;
	}else{
		arvore -> nNodos++;		
		arvore -> raiz = insereNodo(AVL_NODO * raiz, val, arvore -> compara, &cresceu);	
	return arvore;
}

ValorNodo inorder(AVL arvore){
	int tamanho = arvore -> nNodos;	
}


