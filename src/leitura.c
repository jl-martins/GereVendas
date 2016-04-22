#include "leitura.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define LE_INT_BUFF 32
#define LE_CHAR_BUFF 128
#define LE_DOUBLE_BUFF 512

char* leLinha(char buffer[], int tamanho, FILE* stream)
{	
	char* linha = fgets(buffer, tamanho, stream);
	
	if(linha != NULL){
		int i = strcspn(linha, "\r\n");	
		linha[i] = '\0'; /* remove o carater de newline */
	}
	return linha;
}

int avancaEspacosInicio(char str[])
{
	int i = 0;

	while(str[i] != '\0' && isspace(str[i]))
		++i;
	return i;
}

/* Lê um inteiro e devolve-o à função chamadora. Se o utilizador passar
 * uma mensagem como argumento, leInt() apresenta-a antes de ler o inteiro */
int leInt()
{
	int r = 0;
	char buffer[LE_INT_BUFF];
	
	if(fgets(buffer, LE_INT_BUFF, stdin))
		r = atoi(buffer);

	return r;
}

double leDouble()
{
	double r = 0;
	char buffer[LE_DOUBLE_BUFF];

	if(fgets(buffer, LE_DOUBLE_BUFF, stdin))
		r = atof(buffer);
	return r;
}

int leChar()
{
	int c;
	char buffer[LE_CHAR_BUFF];

	if(fgets(buffer, LE_CHAR_BUFF, stdin))
		c = buffer[0];
	else
		c = EOF;
	return c;
}