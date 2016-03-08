#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* tamanhos de arrays */
#define NCLIENTES 20000
#define NPRODUTOS 200000
#define MAXLINHA 64 /* tamanho do buffer usada na fgets */
#define TAM_CODIGOS 10 /* tamanho máximo dos códigos de cliente/produto */

/* caminhos de ficheiros */
#define FCLIENTES "../data/Clientes.txt" /* caminho do ficheiro de clientes */
#define FPRODUTOS "../data/Produtos.txt" /* caminho do ficheiro de produtos */
#define FVENDAS "../data/Vendas_1M.txt"     /* caminho do ficheiro de vendas */
#define FVENDAS_VAL "../data/VendasVal.dat" /* caminho do ficheiro com as vendas válidas */

/* erro de abertura de ficheiro e saída do programa com -1 */
#define OERROR_AND_EXIT(file_name) {perror(file_name); exit(-1);}

typedef struct{
	char codigoProduto[TAM_CODIGOS];
	double preco;
	int nUnidades;
	char tipoCompra;
	char codigoCliente[TAM_CODIGOS];
	int mes;
	int filial;
} venda_t;

int fileToStrArr(char *filename, char strArr[][TAM_CODIGOS]);
void * pesquisaBin(char *str, char strArr[][TAM_CODIGOS], int len);
int comparaStrings(const void *str1, const void *str2);

void criaVenda(char *campos_venda[7], venda_t *v_ptr);
int criaFvendasVal(FILE *fp, char clientes[][TAM_CODIGOS], int nclientes, char produtos[][TAM_CODIGOS], int nprods);

int main(){
	int nclientes, nprods, nvendas_val;
	char clientes[NCLIENTES][TAM_CODIGOS];
	char produtos[NPRODUTOS][TAM_CODIGOS];
	FILE *fp;

	nclientes = fileToStrArr(FCLIENTES, clientes);	
    nprods = fileToStrArr(FPRODUTOS, produtos);

	fp = fopen(FVENDAS, "r");

	if(fp == NULL)
		OERROR_AND_EXIT(FVENDAS);
	/* tenta criar ficheiro com informação sobre as vendas válidas */	
	nvendas_val = criaFvendasVal(fp, clientes, nclientes, produtos, nprods);
	
	if(nvendas_val == -1)
		fprintf(stderr, "Erro: Não foi possível criar o ficheiro de vendas válidas\n");

	fclose(fp);
	printf("nº produtos: %d | nº clientes: %d | nº vendas válidas: %d\n", nprods, nclientes, nvendas_val);

	return 0;
}

/* passa cada linha de um ficheiro para uma string, produzindo um
   array de strings ordenado lexicograficamente */
/*
int fileToStrArr(FILE *fp, char strArr[][TAM_CODIGOS], int nlinhas){
	int i = 0;
	char linha[MAXLINHA];
	char *token;

	while(i < nlinhas && fgets(linha, MAXLINHA, fp) != NULL){
		if((int) strlen(linha) < TAM_CODIGOS){
			token = strtok(linha, "\r\n");
			strcpy(strArr[i++], token);
		}
	}

	qsort(strArr, i, TAM_CODIGOS, comparaStrings); 

	return i; 
}
*/

/* Dúvida: os ficheiros de clientes e produtos estão bem formatados(em termos de fim de linhas, etc) ??*/

int fileToStrArr(char * filename, char strArr[][TAM_CODIGOS]){
	int i;
    FILE *fp = fopen(filename, "r");

	if(fp == NULL) OERROR_AND_EXIT(filename);
	for(i = 0; fgets(strArr[i], TAM_CODIGOS, fp); i++)
        strtok(strArr[i], "\r\n");
	fclose(fp);
	qsort(strArr, i, TAM_CODIGOS * sizeof(char), comparaStrings); 
	return i; /* #strings lidas */
}

/* cria um ficheiro com informação sobre as vendas válidas.
   Devolve: o número de vendas válidas em caso de sucesso. -1 caso contrário. */
int criaFvendasVal(FILE *fp, char clientes[][TAM_CODIGOS], int nclientes, char produtos[][TAM_CODIGOS], int nprods) {
	venda_t v;
	int i, nvendas_val;
	char linha_venda[MAXLINHA];
	char *campos_venda[7];
	FILE *fdest;
		
	fdest = fopen(FVENDAS_VAL, "w");

	if(fdest == NULL)
		return -1;

	nvendas_val = 0;
	while(fgets(linha_venda, MAXLINHA, fp) != NULL){

		campos_venda[0] = strtok(linha_venda, " ");
		for(i = 1; i < 7; ++i)
			campos_venda[i] = strtok(NULL, " ");

		campos_venda[i-1] = strtok(campos_venda[i-1], "\r\n"); /* remove newlines e/ou carriage returns */

		/* testa se a venda é válida e se for, cria struct com os dados,
		   escreve-a em ficheiro e incrementa nº de vendas válidas */
		if(pesquisaBin(campos_venda[0], produtos, nprods) &&
		   pesquisaBin(campos_venda[4], clientes, nclientes)){
				criaVenda(campos_venda, &v);
				fwrite(&v, sizeof(venda_t), 1, fdest);
				++nvendas_val;
		}
	}

	fclose(fdest);
	return nvendas_val;
}

/* cria uma struct com os dados de uma venda a partir de um
   array de strings com as várias informações sobre a mesma. */
void criaVenda(char *campos_venda[7], venda_t *v_ptr){
	
	strcpy(v_ptr->codigoProduto, campos_venda[0]);

	v_ptr->preco = atof(campos_venda[1]);
	v_ptr->nUnidades = atoi(campos_venda[2]);
	v_ptr->tipoCompra = campos_venda[3][0];

	strcpy(v_ptr->codigoCliente, campos_venda[4]);

	v_ptr->mes = atoi(campos_venda[5]);
	v_ptr->filial = atoi(campos_venda[6]);
}

/* pesquisa binária num array de strings */
/*
int pesquisaBin(char *str, char strArr[][TAM_CODIGOS], int len){
	int l, m, u; 
	int res; 

	l = 0;
	u = len-1;
	while(l < u){
		m = (l + u)/2;

		res = strcmp(str, strArr[m]);
		if(res < 0)
			u = m - 1;
		else if(res > 0)
			l = m + 1;
		else
			l = u = m;
	}

	return (l == u && strcmp(str, strArr[l]) == 0) ? l : -1;
}*/

void * pesquisaBin(char *str, char strArr[][TAM_CODIGOS], int len){
	return bsearch(str, strArr, len, TAM_CODIGOS, comparaStrings);
}

/* função de comparação utilizada no qsort para ordenar um array de strings */
int comparaStrings(const void *str1, const void *str2){
	return strcmp((char *) str1, (char *) str2);
}