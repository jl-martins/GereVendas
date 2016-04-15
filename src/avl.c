/* Necessario fazer free de tudo para limpar completamente da memoria, estruturas compeltamente encapsuladas*/
/* Garantir que os tipos são opacos para preservar encapsulamento */
/* Nao libertar memoria dos campos das avls e dos tipos devolvidos a nao ser por funções da api */
#include "avl.h"
#include <assert.h>
#include <stdlib.h>

/* Casos Possíveis de evolução das árvores */
/* A árvore mudou de tamanho devido a uma inserção de um nodo novo e a altura aumentou */
#define INSERIU_CRESCEU 0
/* Um nodo novo foi inserido mas não aumentou o tamanho da árvore */
#define INSERIU 1 
/* Um nodo já existente foi atualizado pelo que o numero de nodos se manteve */
#define ATUALIZOU 2

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

/* SUGESTAO: ter campo para a funçao de libertaçao que deve ser inicializado à parte, por defeito é a free*/
	
typedef struct TCD_AVL {
	AVL_NODO* raiz;
	/* funcao usada para atualizar o valor de um nodo (o 1o argumento) usando um segundo elemento*/ 
	Atualizador atualiza;
	/* funcao de comparacao entre nodos - se o primeiro < segundo, o valor deve ser negativo */
	Comparador compara;
	/* funcao para criar um duplicado do elemento a inserir */
	Duplicador duplica;
	/* funcao para libertar um nodo da AVL. Se esta função não for fornecida, é utilizado free() */
	LibertarNodo liberta;
	/* número de nodos da AVL */
	int tamanho;
} TCD_AVL;

/* Protótipos das funções privadas ao ficheiro */

/* !! partir assinaturas grandes em várias linhas */
static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modo);
static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modo);
static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int * modo);
static AVL_NODO* equilibraEsquerda(AVL_NODO* raiz);
static AVL_NODO* equilibraDireita(AVL_NODO* raiz);
static AVL_NODO* rodaEsquerda(AVL_NODO* raiz);
static AVL_NODO* rodaDireita(AVL_NODO* raiz);
static int alturaAux(const AVL_NODO* raiz);
static void inorderAux(const AVL_NODO* arv, ValorNodo* res, Duplicador duplica);
/* Remove os nodos de uma AVL ( função auxiliar de apagaAVL() ) */
static void apagaNodos(AVL_NODO* raiz, LibertarNodo liberta);

/* Devolve NULL quando a alocação falha */
AVL criaAVLgenerica(Atualizador atualiza, Comparador compara, Duplicador duplica, LibertarNodo liberta)
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

AVL insere(AVL arvore, ValorNodo val)
{
	int modoInsercao;
	
	assert(arvore != NULL); /* pré-condição para inserir na AVL */
	assert(val != NULL); /* pré-condição para inserir na AVL */

	arvore->raiz = insereNodo(arvore->raiz, val, arvore->atualiza, arvore->compara, arvore->duplica, &modoInsercao);	
	if(modoInsercao != ATUALIZOU)
		arvore->tamanho++;
	return arvore;
}

/* Partir a assinatura desta função em várias linhas */
static AVL_NODO* insereNodo(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
{
	AVL_NODO* ret = NULL;
	int comparacao;
	
	if(raiz == NULL){
		ret = raiz = malloc(sizeof(AVL_NODO)); /* não estamos a verificar o retorno deste malloc() */
		raiz->valor = (duplica != NULL) ? duplica(val) : val;
		raiz->esquerda = raiz->direita = NULL;
		raiz->fatorBalanco = EQ;
		*modoInsercao = INSERIU_CRESCEU;
	}
	else if((comparacao = compara(val, raiz->valor)) < 0) /* raiz->valor > val */
		ret = insereEsquerda(raiz, val, atualiza, compara, duplica, modoInsercao);
	else if(atualiza != NULL && comparacao == 0){
		*modoInsercao = ATUALIZOU;
		atualiza(raiz, val);
		ret = raiz;
	}
	else 
		ret = insereDireita(raiz, val, atualiza, compara, duplica, modoInsercao);	 
	return ret;
}

static AVL_NODO* insereEsquerda(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
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

static AVL_NODO* insereDireita(AVL_NODO* raiz, ValorNodo val, Atualizador atualiza, Comparador compara, Duplicador duplica, int* modoInsercao)
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

	/*assert(raiz != NULL && raiz->direita != NULL);*/

	aux = raiz->direita;
	raiz->direita = aux->esquerda;
	aux->esquerda = raiz;
	raiz = aux;
	return raiz;
}

static AVL_NODO* rodaDireita(AVL_NODO* raiz)
{
	AVL_NODO* aux;
	
	/* assert(raiz != NULL && raiz->esquerda != NULL);*/

	aux = raiz->esquerda;
	raiz->esquerda = aux->direita;
	aux->direita = raiz;
	raiz = aux;
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
	int rCompara; /* guarda o resultado de uma comparação */
	ValorNodo res = NULL;
	AVL_NODO* nodoAtual = arv->raiz;
	
	while(nodoAtual){
		rCompara = arv->compara(nodoAtual->valor, val);
		
		if(rCompara < 0) /* procura na subárvore esquerda */
			nodoAtual = nodoAtual->direita;
		else if(rCompara > 0) /* procura na subárvore direita */
			nodoAtual = nodoAtual->esquerda;
		else{
			/* ver código de tratamento de erros */
			res = (arv->duplica != NULL) ? arv->duplica(val) : val;
			break; /* encontramos o valor procurado */
		}
	}
	return res;
}

bool existeAVL(const AVL arv, ValorNodo val)
{	
	ValorNodo res = procuraAVL(arv, val);
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
		
		if(res != NULL) /* ver se inorderAux não é "programação com balde" */
			inorderAux(arv->raiz, res, arv->duplica);
	}
	return res;
}

static void inorderAux(const AVL_NODO* raiz, ValorNodo* res, Duplicador duplica)
{	
	static int i = 0;

	if(raiz){
		inorderAux(raiz->esquerda, res, duplica);
		res[i++] = (duplica != NULL) ? duplica(raiz->valor) : raiz->valor;
		inorderAux(raiz->direita, res, duplica);
	}
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

void apagaAVL(AVL arvore)
{	
	if(arvore != NULL){
		apagaNodos(arvore->raiz, arvore->liberta);
		free(arvore);
	}
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
			liberta(raiz);
		else
			free(raiz);

		apagaNodos(esq, liberta);
		apagaNodos(dir, liberta);
	}
}
