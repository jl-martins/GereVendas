#include "catalogoClientes.h"
#include "avl.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h> /* para usar a função strcmp() */

/* número de AVLs usadas no catálogo */
#define calculaPos(c) ((inicioCodigoCliente((c))) -'A')
#define MAX_AVL 26 

struct catClientes {
	AVL catalogo[MAX_AVL];
};

struct conjClientes {
	int total;
	Cliente * clientes;
	int pag;
	int i;
};

static int compara(const void *, const void *);
static void* duplica(void *);
static void liberta(void *);
static void atualiza(void *, void *);

CatClientes criaCatClientes() {
	int i;
	CatClientes catC = malloc(sizeof(struct catClientes));
	
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
	bool existe = FALSE;
	if(catC){
		int i = calculaPos(c);
		existe = existeAVL(catC->catalogo[i], c);
	}
	return existe;
}

int totalClientesLetra(CatClientes catC, char l)
{
	int i = isupper(l) ? l - 'A' : -1;
	return (i == -1) ? 0 : tamanho(catC->catalogo[i]);
}

int totalClientes(CatClientes catC)
{
	int i, total = 0;

	for(i = 0; i < MAX_AVL; ++i)
		total += tamanho(catC->catalogo[i]);

	return total;
}

void apagaCatClientes(CatClientes catC)
{
	int i;
	for(i = 0; i < MAX_AVL; ++i)
/*		apagaAVL(catC->catalogo[i]);*/
	free(catC); 
}

/* Funções que manipulam conjuntos de clientes */

ConjClientes criaConjClientes(int total, Cliente* clientes) {
	ConjClientes conjuntoC = malloc(sizeof(struct conjClientes));

	if(conjuntoC){
		conjuntoC->total = total;
		conjuntoC->clientes = clientes;
		conjuntoC->pag = 1;
		conjuntoC->i = 1;
	}
	return conjuntoC;
}

void apagaConjClientes(ConjClientes conjuntoC) {
	int i, total;

	total = conjuntoC->total;
	for(i = 0; i < total; ++i)
		apagaCliente(conjuntoC->clientes[i]);
}

/* Em casos de sucesso, a função 'obterCodigosC' devolve um array de strings 
 * ordenado crescentemente, com os códigos de produtos de um conjunto de produtos. 
 * Se ocorrer um erro, é devolvido NULL */
char** obterCodigosClientes(ConjClientes conjuntoC) {
	int total = conjuntoC->total;
	char** codigos = malloc(total * sizeof(char *));
	
	if(codigos){
		int i = 0;
		char * codigoC;
		
		for(i = 0; i < total; ++i){
			codigoC = obterCodigoCliente(conjuntoC->clientes[i]);
			if(codigoC == NULL)
				break; /* se houve uma falha de alocação, saimos do ciclo */
			codigos[i] = codigoC;
		}
		if(i < total){ /* tratamento de falhas de alocação */
			for(i = i-1; i >= 0; --i)
				free(codigos[i]);
			codigos = NULL;
		}
	}
	return codigos;
}

int numeroClientes(ConjClientes conjuntoC) {
	return conjuntoC->total;
}

int obterPagConjClientes(ConjClientes conjuntoC) {
	return conjuntoC->pag;
}

int obterIndiceConjClientes(ConjClientes conjuntoC) {
	return conjuntoC->i;
}

ConjClientes clientesPorLetra(CatClientes catC, char l) {	
	ConjClientes conjuntoC = NULL;

	if(isupper(l)){/*tratamento de erros */ 
		int i = l - 'A'; /* índice da avl a consultar */
		int total = tamanho(catC->catalogo[i]);
		Cliente* clientes = (Cliente*) inorder(catC->catalogo[i]);;
		
		conjuntoC = criaConjClientes(total, clientes); 
	}
	return conjuntoC;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Cliente */
static int compara(const void *cliente1, const void *cliente2) {	
	char* codigo1 = obterCodigoCliente((Cliente) cliente1);
	char* codigo2 = obterCodigoCliente((Cliente) cliente2);
	int r = strcmp(codigo1, codigo2);

	free(codigo1); 
	free(codigo2);

	return r;
}

static void* duplica(void * cliente)
{
	return duplicaCliente((Cliente) cliente);
}

static void liberta(void * cliente)
{
	apagaCliente((Cliente) cliente);
}

static void atualiza(void* x, void* y)
{
	return;
}
