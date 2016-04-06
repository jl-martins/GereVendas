#include <catalogoProds.h>
#include <ctype.h>

#define MAX_AVL 26

typedef struct cprods {
	AVL catalogo[MAX_AVL];
} Cprods;

typedef struct prod {
	char* prod;
} Prod;

typedef struct prods {
	char** produtos;
} Prods;

static int compara(Produto, Produto);
static char calculaIndiceAVL(Produto);

CatProds initCatProds(int (*compara) (const void *, const void *))
{
	CatProds cat;
	int i;

	for(i = 0; i < MAX_AVL; i++)
		cat->catalogo[i] = criaAVL(compara);
	
	return cat;
}

CatProds insereProduto(CatProds c, Produto p)
{
	int i = calculaIndiceAVL(p);
	
	c->catalogo[i] = insereAVL(c->catalogo[i], p);
	return c;
}

Boolean existeProduto(CatProds cp, Produto p)
{
	int i = calculaIndiceAVL(p);

	return existeAVl(c->catalogo[i], p);
}

int totalProdutosLetra(CatProds cp, char l)
{
	int i = isUpper(l) ? l - 'A' : -1;

	return (i == -1) ? 0 : tamanho(cp->catalogo[i]);
}

int totalProdutos(CatProds cp)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; i++)
	{
		total += tamanho(cp->catalogo[i]);
	}
	return total;
}

void removeCatProds(CatProds cp)
{
	int i;
	
	for(int i = 0; i < MAX_AVL; i++)
		apagaAVL(cp->catalogo[i]);

	free(cp); /*??*/
}

Lista_Prods listaProdutos(CatProds, char l)
{
	
}

static int calculaIndiceAVL(Produto p)
{	
	return p->prod[0] - 'A';
}


