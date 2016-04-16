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
static ValorNodo duplica(void *);
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
	if(catP){
		AVL nova;
		int i = calculaPos(p);

		nova = insere(catP->catalogo[i], p);
		if(nova == NULL) /* falha de alocação a inserir na AVL */
			return NULL;
		catP->catalogo[i] = nova;
	}
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
	int total = 0;

	if(catP){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			total += tamanho(catP->catalogo[i]);
	}
	return total;
}

void apagaCatProds(CatProds catP) {
 	if(catP){
 		int i;

 		for(i = 0; i < MAX_AVL; ++i)
 			apagaAVL(catP->catalogo[i]);
 		free(catP);
 	}
}

LStrings prodsPorLetra(CatProds catP, char l) {	
	LStrings lProdsPorLetra = NULL;

	if(isupper(l)){ 
		int i = l - 'A';
		int total = tamanho(catP->catalogo[i]);
		Produto* prods;
		char** arrCods;

		prods = (Produto*) inorder(catP->catalogo[i]);
		if(prods == NULL) /* falha de alocação na inorder() da AVL */
			return NULL;
		
		arrCods	= malloc(total * sizeof(char *));
		if(arrCods == NULL){ /* falha de alocação */
			apagaArray((void**) prods, total, liberta);
			return NULL;
		}

		for(i = 0; i < total; ++i){
			char* copia = obterCodigoProduto(prods[i]);

			if(copia == NULL){ /* falha a obter o código do produto */
				apagaArray((void**) prods, total, liberta);
				apagaArray((void**) arrCods, i, free);
				return NULL;
			}
			arrCods[i] = copia;
		}
		lProdsPorLetra = criaLStrings(total, arrCods);
		apagaArray((void**) prods, total, liberta);
		apagaArray((void**) arrCods, i, free); 
	}
	return lProdsPorLetra;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Produto */
static int compara(const void* p1, const void* p2)
{	
	return comparaCodigosProduto((Produto) p1, (Produto) p2);
}

static ValorNodo duplica(void* prod)
{
	return (ValorNodo) duplicaProduto((Produto) prod);
}

static void liberta(void* prod)
{
	apagaProduto((Produto) prod);
}

static void atualiza(void* prod1, void* prod2)
{
	return;
}
