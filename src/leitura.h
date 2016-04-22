#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h> /* definição de FILE, necessária para o cabeçalho de leLinha() */

/* Lê uma linha com o tamanho especificado para o buffer passado como argumento.
 * O(s) caratere(s) de newline é/são removidos e é devolvida a linha lida.
 * NOTA: São lidos no máximo 'tamanho' - 1 carateres, porque o útlimo caratere é o '\0'. */
char* leLinha(char buffer[], int tamanho, FILE* stream);

/* Devolve o índice do primeiro carater de uma string que não é um espaço.
 * NOTA: A definição de espaço está de acordo com a da função isspace() */
int avancaEspacosInicio(char str[]);

int leInt();
double leDouble();
int leChar();

#endif
