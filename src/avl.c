#include "avl.h"
#include <stdlib.h>

#define MODO_TESTE 1

#if MODO_TESTE
	#include <stdio.h>
	#define TO_INT(ValorNodo) *((int *) (ValorNodo))
#endif

/* definir funçoes para travessias!! e toString
   fazer codigo mais seguro para quando malloc falha */
typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	struct nodoAVL * direita;
	struct nodoAVL * esquerda;
	ValorNodo valor;
	FatorBalanco fatorBalanco;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao de comparacao entre nodos - se o primeiro < segundo, o valor deve ser negativo */
	int (*compara)(const void*, const void*);
	/* ValorNodo criaValorNodo(void * val); */
	int tamanho;
} TCD_AVL;

/* lembrete: por a static as funcoes privadas */

/* Protótipos das funções privadas ao ficheiro */
static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu);
static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu);
static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu);
static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz);
static AVL_NODO* equilibraDireita(AVL_NODO* raiz);
static AVL_NODO* rodaEsquerda(AVL_NODO* raiz);
static AVL_NODO* rodaDireita(AVL_NODO* raiz);
static int alturaAux(const AVL_NODO* raiz);

/* DEBUG*/
/* TESTE_INICIO */
int compara(const void* x, const void* y){
	return TO_INT(x) - TO_INT(y);
}

void printArvore(AVL_NODO* raiz){
	static int tabs = 0;
	int i = 0;
	if(raiz == NULL){
/*		for (i = 0; i < tabs; i++) printf("\t"); */
		 printf("NULL\n");
		 tabs--;
	}else{
		tabs++;
		printArvore(raiz->esquerda);
		
/*		for (i = 0; i < tabs; i++) printf("\t"); */
		printf("%d\n", *((int*) raiz -> valor));
		tabs++;
		printArvore(raiz->direita);
	}	
}

/* Imprime uma AVL */
void imprimeAVL(AVL_NODO* raiz){
	AVL_NODO *esq, *dir;

	if(!raiz)
		return;

	esq = raiz->esquerda;
	dir = raiz->direita;

	if(!esq && !dir)
		printf("%d\n", TO_INT(raiz->valor));
	else{
		if(esq){
			printf("%d -> (E) %d\n", TO_INT(raiz->valor), TO_INT(esq->valor));
			imprimeAVL(esq);
		}

		if(dir){
			printf("%d -> (D) %d\n", TO_INT(raiz->valor), TO_INT(dir->valor));
			imprimeAVL(dir);
		}
	}
}

int main(){
	int d = 10, a = 2, b = 3, c = 1;
	AVL nova = criaAVL(compara);
	insere(nova, &b);
	insere(nova, &a);
	
	printArvore(nova->raiz);
	printf("\n");
	insere(nova, &c);
	printArvore(nova->raiz);
	insere(nova, &d);
	printf("\n");
	printArvore(nova->raiz);

	return 0;
}
/* TESTE_FIM */

/* ver o que fazer quando falha */
AVL criaAVL(int (*compar)(const void *, const void *)){
	AVL nova = (AVL) malloc(sizeof(TCD_AVL));
	
	if(nova == NULL)
		return NULL;
	nova -> raiz = NULL;
	nova -> compara = compar;
	nova -> tamanho = 0;
	return nova;
}

/* ver qual deve ser o tipo de retorno */
AVL insere(AVL arvore, ValorNodo val){
	int cresceu;
	
	if(arvore == NULL){
		/* Podemos usar IO nos modulos?? 
		   fprintf(stderr, "Necessário inicializar árvore"); */
		return NULL;
	}else{
		arvore -> tamanho++;		
		arvore -> raiz = insereNodo(arvore -> raiz, val, arvore -> compara, &cresceu);	
	}
	return arvore;
}

