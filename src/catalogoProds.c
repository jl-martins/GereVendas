#include "catalogoProds.h"
#include "avl.h"
#include "memUtils.h"
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

/* Funções passadas para criaAVLgenerica() */
static int compara(const void *, const void *);
static void* duplica(void *);
static void atualiza(void *, void *);

/* Funções que manipulam catálogos de produtos */

CatProds criaCatProds()
{	
	int i;
	CatProds catP = malloc(sizeof(struct catProds));
	
	if(catP == NULL)
		return NULL;

	for(i = 0; i < MAX_AVL; ++i){
		catP->catalogo[i] = criaAVL(atualiza, compara, duplica, NULL);
		if(catP->catalogo[i] == NULL){ /* falha de alocação em criaAVL() */
			for( ; i >= 0; --i)
				catP->catalogo[i] = apagaAVL(catP->catalogo[i]);
			free(catP);
			return NULL;
		}
	}
	return catP;
}

CatProds insereProduto(CatProds catP, Produto p)
{	
	if(catP){
		AVL nova;
		int i = calculaPos(p);
		char* codProd = obterCodigoProduto(p);

		if(codProd == NULL)
			return NULL;

		nova = insereAVL(catP->catalogo[i], codProd);
		if(nova == NULL) /* falha de alocação a inserir na AVL */
			catP = NULL;
		else
			catP->catalogo[i] = nova;
		free(codProd);
	}
	return catP;
}

bool existeProduto(CatProds catP, Produto p)
{
	bool existe = FALSE;

	if(catP){ /* temos um catálogo */
		char* codProd = obterCodigoProduto(p);

		if(codProd != NULL && codProd[0] >= 'A' && codProd[0] <= 'Z'){
			int i = calculaPos(p);
			existe = existeAVL(catP->catalogo[i], codProd);
		}
		free(codProd);
	}
	return existe;
}

int totalProdutosLetra(CatProds catP, char l)
{
	int i = isupper(l) ? l - 'A' : -1; /* validação da letra */
	
	return (i == -1) ? 0 : tamanhoAVL(catP->catalogo[i]);
}

int totalProdutos(CatProds catP)
{
	int total = 0;

	if(catP){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			total += tamanhoAVL(catP->catalogo[i]);
	}
	return total;
}

CatProds apagaCatProds(CatProds catP)
{
 	if(catP){
 		int i;

 		for(i = 0; i < MAX_AVL; ++i)
 			catP->catalogo[i] = apagaAVL(catP->catalogo[i]);
 		free(catP);
 	}
	return NULL;
}

LStrings prodsPorLetra(CatProds catP, char l)
{	
	LStrings lProdsPorLetra = NULL;

	if(isupper(l)){ 
		int i = l - 'A';
		int total = tamanhoAVL(catP->catalogo[i]);
		char** codigosPorLetra = (char **) inorderAVL(catP->catalogo[i]);

		if(codigosPorLetra == NULL) /* falha de alocação na inorderAVL() */
			return NULL;

		lProdsPorLetra = criaLStrings(total, codigosPorLetra);
		apagaArray((void**) codigosPorLetra, total, free);

	}
	return lProdsPorLetra;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Produto */
static int compara(const void* cod1, const void* cod2)
{	
	return strcmp((char *) cod1, (char *) cod2);
}

static void* duplica(void* codProd)
{	
	char* original = codProd;
	int len = strlen(original);
	char* copia = malloc((len + 1) * sizeof(char));

	if(copia != NULL)
		strcpy(copia, original);
	return copia;
}


static void atualiza(void* prod1, void* prod2)
{
	prod1 = prod1; /* a prod1 mantem-se inalterada, esta instruçao apaga warnings do compilador */
	free(prod2); /* apaga o elemento duplicado */
	return;
}
