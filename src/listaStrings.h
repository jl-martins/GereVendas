#ifndef LISTA_STRINGS_H
#define LISTA_STRINGS_H

#define STRINGS_POR_PAG 32

typedef struct listaStrings* LStrings;

LStrings criaLStrings(int tamanho, char** elems);
LStrings insereLStrings(char* str, LStrings l);
LStrings removeLStrings(char* str, LStrings l);
void apagaLStrings(LStrings l);

/* Funções de navegação */
char** obterPag(LStrings l);
int proxPag(LStrings l);
int pagAnt(LStrings l);
int irParaPag(LStrings l);
int primPag(LStrings l);
int ultimaPag(LStrings l);
