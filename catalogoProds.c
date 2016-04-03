#include <catalogoProds.h>

#define MAX_AVL 26

struct cprods { AVL catalogo[MAX_AVL]; } Cprods;

struct prod { char* prod; } Prod;

struct prods { char** produtos; } Prods;

static int compara(Produto, Produto);
static char fstLetter(Produto);

CatProds initCatProds()
{
	CatProds cat;
	int i;
	for(i = 0; i < MAX_AVL; i++)
		cat->catalogo[i] = criaAVL(compara);
	
	return cat;
}

CatProds insereProduto(CatProds c, Produto p)
{
	char j = fstLetter(p);
	c->catalogo[j] = insereAVL(c->catalogo[j], p);
	return c;
}

Boolean existeProduto(CatProds cp, Produto p)
{
	char j = fstLetter(p);
	return existeAVl(c->catalogo[j], p);
}

int totalProdutosLetra(CatProds cp, char l)
{
	l = l - 'A';
	return cp->catalogo[l]->tamanho;
}

int totalProdutos(CatProds cp)
{
	int i, total = 0;
	for(i = 0; i < MAX_AVL; i++)
	{
		total += cp->catalogo[l]->tamanho;
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

static char fstLetter(Produto p)
{
	char l = p->prod[0];
	l = l - 'A';
	return j;
}


