/* Módulo com várias funções úteis para a leitura e processamento de input */

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

		if(linha[i] == '\0') /* ficaram carateres no stdin */
			FLUSH_STDIN()
		else
			linha[i] = '\0'; /* remove o carater de newline, se este existir */
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

int leInt()
{
	int r = 0;
	char buffer[LE_INT_BUFF];
	
	if(fgets(buffer, LE_INT_BUFF, stdin)){
		int i = strcspn(buffer, "\r\n");

		if(buffer[i] == '\0')
			FLUSH_STDIN()
		r = atoi(buffer);
	}
	return r;
}

double leDouble()
{
	double r = 0;
	char buffer[LE_DOUBLE_BUFF];

	if(fgets(buffer, LE_DOUBLE_BUFF, stdin)){
		int i = strcspn(buffer, "\r\n");

		if(buffer[i] == '\0')
			FLUSH_STDIN()
		r = atof(buffer);
	}
	return r;
}

int leChar()
{
	int c;
	char buffer[LE_CHAR_BUFF];

	if(fgets(buffer, LE_CHAR_BUFF, stdin)){
		int i = strcspn(buffer, "\r\n");

		if(buffer[i] == '\0')
			FLUSH_STDIN()
		c = buffer[0];
	}
	else
		c = EOF;
	return c;
}
