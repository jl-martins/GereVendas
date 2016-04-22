#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h> /* definição de FILE, necessária para o cabeçalho de leLinha() */

#define FLUSH_STDIN() { int c; while((c = getchar()) != EOF && (c != '\r' || c != '\n')); }

/* Lê uma linha com o tamanho especificado e guarda-a no buffer passado como argumento,
 * sem o(s) caratere(s) de newline. São lidos no máximo 'tamanho' - 1 carateres, porque
 * o útlimo caratere é o '\0'. Se for lido o EOF antes de qualquer outra carater,
 * a função leLinha() devolve NULL, se não, devolve a linha lida à função chamadora. */
char* leLinha(char buffer[], int tamanho, FILE* stream);

/* Devolve o índice do primeiro carater de uma string que não é um espaço.
 * NOTA: A definição de espaço está de acordo com a da função isspace() */
int avancaEspacosInicio(char str[]);

int leInt();
double leDouble();
int leChar();

#endif
