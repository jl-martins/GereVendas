#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */
#include <ctype.h>
#include "avl.h"
#include "catalogoClientes.h"

/* número de AVLs usadas no catálogo */
#define calculaPos(c) ((inicioCodigoCliente((c))) -'A')
#define MAX_AVL 26 

struct catClientes {
	AVL catalogo[MAX_AVL];
};

struct conjClientes {
	Cliente * clientes;
	int tamanho;
};

static int compara(const void *, const void *);

CatClientes criaCatClientes()
{	int i;
	CatClientes catC = (CatClientes) malloc(sizeof(struct catClientes));
	
	if(catC) 
		for(i = 0; i < MAX_AVL; ++i)
			catC->catalogo[i] = criaAVL(compara);
	return catC;
}

CatClientes insereCliente(CatClientes catC, Cliente c)
{
	int i = calculaPos(c);
	catC->catalogo[i] = insere(catC->catalogo[i], c);
	return catC;
}

bool existeCliente(CatClientes catC, Cliente c)
{
	int i = calculaPos(c); 
	return existeAVL(catC->catalogo[i], c);
}

int totalClientesLetra(CatClientes catC, char l)
{
	int i = isalpha(l) ? toupper(l) - 'A' : -1;
	return (i == -1) ? 0 : tamanho(catC->catalogo[i]);
}

int totalClientes(CatClientes catC)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; ++i)
		total += tamanho(catC->catalogo[i]);

	return total;
}

void removeCatClientes(CatClientes catC)
{
	int i;
	for(i = 0; i < MAX_AVL; ++i)
		apagaAVL(catC->catalogo[i]);
	free(catC); 
}

/* o nome dos Clientes do catálgo tem que começar por maiúscula*/
/*
ConjuntoClientes clientesPorLetra(CatClientes catC, char l)
{	
	ConjuntoClientes conjuntoC = NULL;

	if(isupper(l)){ 
		conjuntoC = (ConjuntoClientes) malloc(sizeof(struct conjClientes));

		if(conjuntoC){
			int i = l - 'A';
			conjuntoC->nomes = (char **) inorder(catC->catalogo[i]);
		}
	}

	return conjunto;
}*/

static int compara(const void *str1, const void *str2)
{
	return strcmp((char *) str1, (char *) str2);
}
