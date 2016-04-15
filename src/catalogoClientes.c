#include "catalogoClientes.h"
#include "avl.h"
#include "memUtils.h" /* funções para alocar e libertar arrays de apontadores genéricos */
#include <ctype.h>
#include <stdlib.h>
#include <string.h> /* importa strcmp() */

/* número de AVLs usadas no catálogo */
#define calculaPos(c) ((inicioCodigoCliente((c))) -'A')
#define MAX_AVL 26 

struct catClientes {
	AVL catalogo[MAX_AVL];
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
			catC->catalogo[i] = criaAVLgenerica(atualiza, compara, duplica, liberta);
	return catC;
}

CatClientes insereCliente(CatClientes catC, Cliente c)
{
	if(catC){
		AVL nova;
		int i = calculaPos(c);

		nova = insere(catC->catalogo[i], c);
		if(nova == NULL) /* falha de alocação a inserir na AVL */
			return NULL;
		catC->catalogo[i] = nova;
	}
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
	int total = 0;

	if(catC){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			total += tamanho(catC->catalogo[i]);
	}
	return total;
}

void apagaCatClientes(CatClientes catC)
{
	if(catC){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			apagaAVL(catC->catalogo[i]);
		free(catC); 
	}
}

/* Funções que manipulam conjuntos de clientes */

LStrings clientesPorLetra(CatClientes catC, char l) {	
	LStrings lClientesPorLetra = NULL;

	if(isupper(l)){ 
		int i = l - 'A';
		int total = tamanho(catC->catalogo[i]);
		Cliente* clientes;
		char** arrCods;

		clientes = (Cliente*) inorder(catC->catalogo[i]);
		if(clientes == NULL) /* falha de alocação na inorder() da AVL */
			return NULL;
		
		arrCods	= malloc(total * sizeof(char *));
		if(arrCods == NULL){ /* falha de alocação do array de códigos de clientes */
			apagaArray((void *) clientes, total, liberta);
			return NULL;
		}
		for(i = 0; i < total; ++i){
			char* copia = obterCodigoCliente(clientes[i]);

			if(copia == NULL){ /* falha a obter o código do cliente */
				apagaArray((void**) clientes, total, liberta);
				apagaArray((void**) arrCods, i, free);
				return NULL;
			}
			arrCods[i] = copia;
		}
		lClientesPorLetra = criaLStrings(total, arrCods);
		apagaArray((void**) clientes, total, liberta);
		apagaArray((void**) arrCods, i, free); 
	}
	return lClientesPorLetra;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois elementos do tipo Cliente */
static int compara(const void *cliente1, const void *cliente2)
{	
	return comparaCodigosCliente((Cliente) cliente1, (Cliente) cliente2);
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
