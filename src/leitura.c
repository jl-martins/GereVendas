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

	if(fgets(linha, tamanho, stdin) == NULL){ /* chegamos ao EOF */
		free(linha);
		linha = NULL;
	}
	else{
		/* índice do 1º carater de linha que pertence à string "\r\n" ou do '\0', se a string não tiver nem '\r' nem '\n' */
		int i = strcspn(linha, "\r\n");
		
		if(linha[i] == '\0') /* não foi encontrado \r nem \n. Ficaram carateres no buffer do stdin */
			FLUSH_STDIN();
		else
			linha[i] = '\0'; /* remove o carater de newline */
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
