#include "catalogoProds.h"
#include "avl.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */

/* número de AVLs usadas no catálogo */
#define MAX_AVL 26
#define calculaPos(p) ((inicioCodigoProduto((p))) - 'A')

struct catProds {
	AVL catalogo[MAX_AVL];
};

struct conjProds {
	int total;
	Produto* prods;
	int pag;
	int i;
};

static int compara(const void *, const void *);
static void* duplica(const void *);

/* Funções que manipulam catálogos de produtos */

CatProds criaCatProds() {	
	int i;
	CatProds catP = malloc(sizeof(struct catProds));
	
	if(catP)
		for(i = 0; i < MAX_AVL; ++i)
			catP->catalogo[i] = criaAVL(compara, duplica);
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

void removeCatProds(CatProds catP) {
	int i;

	for(i = 0; i < MAX_AVL; ++i)
		apagaAVL(catP->catalogo[i]);
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
	}
	return conjuntoP;
}

void removeConjuntoProds(ConjuntoProds conjuntoP) {
	int i, total;

	total = conjuntoP->total;
	for(i = 0; i < total; ++i)
		removeProduto(conjuntoP->prods[i]);
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

int cardinalidade(ConjuntoProds conjuntoP) {
	return conjuntoP->total;
}

int obterPag(ConjuntoProds conjuntoP) {
	return conjuntoP->pag;
}

int obterIndice(ConjuntoProds conjuntoP) {
	return conjuntoP->i;
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
