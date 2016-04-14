#include "catalogoProds.h"
#include "avl.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */

/* número de AVLs usadas no catálogo */
#define MAX_AVL 26
#define calculaPos(p) ((inicioCodigoProduto((p))) - 'A')
#define CODIGOS_POR_PAG 20

struct catProds {
	AVL catalogo[MAX_AVL];
};

/*Para isto ficar mais organizado, podiamos ter um apontador para 
  uma estrutura tipo infoPaginacao ou infoNavegacao, que tinha 
  todas as variáveis relativas á navegacao do conjunto*/
struct conjProds {
	int total;
	Produto* prods;
	int pag;
	int maxpag; /*Só inicializada quando houver codPorPag*/
	int i;
	int f;	/*Só inicializado quando houver codPorPag*/
	int codPorPag;
	int mod; /*idem*/ /*Não sei bem o que chamar a isto, 
						mas uso muitas vezes por isso mais 
						vale ter um campo para ele do que 
						tar sempre a calcular -> (total % codPorPag)*/
};

static int compara(const void *, const void *);
static void* duplica(void *);
static void liberta(void *);
static void atualiza(void *, void *);
/* Funções que manipulam catálogos de produtos */

CatProds criaCatProds() {	
	int i;
	CatProds catP = malloc(sizeof(struct catProds));
	
	if(catP)
		for(i = 0; i < MAX_AVL; ++i)
			catP->catalogo[i] = criaAVLgenerica(atualiza, compara, duplica, liberta);
	return catP;
}

CatProds insereProduto(CatProds catP, Produto p) {
	int i = calculaPos(p);
	if(catP)	
		catP->catalogo[i] = insere(catP->catalogo[i], p);
	return catP;
}

bool existeProduto(CatProds catP, Produto p) {
	bool existe = FALSE;
	int i;

	if(catP){ /* temos um catálogo */
		i = calculaPos(p);
		existe = existeAVL(catP->catalogo[i], p);
	}

	return existe;
}

/* onde é usada??*/
int totalProdutosLetra(CatProds catP, char l) {
	int i = isupper(l) ? l - 'A' : -1; /* validação da letra */
	
	return (i == -1) ? 0 : tamanho(catP->catalogo[i]);
}

int totalProdutos(CatProds catP) {
	int i, total = 0;

	for(i = 0; i < MAX_AVL; ++i)
		total += tamanho(catP->catalogo[i]);

	return total;
}

void apagaCatProds(CatProds catP) {
	int i;

	for(i = 0; i < MAX_AVL; ++i)
		/*apagaAVL(catP->catalogo[i]);*/
	free(catP);
}

/* Funções que manipulam conjuntos de produtos */

ConjuntoProds criaConjuntoProds(int total, Produto* prods) {
	ConjuntoProds conjuntoP = malloc(sizeof(struct conjProds));

	if(conjuntoP){
		conjuntoP->total = total;
		conjuntoP->prods = prods;
		conjuntoP->pag = 1;
		conjuntoP->i = 0;
		conjuntoP = setCodsPorPag(conjuntoP, CODIGOS_POR_PAG);
	}
	return conjuntoP;
}

ConjuntoProds setCodsPorPag(ConjuntoProds conjP, int nmr)
{
	if(conjP)
	{
		conjP->codPorPag = nmr;
		conjP->mod = conjP->total % nmr;
		conjP->maxpag = conjP->mod 
						? (int) (conjP->total)/nmr + 1
						: conjP->total/nmr;

		/* TODO: rever isto */
		if(conjP->total <= nmr)
			conjP->f = conjP->total;
		else
			conjP->f = nmr;
	}
	return conjP;
}

void apagaConjuntoProds(ConjuntoProds conjuntoP) {
	int i, total;

	total = conjuntoP->total;
	for(i = 0; i < total; ++i)
		apagaProduto(conjuntoP->prods[i]);
}

/* Em casos de sucesso, a função 'obterCodigosP' devolve um array de strings 
 * ordenado crescentemente, com os códigos de produtos de um conjunto de produtos. 
 * Se ocorrer um erro, é devolvido NULL */
char** obterCodigosP(ConjuntoProds conjuntoP) {
	int total = conjuntoP->total;
	char** codigos = malloc(total * sizeof(char *));
	
	if(codigos != NULL){
		int i = 0;
		char* codigoP;
		
		for(i = 0; i < total; ++i){
			codigoP = obterCodigoProduto(conjuntoP->prods[i]);
			if(codigoP == NULL)
				break; /* se houve uma falha de alocação, saimos do ciclo */
			codigos[i] = codigoP;
		}
		if(i < total){ /* tratamento de falhas de alocação */
			for(i = i-1; i >= 0; --i)
				free(codigos[i]);
			codigos = NULL;
		}
	}
	return codigos;
}

