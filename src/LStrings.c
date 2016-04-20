#include "LStrings.h"
#include "memUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lStrings{
	int total;
	char** strings;
	int pag;
	int indice;
};

struct pagina{
	int total; /* total de strings na página */
	char** strings;
	int indice; /* índice da próxima string a devolver com obterPagina() */
};

/* Cria uma lista de Strings */
LStrings criaLStrings(int total, char* strings[])
{	
	int i, len;
	LStrings lStr = malloc(sizeof(struct lStrings));

	if(lStr == NULL)
		return NULL;
	if(total <= 0){ /* cria uma LStrings com 0 elementos */
		lStr->total = 0;
		lStr->strings = NULL;
	}
	else{
		lStr->strings = malloc(total * sizeof(char *));
		if(lStr->strings == NULL){ /* falha de alocação do array de strings */
			free(lStr);
			return NULL;
		}
		/* Copia as o conteúdo de strings[] para lStr->strings[] */
		for(i = 0; i < total; ++i){
			len = strlen(strings[i]);
			lStr->strings[i] = malloc((len + 1) * sizeof(char));

			if(lStr->strings[i] == NULL){ /* falha de alocação de lStr->strings[i] */
				lStr->total = i;
				apagaLStrings(lStr);
				return NULL;
			}
			strcpy(lStr->strings[i], strings[i]);
		}
		/* só chegamos aqui se não houve falhas de alocação */
		lStr->total = total;
	}
	lStr->pag = 1; lStr->indice = 0;
	return lStr;
}

/* Liberta a memória alocada para uma lista de Strings */
LStrings apagaLStrings(LStrings lStr)
{	
	if(lStr != NULL){
		if(lStr->strings != NULL)
			apagaArray((void**) lStr->strings, lStr->total, free);
		free(lStr);
	}
	return NULL;
}

/* Funções de navegação */

/* Devolve a página atual de uma LStrings ou NULL, em caso de erro. */
Pagina obterPag(LStrings lStr)
{
	int i, j, fimPag; /* fimPag - fim da página atual da LStrings */
	Pagina pag = malloc(sizeof(struct pagina));

	if(pag == NULL)
		return NULL;

	i = lStr->indice;
	if(i + STRINGS_POR_PAG >= lStr->total){ /* estamos na última página da LStrings */
		pag->strings = malloc((lStr->total - i) * sizeof(char *));
		fimPag = lStr->total;
	}
	else{
		pag->strings = malloc(STRINGS_POR_PAG * sizeof(char *));
		fimPag = i + STRINGS_POR_PAG;
	}
	
	if(pag->strings == NULL){ /* falha de alocação do array de strings da 'struct pagina' */
		free(pag);
		return NULL;
	}

	j = 0; /* j - índice do array de strings da 'struct pagina' */
	while(i < fimPag){
		int len = strlen(lStr->strings[i]);
		pag->strings[j] = malloc((len + 1) * sizeof(char));

		if(pag->strings[j] == NULL){ /* falha a alocar pag->strings[j] */
			apagaArray((void**) pag->strings, j, free);
			free(pag);
			return NULL;
		}
		strcpy(pag->strings[j++], lStr->strings[i++]);
	}
	/* chegamos aqui quando não houve falhas de alocação */
	pag->indice = 0; pag->total = j;
	return pag;
}

/* Liberta a memória alocada para uma página */
void apagaPag(Pagina pag)
{	
	if(pag != NULL){
		apagaArray((void**) pag->strings, pag->total, free);
		free(pag);
	}
}

/**
 * Recebe uma página e devolve a próxima linha da mesma, se ainda existirem
 * linhas por obter e se a página existir (i.e.: for diferente de NULL). 
 * Caso contrário, é devolvido NULL.
 */
char* obterLinha(Pagina pag)
{
	char* linha = NULL;

	if(pag != NULL && pag->indice < pag->total){
		int i = pag->indice;
		int len = strlen(pag->strings[i]);

		linha = malloc((len + 1) * sizeof(char));
		if(linha == NULL) /* falha de alocação */
			return NULL;
		strcpy(linha, pag->strings[i]);
		++(pag->indice);
	}
	return linha;
}

/* Devolve o número da página atual */
int obterNumPag(LStrings lStr)
{
	return lStr->pag;
}

/* Devolve o número total de Strings numa lista de strings */
int obterTotal(LStrings lStr)
{
	return lStr->total;
}

/* Devolve o número total de páginas numa LStrings */
int obterNumTotalPags(LStrings lStr)
{
	int total = lStr->total;
	int resto = total%STRINGS_POR_PAG;
	int res = (total/STRINGS_POR_PAG) + ((resto != 0) ? 1 : 0);

	return res;
}

/** 
 * A função proxPag() atualiza o índice e a página de uma LStrings
 * se a página atual não for a última. Se o utilizador estiver na
 * última página, a função proxPag() não tem qualquer efeito.
 */
void proxPag(LStrings lStr)
{	
	int i = lStr->indice;
	int total = lStr->total;

	/* não estando na última página, avança para a próxima */
	if((i + STRINGS_POR_PAG) < total){
		lStr->indice += STRINGS_POR_PAG;
		++(lStr->pag);
	}
}

/** 
 * A função pagAnt() atualiza o índice e a página de uma LStrings
 * se a página atual não for a primeira. Se o utilizador estiver na
 * primeira página, a função pagAnt() não tem qualquer efeito.
 */
void pagAnt(LStrings lStr)
{
	int i = lStr->indice;

	/* não estando na primeira página, volta à anterior */
	if(i >= STRINGS_POR_PAG){
		lStr->indice -= STRINGS_POR_PAG;
		--lStr->pag;
	}
}

/**
 * A função irParaPag() avança para a página especificada, se esta
 * não for inferior à primeira ou superior à última página da LStrings.
 */
void irParaPag(int pag, LStrings lStr)
{	
	if(pag >= 1 && pag <= obterNumTotalPags(lStr)){
		int difAbsPag = abs(lStr->pag - pag);

		if(pag < lStr->pag)
			lStr->indice -= difAbsPag * STRINGS_POR_PAG;
		else
			lStr->indice += difAbsPag * STRINGS_POR_PAG;
		lStr->pag = pag;
	}
}

/* A função primPag() coloca uma LStrings na sua primeira página. */
void primPag(LStrings lStr)
{
	lStr->pag = 1;
	lStr->indice = 0;
}

/* A função primPag() coloca uma LStrings na sua última página. */
void ultimaPag(LStrings lStr)
{	
	int ultima = obterNumTotalPags(lStr);

	lStr->indice += (ultima - lStr->pag) * STRINGS_POR_PAG;
	lStr->pag = ultima;
}
