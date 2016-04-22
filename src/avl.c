/* Módulo de árvores AVL genéricas */
#include "avl.h"
#include <stdlib.h>

/* Casos Possíveis de evolução das árvores: */
/* A árvore mudou de tamanho devido a uma inserção de um nodo novo e a altura aumentou */
#define INSERIU_CRESCEU 0
/* Um nodo novo foi inserido mas não aumentou o tamanho da árvore */
#define INSERIU 1 
/* Um nodo já existente foi atualizado pelo que o numero de nodos se manteve */
#define ATUALIZOU 2

typedef enum fatorBalanco {ESQ, EQ, DIR} FatorBalanco; 

typedef struct nodoAVL {
	void * valor;
	FatorBalanco fatorBalanco;
	struct nodoAVL *esquerda, *direita;
} AVL_NODO;

typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao usada para atualizar o valor de um nodo (o 1o argumento) usando um segundo elemento*/ 
	Atualizador atualiza;
	/* funcao de comparacao entre nodos - se o primeiro argumento for < que o segundo, o valor deve ser negativo */
	Comparador compara;
	/* funcao para criar um duplicado de um valor da avl */
	Duplicador duplica;
	/* funcao para libertar um nodo da AVL. Se esta função não for fornecida, é utilizado free() */
	LibertarNodo liberta;
	/* número de nodos da AVL */
	int tamanho;
} TCD_AVL;

/* Protótipos das funções privadas ao ficheiro */
static AVL_NODO* insereNodo(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modo);
static AVL_NODO* insereEsquerda(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modo);
static AVL_NODO* insereDireita(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int * modo);
static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz);
static AVL_NODO* equilibraDireita(AVL_NODO* raiz);
static AVL_NODO* rodaEsquerda(AVL_NODO* raiz);
static AVL_NODO* rodaDireita(AVL_NODO* raiz);
static int alturaAux(const AVL_NODO* raiz);
static int inorderAux(const AVL_NODO* arv, int i, void ** res, Duplicador duplica);
/* Remove os nodos de uma AVL ( função auxiliar de apagaAVL() ) */
static void apagaNodos(AVL_NODO* raiz, LibertarNodo liberta);

/* Devolve NULL quando a alocação falha */
AVL criaAVL(Atualizador atualiza, Comparador compara, Duplicador duplica, LibertarNodo liberta)
{
	AVL nova = NULL;
	/* só é criada uma AVL se tivermos uma função de comparação de nodos */
	if(compara){
		nova = malloc(sizeof(TCD_AVL));
		if(nova){
			nova->raiz = NULL;
			nova->atualiza = atualiza;
			nova->compara = compara;
			nova->duplica = duplica;
			nova->liberta = liberta;
			nova->tamanho = 0;
		}
	}
	return nova;
}

AVL insereAVL(AVL arvore, void * val)
{
	int modoInsercao;
	
	arvore->raiz = insereNodo(arvore->raiz, val, arvore->atualiza, arvore->compara, arvore->duplica, &modoInsercao);	
	if(modoInsercao != ATUALIZOU)
		arvore->tamanho++;
	return arvore;
}

