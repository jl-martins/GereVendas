#include <ctype.h>
#include <stdlib.h>
#include "avl.h"
#include "catalogo.h"

/* número de AVLs usadas no catálogo */
#define MAX_AVL 26 

struct cat {
	AVL catalogo[MAX_AVL];
};

struct elem {
	char* nome;
};

struct conjElems {
	char** nomes;
};

static int calculaIndiceAVL(Elem e);

Catalogo initCatProds(int (*compara) (const void *, const void *))
{	
	Catalogo c = (Catalogo) malloc(sizeof(struct cat));
	
	if(c){ /* assumimos que compara != NULL */
		int i;

		for(i = 0; i < MAX_AVL; i++)
			c->catalogo[i] = criaAVL(compara);
	}
	
	return c;
}

Catalogo insereProduto(Catalogo c, Elem e)
{
	int i = calculaIndiceAVL(e);
	
	c->catalogo[i] = insere(c->catalogo[i], e);
	return c;
}

bool existeProduto(Catalogo c, Elem e)
{
	int i = calculaIndiceAVL(e);

	return existeAVL(c->catalogo[i], e);
}

int totalProdutosLetra(Catalogo c, char l)
{
	int i = isupper(l) ? l - 'A' : -1;

	return (i == -1) ? 0 : tamanho(c->catalogo[i]);
}

int totalProdutos(Catalogo c)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; i++)
		total += tamanho(c->catalogo[i]);

	return total;
}

void removeCatProds(Catalogo c)
{
	int i;
	
	for(i = 0; i < MAX_AVL; i++)
		apagaAVL(c->catalogo[i]);

	free(c); 
}

/* o nome dos elementos do catálgo tem que começar por maiúscula*/
/*
ConjuntoElems elemsPorLetra(Catalogo c, char l)
{	
	ConjuntoElems conjunto = NULL;

	if(isupper(l)){ 
		conjunto = (ConjuntoElems) malloc(sizeof(struct conjElems));

		if(conjunto){
			int i = l - 'A';
			conjunto->nomes = (char **) inorder(c->catalogo[i]);
		}
	}

	return conjunto;
}*/

static int calculaIndiceAVL(Elem e)
{	
	return e->nome[0] - 'A';
}
