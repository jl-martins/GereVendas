#include "catalogoClientes.h"
#include "avl.h"
#include "memUtils.h" /* funções para alocar e libertar arrays genéricos */
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
static void atualiza(void *, void *);

CatClientes criaCatClientes()
{
	int i;
	CatClientes catC = malloc(sizeof(struct catClientes));
	
	if(catC) 
		for(i = 0; i < MAX_AVL; ++i) /* agora que estamos a guardar strings, não precisamos de passar uma função de libertação */
			catC->catalogo[i] = criaAVL(atualiza, compara, duplica, NULL);
	return catC;
}

CatClientes insereCliente(CatClientes catC, Cliente c)
{
	if(catC){
		AVL nova;
		int i = calculaPos(c);
		char* codCliente = obterCodigoCliente(c);

		if(codCliente == NULL)
			return NULL;
		
		nova = insereAVL(catC->catalogo[i], codCliente);
		if(nova == NULL) /* falha de alocação a inserir na AVL */
			catC = NULL;
		else
			catC->catalogo[i] = nova;
	}
	return catC;
}

bool existeCliente(CatClientes catC, Cliente c)
{
	bool existe = FALSE;

	if(catC){
		int i = calculaPos(c);
		char* codCliente = obterCodigoCliente(c);

		if(codCliente != NULL)
			existe = existeAVL(catC->catalogo[i], c);
	}
	return existe;
}

int totalClientesLetra(CatClientes catC, char l)
{
	int i = isupper(l) ? l - 'A' : -1;
	
	return (i == -1) ? 0 : tamanhoAVL(catC->catalogo[i]);
}

int totalClientes(CatClientes catC)
{
	int total = 0;

	if(catC){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			total += tamanhoAVL(catC->catalogo[i]);
	}
	return total;
}

CatClientes apagaCatClientes(CatClientes catC)
{
	if(catC){
		int i;

		for(i = 0; i < MAX_AVL; ++i)
			apagaAVL(catC->catalogo[i]);
		free(catC); 
	}
	return NULL;
}

Cliente* todosClientes(CatClientes catC)
{
	int i, j, iclientes, quantos;
	Cliente* clientes = malloc(totalClientes(catC) * sizeof(Cliente)); 
	
	if(clientes == NULL)
		return NULL;
	iclientes = 0;
	for(i = 0; i < MAX_AVL; ++i){
		char** temp = (char **) inorderAVL(catC->catalogo[i]);
		/* fazer função de limpeza em caso de erros */
		quantos = tamanhoAVL(catC->catalogo[i]);
		for(j = 0; j < quantos; ++j)
			clientes[iclientes++] = criaCliente(temp[j]);
		
		apagaArray((void**) temp, quantos, free);
	}	
	return clientes;
}

LStrings clientesPorLetra(CatClientes catC, char l)
{	
	LStrings lClientesPorLetra = NULL;

	if(isupper(l)){ 
		int i = l - 'A';
		int total = tamanhoAVL(catC->catalogo[i]);
		char** codigosPorLetra = (char **) inorderAVL(catC->catalogo[i]);

		if(codigosPorLetra == NULL) /* falha de alocação na inorder() da AVL */
			return NULL;

		lClientesPorLetra = criaLStrings(total, codigosPorLetra);
		apagaArray((void**) codigosPorLetra, total, free);
	}
	return lClientesPorLetra;
}

/* Funções static passadas para criar AVLs */

/* Função de comparação entre dois códigos de cliente */
static int compara(const void* cod1, const void* cod2)
{	
	return strcmp((char *) cod1, (char *) cod2);
}

static void* duplica(void* codCliente)
{	
	char* original = codCliente;
	int len = strlen(original);
	char* copia = malloc((len + 1) * sizeof(char));

	if(copia != NULL)
		strcpy(copia, original);
	return copia;
}

static void atualiza(void* x, void* y)
{
	return;
}
