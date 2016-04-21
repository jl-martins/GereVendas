#ifndef LEITURA_H
#define LEITURA_H

#define FLUSH_STDIN() while(getchar() != '\n')

/* Lê uma linha que tem no máximo o tamanho especificado.
 * Se for lido EOF antes de qualquer outro carater, a função
 * leLinha() devolve NULL, se não devolve a linha lida, sem
 * carater(es) de "newline" no fim. Se forem lidos mais carateres
 * do que 'tamanho', é feito o flush do stdin, para eliminar
 * o excesso de carateres introduzidos. */
char* leLinha(int tamanho);
/* Avança os espaços do início de uma string, até encontrar
 * um carater que não seja um espaço ou '\0'. Considera-se
 * um espaço, todo o carater 'c' tal que isspace(c) == TRUE */
char* avancaEspacosInicio(char str[]);
/* Lê um inteiro e faz o flush do stdin quando necessário */
int leInt();
/* Lê um double e faz o flush do stdin quando necessário */
double leDouble();

#endif