/* Partir a assinatura desta função em várias linhas */
static AVL_NODO* insereNodo(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
{
	AVL_NODO* ret = NULL;
	int comparacao;
	
	if(raiz == NULL){
		ret = raiz = malloc(sizeof(AVL_NODO));
		if(raiz == NULL){
			*modoInsercao = ATUALIZOU;  /* indica que o tamanho nao se alterou */
			 return NULL; 
		}
		raiz->valor = (duplica != NULL) ? duplica(val) : val;
		raiz->esquerda = raiz->direita = NULL;
		raiz->fatorBalanco = EQ;
		*modoInsercao = INSERIU_CRESCEU;
	}
	else if((comparacao = compara(val, raiz->valor)) < 0) /* raiz->valor > val */
		ret = insereEsquerda(raiz, val, atualiza, compara, duplica, modoInsercao);
	else if(atualiza && comparacao == 0){
		*modoInsercao = ATUALIZOU;
		atualiza(raiz->valor, val);
		ret = raiz;
	}
	else 
		ret = insereDireita(raiz, val, atualiza, compara, duplica, modoInsercao);	 
	return ret;
}

static AVL_NODO* insereEsquerda(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
{
	raiz->esquerda = insereNodo(raiz->esquerda, val, atualiza, compara, duplica, modoInsercao);
	
	if(*modoInsercao == INSERIU_CRESCEU){
		switch(raiz->fatorBalanco){
			case ESQ:
				raiz = equilibraEsquerda(raiz);
				*modoInsercao = INSERIU;
				break;
			case EQ:
				raiz->fatorBalanco = ESQ;
				break;
			case DIR:
				raiz->fatorBalanco = EQ;
				*modoInsercao = INSERIU;
				break;
		}
	}
	return raiz;
}

static AVL_NODO* insereDireita(AVL_NODO* raiz, void * val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
{
	raiz->direita = insereNodo(raiz->direita, val, atualiza, compara, duplica, modoInsercao);
	
	if(*modoInsercao == INSERIU_CRESCEU){
		switch(raiz->fatorBalanco){
			case ESQ:
				raiz->fatorBalanco = EQ;
				*modoInsercao = INSERIU;
				break;
			case EQ:
				raiz->fatorBalanco = DIR;
				break;
			case DIR:
				raiz = equilibraDireita(raiz);
				*modoInsercao = INSERIU;
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
				raiz->direita->fatorBalanco = DIR;
				raiz->esquerda->fatorBalanco = EQ;
				break;
			case DIR:
				raiz->direita->fatorBalanco = EQ;
				raiz->esquerda->fatorBalanco = ESQ;
				break;
		}
		raiz->fatorBalanco = EQ;
	}
	return raiz;
}

static AVL_NODO* equilibraDireita(AVL_NODO* raiz)
{
	if (raiz->direita->fatorBalanco == DIR) {
		/* Rotacao simples a esquerda */
		raiz = rodaEsquerda(raiz);
		raiz->fatorBalanco = EQ;
		raiz->esquerda->fatorBalanco = EQ;
	} else {
		/* Dupla rotacao */
		raiz->direita = rodaDireita(raiz->direita);
		raiz = rodaEsquerda(raiz);
		switch (raiz->fatorBalanco) {
			case EQ:
				raiz->esquerda->fatorBalanco = EQ;
				raiz->direita->fatorBalanco = EQ;
				break;
			case ESQ:
				raiz->esquerda->fatorBalanco = EQ;
				raiz->direita->fatorBalanco = DIR;
				break;
			case DIR:
				raiz->esquerda->fatorBalanco = ESQ;
				raiz->direita->fatorBalanco = EQ;
		}
		raiz->fatorBalanco = EQ;
	}
	return raiz;
}

static AVL_NODO* rodaEsquerda(AVL_NODO* raiz)
{
	AVL_NODO *aux;

	aux = raiz->direita;
	raiz->direita = aux->esquerda;
	aux->esquerda = raiz;
	raiz = aux;
	return raiz;
}

static AVL_NODO* rodaDireita(AVL_NODO* raiz)
{
	AVL_NODO* aux;
	
	aux = raiz->esquerda;
	raiz->esquerda = aux->direita;
	aux->direita = raiz;
	raiz = aux;
	return raiz;
}

void * procuraAVL(const AVL arv, void* val)
{
	int rCompara; /* guarda o resultado de cada comparação */
	void* res = NULL;
	AVL_NODO* nodoAtual = arv->raiz;
	
	while(nodoAtual){
		rCompara = arv->compara(nodoAtual->valor, val);
		
		if(rCompara < 0) /* procura na subárvore esquerda */
			nodoAtual = nodoAtual->direita;
		else if(rCompara > 0) /* procura na subárvore direita */
			nodoAtual = nodoAtual->esquerda;
		else{
			res = (arv->duplica != NULL) ? arv->duplica(nodoAtual->valor) : nodoAtual->valor;
			break; /* encontramos o valor procurado */
		}
	}
	return res;
}

bool existeAVL(const AVL arv, void* val)
{	
	void* res = procuraAVL(arv, val);
	bool existe;

	if(res != NULL){
		existe = TRUE;
		if(arv->duplica){
			if(arv->liberta)
				arv->liberta(res);
			else
				free(res);
		}
	}
	else
		existe = FALSE;
	return existe;
}

void ** inorderAVL(const AVL arv)
{
	void ** res = NULL;

	if(arv && arv->raiz){
		res = malloc(arv->tamanho * sizeof(void *));
		
		if(res != NULL)
			inorderAux(arv->raiz, 0, res, arv->duplica);
	}
	return res;
}

static int inorderAux(const AVL_NODO* raiz, int i, void ** res, Duplicador duplica)
{	
	if(raiz){
		i = inorderAux(raiz->esquerda, i, res, duplica);
		res[i] = (duplica != NULL) ? duplica(raiz->valor) : raiz->valor;
		i = inorderAux(raiz->direita, i+1, res, duplica);
	}
	return i;
}

/**
 * @param arvore AVL cujo tamanho será retornado
 * @return Tamanho de 'arvore' se esta existir. -1 caso contrário.
 */
int tamanhoAVL(const AVL arvore)
{
	return (arvore == NULL) ? -1 : arvore->tamanho;
}

/**
 * @param arvore AVL cuja altura será calculada
 * @return A altura de 'arvore' se esta exisitir. -1 caso contrário.
 */ 
int alturaAVL(const AVL arvore)
{
	return (arvore == NULL) ? -1 : alturaAux(arvore->raiz);
}

static int alturaAux(const AVL_NODO* raiz)
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

AVL apagaAVL(AVL arvore)
{	
	if(arvore){
		apagaNodos(arvore->raiz, arvore->liberta);
		free(arvore);
	}
	return NULL;
}

/* Apaga os nodos de uma AVL. Se o apontador para a função
 * de libertação for diferente de NULL, esta é utilizada para
 * libertar cada nodo. Se não, é utilizada a função free(). */
static void apagaNodos(AVL_NODO* raiz, LibertarNodo liberta)
{
	if(raiz != NULL){
		AVL_NODO* esq = raiz->esquerda;
		AVL_NODO* dir = raiz->direita;

		if(liberta != NULL)
			liberta(raiz->valor);
		else
			free(raiz->valor);
		raiz->valor = NULL;
		free(raiz);
		apagaNodos(esq, liberta);
		apagaNodos(dir, liberta);
	}
}
