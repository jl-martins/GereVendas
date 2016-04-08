#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */
#include <ctype.h>
#include "avl.h"
#include "catalogoProds.h"

/* número de AVLs usadas no catálogo */
#define MAX_AVL 26 
#define calculaPos(c) ((inicioCodigoCliente((c))) -'A')

struct catProds {
	AVL catalogo[MAX_AVL];
};

struct conjProds {
	Produto * produtos;
	int tamanho;
};

static int compara(const void *, const void *);

CatProds criaCatProds()
{	
	int i;
	CatProds catP = (CatProds) malloc(sizeof(struct catProds));
	
	if(catP)
		for(i = 0; i < MAX_AVL; ++i)
			catP->catalogo[i] = criaAVL(compara);
	return catP;
}

CatProds insereProduto(CatProds catP, Produto p)
{
	int i = calculaPos(p);
	catP->catalogo[i] = insere(catP->catalogo[i], p);
	return catP;
}

bool existeProduto(CatProds catP, Produto p)
{
	int i = calculaPos(p);
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

static int compara(const void *str1, const void *str2)
{
	return strcmp((char *) str1, (char *) str2);
}
