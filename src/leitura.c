#include "leitura.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LE_INT_BUFF 32
#define LE_DOUBLE_BUFF 512

char* leLinha(int tamanho)
{	
	char* linha = malloc((tamanho + 1) * sizeof(char)); /* (tamanho+1) porque o último carater é o '\0 */
	
	if(linha == NULL) /* falha de alocação */
		return NULL;

	if(fgets(linha, tamanho, stdin) == NULL){
		free(linha);
		linha = NULL;
	}
	else{
		int len = strlen(linha);

		if(linha[len-1] == '\r' || linha[len-1] == '\n')
			linha[len-1] = '\0';
		else /* ficaram carateres no stdin porque o utilizador introduziu mais do que 'tamanho' */
			FLUSH_STDIN();
	}
	return linha;
}

char* avancaEspacosInicio(char str[])
{
	int i = 0;

	while(str[i] != '\0' && isspace(str[i]))
		++i;
	return &str[i];
}

/* Lê um inteiro e devolve-o à função chamadora. Se o utilizador passar
 * uma mensagem como argumento, leInt() apresenta-a antes de ler o inteiro */
int leInt()
{
	int r = 0;
	char buffer[LE_INT_BUFF];
	/* ver quando é preciso fazer flush do stdin */
	if(fgets(buffer, LE_INT_BUFF, stdin))
		r = atoi(buffer);
	return r;
}

double leDouble()
{
	double r = 0;
	char buffer[LE_DOUBLE_BUFF];
	/* ver quando é preciso fazer flush do stdin */
	if(fgets(buffer, LE_INT_BUFF, stdin))
		r = atoi(buffer);
	return r;
}
