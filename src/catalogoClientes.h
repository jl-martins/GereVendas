#ifndef CATALOGO_CLIENTES_H
#define CATALOGO_CLIENTES_H

#include "cliente.h" /* definição do tipo Cliente */
#include "bool.h"

typedef struct catClientes* CatClientes;
typedef struct conjClientes* ConjClientes;

/* Funções que manipulam catálogos de clientes */
CatClientes criaCatClientes();
CatClientes insereCliente(CatClientes, Cliente);
bool existeCliente(CatClientes, Cliente);
int totalClientes(CatClientes);
int totalClientesLetra(CatClientes, char);
void apagaCatClientes(CatClientes);

/* Funções que manipulam conjuntos de clientes */
ConjClientes criaConjClientes(int, Cliente*);
void apagaConjClientes(ConjClientes);
char** obterCodigosClientes(ConjClientes);
int numeroClientes(ConjClientes);
int obterPagConjClientes(ConjClientes conjuntoC);
int obterIndiceConjClientes(ConjClientes);
ConjClientes clientesPorLetra(CatClientes, char);

#endif
