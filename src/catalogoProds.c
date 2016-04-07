#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */
#include <ctype.h>
#include "avl.h"
#include "catalogoProds.h"

/* número de AVLs usadas no catálogo */
#define MAX_AVL 26 

struct catProds {
	AVL catalogo[MAX_AVL];
};

struct conjProds {
	char** codigos;
};

static int calculaIndiceAVL(Produto);
static int compara(const void *, const void *);

CatProds criaCatProds()
{	
	CatProds catP = (CatProds) malloc(sizeof(struct catProds));
	
	if(catP){ /* assumimos que compara != NULL */
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			catP->catalogo[i] = criaAVL(compara);
	}
	
	return catP;
}

CatProds insereProduto(CatProds catP, Produto p)
{
	int i = calculaIndiceAVL(p);
	
	catP->catalogo[i] = insere(catP->catalogo[i], p);
	return catP;
}

bool existeProduto(CatProds catP, Produto p)
{
	int i = calculaIndiceAVL(p);

	return existeAVL(catP->catalogo[i], p);
}

int totalProdutosLetra(CatProds catP, char l)
{
	int i = isupper(l) ? l - 'A' : -1;

	return (i == -1) ? 0 : tamanho(catP->catalogo[i]);
}

int totalProdutos(CatProds catP)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; ++i)
		total += tamanho(catP->catalogo[i]);

	return total;
}

void removeCatProds(CatProds catP)
{
	int i;
	
	for(i = 0; i < MAX_AVL; ++i)
		apagaAVL(catP->catalogo[i]);

	free(catP); 
}

/* o nome dos produtos do catálgo tem que começar por maiúscula*/
/*
ConjuntoProdutos prodsPorLetra(CatProds catP, char l)
{	
	ConjuntoProdutos conjuntoP = NULL;

	if(isupper(l)){ 
		conjuntoP = (ConjuntoProds) malloc(sizeof(struct conjProds));

		if(conjuntoP){
			int i = l - 'A';
			conjuntoP->codigos = (char **) inorder(catP->catalogo[i]);
		}
	}

	return conjunto;
}*/

static int calculaIndiceAVL(Produto p)
{	
	char* codigoProduto = obterCodigoProduto(p);
	int i = codigoProduto[0] - 'A';

	free(codigoProduto); /* já não precisamos do código do produto */
	return i;
}

static int compara(const void *str1, const void *str2)
{
	return strcmp((char *) str1, (char *) str2);
}