static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu){
	AVL_NODO * ret;
	
	if(raiz == NULL){
		ret = raiz = (AVL_NODO *) malloc(sizeof(AVL_NODO));
		raiz -> valor = val;	
		raiz -> esquerda = raiz -> direita = NULL;
		raiz -> fatorBalanco = EQ;
		*cresceu = 1;
	}
	else if(compara(val, raiz -> valor) < 0) /* raiz->valor > val */
		ret = insereEsquerda(raiz, val, compara, cresceu);
	else
		ret = insereDireita(raiz, val, compara, cresceu);	 
	return ret;
}

static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu){
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

static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, int (*compara) (const void *, const void *), int* cresceu){
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

static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz){

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
				raiz->direita->fatorBalanco = EQ;
				raiz->esquerda->fatorBalanco = ESQ;
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

static AVL_NODO* equilibraDireita(AVL_NODO* raiz){
	if (raiz -> direita -> fatorBalanco == DIR) {
		/* Rotacao simples a esquerda */
		raiz = rodaEsquerda(raiz);
		raiz -> fatorBalanco = EQ;
		raiz -> esquerda -> fatorBalanco = EQ;
	} else {
		/* Dupla rotacao */
		raiz -> direita = rodaDireita(raiz -> direita);
		raiz = rodaEsquerda(raiz);
		switch (raiz -> fatorBalanco) {
			case EQ:
				raiz -> esquerda -> fatorBalanco = EQ;
				raiz -> direita -> fatorBalanco = EQ;
				break;
			case ESQ:
				raiz -> esquerda -> fatorBalanco = EQ;
				raiz -> direita -> fatorBalanco = DIR;
				break;
			case DIR:
				raiz -> esquerda -> fatorBalanco = ESQ;
				raiz -> direita -> fatorBalanco = EQ;
		}
		raiz -> fatorBalanco = EQ;
	}
	return raiz;
}

static AVL_NODO* rodaEsquerda(AVL_NODO* raiz){
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

static AVL_NODO* rodaDireita(AVL_NODO* raiz){
	AVL_NODO* aux;
	
	if ((! raiz) || (! raiz -> esquerda)) {
		/* inserir código de tratamento de erros */
	} else {
		aux = raiz -> esquerda;
		raiz -> esquerda = aux -> direita;
		aux -> direita = raiz;
		raiz = aux;
	}
	return raiz;
}

/**
 * Verifica se um determinado valor existe numa AVL
 * @param arv Árvore AVL onde o valor será procurado
 * @param val Valor a procurar
 * @return 1 se o valor existir; 0 caso contrário.
 */
bool existeAVL(const AVL arv, ValorNodo val){
	int existe = FALSE;
	int r_compara; /* resultado de uma comparação */
	AVL_NODO *nodo_atual = arv->raiz;
	
	while(!existe && nodo_atual){
		r_compara = arv->compara(nodo_atual->valor, val);
		
		if(r_compara < 0) /* procura na subárvore esquerda */
			nodo_atual = nodo_atual->esquerda;
		else if(r_compara > 0) /* procura na subárvore direita */
			nodo_atual = nodo_atual->direita;
		else
			existe = TRUE;
	}

	return existe;
}

/**
 * @param arvore AVL cujo tamanho será retornado
 * @return Tamanho de 'arvore' se esta existir. -1 caso contrário.
 */
int tamanho(const AVL arvore){
	return (arvore == NULL) ? -1 : arvore->tamanho;
}

/**
 * @param arvore AVL cuja altura será calculada
 * @return A altura de 'arvore' se esta exisitir. -1 caso contrário.
 */ 
int altura(const AVL arvore){
	return (arvore == NULL) ? -1 : alturaAux(arvore->raiz);
}

static int alturaAux(const AVL_NODO *raiz){
	int res;

	if(raiz == NULL)
		res = 0;
	else if(raiz->fatorBalanco == ESQ)
		res = 1 + alturaAux(raiz->esquerda);
	else
		res = 1 + alturaAux(raiz->direita);

	return res;
}