/*mesma função que em cima mas extrai os códigos da página*/
/*Solução: passar na primeira uma flag pag, se for 1 extraimos a pagina senão extraimos todo o conjunto*/
char** obterCodigosPPag(ConjuntoProds conjP) {
	/*
	  Se estivermos na ultima página e tam/codPorPag não for total
	  então a ultima pagina não vai ter o mesmo numero de códigos
	  que as outras páginas, vai ter tam%codPorPag, que está em conjP->mod
	  int total = conjP->mod && conjP->pag == conjP->maxpag
	  			? conjP->mod
				: conjP->codPorPag;
	*/
	int total = (conjP->f - conjP->i) + 1;

	char** codigos = malloc(total * sizeof(char *));
	
	if(codigos != NULL){
		int i;
		char* codigoP;
		
		for(i = conjP->i; i < conjP->f; ++i){
			codigoP = obterCodigoProduto(conjP->prods[i]);
			if(codigoP == NULL)
				break; /* se houve uma falha de alocação, saimos do ciclo */
			codigos[conjP->f - (conjP->f - i)] = codigoP;
		}
		if(i < conjP->f){ /* tratamento de falhas de alocação */
			for(i = i-1; i >= conjP->i; --i)
				free(codigos[i]);
			codigos = NULL;
		}
	}
	return codigos;
}

int cardinalidade(ConjuntoProds conjuntoP) {
	return conjuntoP->total;
}

int obterPag(ConjuntoProds conjuntoP) {
	return conjuntoP->pag;
}

int obterMaxPag(ConjuntoProds conjuntoP) {
	return conjuntoP->maxpag;
}

int obterIndice(ConjuntoProds conjuntoP) {
	return conjuntoP->i;
}

int obterIndiceFinal(ConjuntoProds conjuntoP) {
	return conjuntoP->f;
}

void nextPage(ConjuntoProds conj)
{
	if(conj->pag < conj->maxpag)
	{
		conj->i = conj->f;
		conj->f = (conj->i + conj->codPorPag) > conj->total
					? conj->total
					: conj->i + conj->codPorPag;
		conj->pag++;
	}
	else
		conj->i -= conj->mod ? conj->mod : conj->codPorPag;
}

void prevPage(ConjuntoProds conj)
{
	if(conj->pag > 1)
	{
		if(conj->mod && conj->pag == conj->maxpag)
		{
			conj->i -= conj->mod + conj->codPorPag;
			conj->f -= conj->mod;
		}
		else
		{
			conj->i -= 2 * conj->codPorPag;
			conj->f -= conj->codPorPag;
		}
		
		conj->pag--;
	}
	else
		conj->i = 0;
}

void lastPage(ConjuntoProds conj)
{
	conj->i = conj->f = conj->total;
	conj->i -= (conj->mod) ? conj->mod : conj->codPorPag;
	conj->pag = conj->maxpag;
}

void fstPage(ConjuntoProds conj)
{
	conj->i = 0;
	/*	TODO: again rever isto */
	if(conj->mod && conj->total <= conj->codPorPag)
			conj->f = conj->total;
		else
			conj->f = conj->codPorPag;

	conj->pag = 1;
}

/*retorna 0 se a página existe, 1 se não existe*/
int goToPage(ConjuntoProds conj, int p)
{
	int err = 0;
	if(p < 1 || p > conj->maxpag) /*se não existir vai para a pagina 1*/
	{
		conj->i = 0;
		conj->f = conj->mod && conj->total <= conj->codPorPag
				? conj->total
				: conj->codPorPag;

		conj->pag = 1;
		err = 1;	
	}
	else
	{
		conj->i = 0; conj->f = conj->i + conj->codPorPag;
		conj->i = conj->f * p - conj->codPorPag;
		conj->f = conj->mod && conj->pag == conj->maxpag 
				? conj->i + conj->mod
				: conj->f * p;
		conj->pag = p;
	}
	return err;
}

ConjuntoProds prodsPorLetra(CatProds catP, char l) {	
	ConjuntoProds conjuntoP = NULL;

	if(isupper(l)){ 
		int i = l - 'A'; /* índice da avl a consultar */
		int total = tamanho(catP->catalogo[i]);
		Produto* prods = (Produto*) inorder(catP->catalogo[i]);
		
		conjuntoP = criaConjuntoProds(total, prods); 
	}
	return conjuntoP;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Produto */
static int compara(const void* prod1, const void* prod2) {	
	char* codigo1 = obterCodigoProduto((Produto) prod1);
	char* codigo2 = obterCodigoProduto((Produto) prod2);
	int r = strcmp(codigo1, codigo2);

	free(codigo1);
	free(codigo2);
	
	return r;
}

static void* duplica(void* prod)
{
	return duplicaProduto((Produto) prod);
}

static void liberta(void* prod)
{
	apagaProduto((Produto) prod);
}

static void atualiza(void* prod1, void* prod2)
{
	return;
}
