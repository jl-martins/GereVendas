#include <catalogoProds.h>

#define MAX_AVL 26

struct prods { AVL catalogo[MAX_AVL]; } cprods;

struct prod { char* prod; } produto;

struct conjprods { } Conj_Prods;

struct listaprods { } Lista_Prods;

static int compara(Produto, Produto);

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
	char j = p->prod[0];
	j = j - 'A';
	c->catalogo[j] = insereAVL(c->catalogo[j], p);
	return c;
}


