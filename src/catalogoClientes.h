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
void removeCatClientes(CatClientes);

/* Funções que manipulam conjuntos de clientes */
ConjClientes criaConjClientes(int, Cliente*);
void removeConjClientes(ConjClientes);
char** obterCodigosC(ConjClientes);
int cardinalidade(ConjClientes);
int obterPag(ConjClientes conjuntoC);
int obterIndice(ConjClientes);
ConjClientes clientesPorLetra(CatClientes, char);

#endif
