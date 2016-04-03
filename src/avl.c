#include "avl.h"
#include <stdlib.h>
/*definir funçoes para travessias!! e toString*/
typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	ValorNodo valor;
	FatorBalanco fatorBalanco;
	int nNodos;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao de comparacao entre nodos - se o primeiro < segundo, o valor deve ser negativo */
	int (*compara)(const void*, const void*);
} TCD_AVL;

//lembrete: por a static as funcoes privadas 
/* Protótipos das funções privadas ao ficheiro */
static AVL_NODO* rodaDireita(AVL_NODO* t);
static AVL_NODO* equilibraDireita(AVL_NODO * t);
static AVL_NODO* insereDireita(AVL_NODO * raiz, ValorNodo val, int (*compara) (void*, void*), int * cresceu);
static AVL_NODO* insereNodo(AVL_NODO * raiz, ValorNodo val, int (*compara) (void *, void *), int * cresceu);
static AVL_NODO equilibraEsquerda(AVL_NODO *raiz);
static AVL rodaEsquerda(AVL_NODO *raiz);
static AVL_NODO equilibraEsquerda(AVL_NODO *raiz);
static AVL_NODO* insereEsquerda(AVL_NODO *raiz, ValorNodo val, int (*compara) (void *, void *), int *cresceu);
static int alturaAux(const AVL_NODO *raiz);



// ver o que fazer quando falha
AVL criaAVL(int (*compar)(const void*, const void*)){
	AVL nova = (AVL) malloc(sizeof(TCD_AVL));
	if(nova == NULL) return NULL;
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
		/* Podemos usar IO nos modulos?? */
		//fprintf(stderr, "Necessário inicializar árvore");
		return -1;
	}else{
		arvore -> nNodos++;		
		arvore -> raiz = insereNodo(AVL_NODO * raiz, val, arvore -> compara, &cresceu);	
	return arvore;
}

static AVL_NODO equilibraEsquerda(AVL_NODO *raiz){

	if(raiz->esquerda->fatorBalanco == ESQ){
		/* rotação simples à direita */
		raiz = rodaDireita(raiz);
		raiz->fatorBalanco = EQ;
		raiz->direita->fatorBalanco = EQ;
	}
	else{
		/* Dupla rotação */
		raiz->esquerda = rodaEsquerda(raiz->esquerda);
		raiz = rodaDireita(raiz);

		switch(raiz->fatorBalanco){
			case EQ:
				raiz->esquerda->fatorBalanco = EQ;
				raiz->direita->fatorBalanco = EQ;
				break;
			case ESQ:
				t->direita->fatorBalanco = EQ;
				t->esquerda->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->direita->fatorBalanco = DIR;
				raiz->esquerda->fatorBalanco = EQ;
				break;
		}
		raiz->fatorBalanco = EQ;
	}
	return raiz;
}

// JM
static AVL rodaEsquerda(AVL_NODO *raiz){
	AVL_NODO *aux;

	if(!raiz || !raiz->direita){
		/* tratamento de erros */
	}
	else{
		aux = raiz->direita;
		raiz->direita = aux->esquerda;
		aux->esquerda = raiz;
		raiz = aux;
	}

	return raiz;
}

static AVL_NODO equilibraEsquerda(AVL_NODO *raiz){

	if(raiz->esquerda->fatorBalanco == ESQ){
		/* rotação simples à direita */
		raiz = rodaDireita(raiz);
		raiz->fatorBalanco = EQ;
		raiz->direita->fatorBalanco = EQ;
	}
	else{
		/* Dupla rotação */
		raiz->esquerda = rodaEsquerda(raiz->esquerda);
		raiz = rodaDireita(raiz);

		switch(raiz->fatorBalanco){
			case EQ:
				raiz->esquerda->fatorBalanco = EQ;
				raiz->direita->fatorBalanco = EQ;
				break;
			case ESQ:
				t->direita->fatorBalanco = EQ;
				t->esquerda->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->direita->fatorBalanco = DIR;
				raiz->esquerda->fatorBalanco = EQ;
				break;
		}
		raiz->fatorBalanco = EQ;
	}
	return raiz;
}

static AVL_NODO* insereEsquerda(AVL_NODO *raiz, ValorNodo val, int (*compara) (void *, void *), int *cresceu){
	raiz->esquerda = insereNodo(raiz->esquerda, val, compara, cresceu);
	
	if(*cresceu){
		switch(raiz->fatorBalanco){
			case ESQ:
				raiz = equilibraEsquerda(raiz);
				*cresceu = 0;
				break;
			case EQ:
				raiz->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->fatorBalanco = EQ;
				*cresceu = 0;
				break;
		}
	}
	return raiz;
}

int altura(const AVL arv){
	return (arv == NULL) ? -1 : alturaAux(arv->raiz);
}

static int alturaAux(const AVL_NODO *raiz){
	int res;

	if(raiz == NULL)
		res = 0;
	else if(raiz->fatorBalanco == ESQ)
		res = 1 + alturaAux(raiz->esq);
	else
		res = 1 + alturaAux(raiz->dir);

	return res;
}