/* Garantir que os tipos são opacos para preservar encapsulamento, eliminar funçoes de duplicaçao */


#include "avl.h"
#include <stdlib.h>

/* Tamanho inicial do array produzido pela função filtraAVL() */
#define TAM_INI_FILTRA 40

/* definir funçoes para travessias!! e toString
   fazer codigo mais seguro para quando malloc falha */
typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	ValorNodo valor;
	FatorBalanco fatorBalanco;
	struct nodoAVL *esquerda, *direita;
} AVL_NODO;
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao de comparacao entre nodos - se o primeiro < segundo, o valor deve ser negativo */
	Comparador compara;
	/* funcao usada para atualizar o valor de um nodo(o 1o argumento) usando um segundo elemento*/ 
	Atualizador atualiza;
	/* ValorNodo criaValorNodo(void * val); */
	Duplicador duplicaElem; /* funcão usada para fazer clone do valor de um nodo */
	int tamanho;
} TCD_AVL;


/* Protótipos das funções privadas ao ficheiro */
static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu);
static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu);
static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu);
static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz);
static AVL_NODO* equilibraDireita(AVL_NODO* raiz);
static AVL_NODO* rodaEsquerda(AVL_NODO* raiz);
static AVL_NODO* rodaDireita(AVL_NODO* raiz);
static ValorNodo* inorderAux(AVL_NODO* raiz, Duplicador dup, ValorNodo* res);
static int alturaAux(const AVL_NODO* raiz);

/* ver o que fazer quando falha */
AVL criaAVLgenerica(Comparador compara, Atualizador atualiza, Duplicador duplicaElem)
{
	AVL nova = NULL;
	
	/* só é criada uma AVL se tivermos uma função de comparação */
	if(compara && duplicaElem){
		nova = malloc(sizeof(TCD_AVL));

		if(nova){
			nova -> raiz = NULL;
			nova -> compara = compara;
			nova -> atualiza = atualiza;
			nova -> duplicaElem = duplicaElem;
			nova -> tamanho = 0;
		}
	}
	return nova;
}

/* ver qual deve ser o tipo de retorno */
AVL insere(AVL arvore, ValorNodo val)
{
	int cresceu;
	
	if(arvore == NULL){
		/*codigo de erros*/
	}else{
		arvore -> tamanho++;		
		/* ver o que fazer se nao der para inserir */
		arvore -> raiz = insereNodo(arvore -> raiz, val, arvore -> compara, arvore -> atualiza, &cresceu);	
	}
	return arvore;
}

static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu)
{
	AVL_NODO* ret = NULL;
	int comparacao;
	
	if(raiz == NULL){
		ret = raiz = malloc(sizeof(AVL_NODO));
		raiz -> valor = val;	
		raiz -> esquerda = raiz -> direita = NULL;
		raiz -> fatorBalanco = EQ;
		*cresceu = 1;
	}
	else if((comparacao = compara(val, raiz -> valor)) < 0) /* raiz->valor > val */
		ret = insereEsquerda(raiz, val, compara, atualiza, cresceu);
	else if(atualiza != NULL && comparacao == 0)
		atualiza(raiz, val);
	else 
		ret = insereDireita(raiz, val, compara, atualiza, cresceu);	 
	return ret;
}

static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu)
{
	raiz->esquerda = insereNodo(raiz->esquerda, val, compara, atualiza, cresceu);
	
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

static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, Comparador compara, Atualizador atualiza, int* cresceu)
{
	raiz -> direita = insereNodo(raiz -> direita, val, compara, atualiza, cresceu);
	
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

static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz)
{
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

static AVL_NODO* equilibraDireita(AVL_NODO* raiz)
{
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

static AVL_NODO* rodaEsquerda(AVL_NODO* raiz)
{
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

static AVL_NODO* rodaDireita(AVL_NODO* raiz)
{
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
ValorNodo procuraAVL(const AVL arv, ValorNodo val)
{
	int r_compara; /* guarda o resultado de uma comparação */
	ValorNodo res = NULL;
	AVL_NODO *nodo_atual = arv->raiz;
	
	while(nodo_atual){
		r_compara = arv->compara(nodo_atual->valor, val);
		
		if(r_compara < 0) /* procura na subárvore esquerda */
			nodo_atual = nodo_atual->esquerda;
		else if(r_compara > 0) /* procura na subárvore direita */
			nodo_atual = nodo_atual->direita;
		else{
			/* ver código de tratamento de erros */
			res = arv->duplicaElem(nodo_atual->valor);
			break; /* encontramos o valor */
		}
	}
	return res;
}

bool existeAVL(const AVL arv, ValorNodo val)
{
	bool existe = procuraAVL(arv, val) != NULL;

	return existe;
}

/* Faz uma travessia inorder da AVL 'arv' e devolve um array com os 
   valores dos nodos que satisfazem o predicado passado como argumento.
   NOTA: filtraAVL(arv, NULL) produz o mesmo resultado que inorder(arv)
*/

/*
ValorNodo* filtraAVL(const AVL arv, Predicado p)
{
	ValorNodo* res = NULL;

	if(arv && arv->raiz){
		res = malloc(TAM_INI_FILTRA * sizeof(ValorNodo *));

		if(res != NULL)
			res = filtraAVLaux(arv->raiz, p, 0, TAM_INI(arv->tamanho));
	}

	return res;
}

ValorNodo* filtraAVLaux(AVL_NODO* raiz, Predicado p, int tamanho, int max)
{

}
*/

ValorNodo* inorder(const AVL arv)
{
	ValorNodo* res = NULL;

	if(arv && arv->raiz){
		res = malloc(arv->tamanho * sizeof(ValorNodo));
		
		if(res != NULL)
			res = inorderAux(arv->raiz, arv->duplicaElem, res);
	}

	return res;
}

static ValorNodo* inorderAux(AVL_NODO* raiz, Duplicador dup, ValorNodo* res)
{	
	static int i = 0;

	if(raiz){
		res = inorderAux(raiz->esquerda, dup, res);
		res[i++] = dup(raiz->valor);
		res = inorderAux(raiz->direita, dup, res);
	}

	return res;
}

/**
 * @param arvore AVL cujo tamanho será retornado
 * @return Tamanho de 'arvore' se esta existir. -1 caso contrário.
 */
int tamanho(const AVL arvore)
{
	return (arvore == NULL) ? -1 : arvore->tamanho;
}

/**
 * @param arvore AVL cuja altura será calculada
 * @return A altura de 'arvore' se esta exisitir. -1 caso contrário.
 */ 
int altura(const AVL arvore)
{
	return (arvore == NULL) ? -1 : alturaAux(arvore->raiz);
}

static int alturaAux(const AVL_NODO *raiz)
{
	int res;

	if(raiz == NULL)
		res = 0;
	else if(raiz->fatorBalanco == ESQ)
		res = 1 + alturaAux(raiz->esquerda);
	else
		res = 1 + alturaAux(raiz->direita);

	return res;
}
