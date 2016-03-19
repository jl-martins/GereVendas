/* Módulo de filtragem das vendas válidas */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* tamanhos de arrays */
#define NCLIENTES 20000
#define NPRODUTOS 200000
#define MAXLINHA 64 /* tamanho do buffer usada na fgets */
#define MAXLINHA_VENDAS 128
#define TAM_CODIGOS 10 /* tamanho máximo dos códigos de cliente/produto */
#define NUM_CAMPOS 7

/* caminhos de ficheiros */
/* sugestão: programar de forma a que se possa passar os caminhos dos ficheiros no argvs */
#define FCLIENTES "../data/Clientes.txt" /* caminho do ficheiro de clientes */
#define FPRODUTOS "../data/Produtos.txt" /* caminho do ficheiro de produtos */
#define FVENDAS "../data/Vendas_1M.txt"     /* caminho do ficheiro de vendas */
#define FVENDAS_VAL "../data/VendasValidas.txt" /* caminho do ficheiro com as vendas válidas */

/* erro de abertura de ficheiro e saída do programa com -1 */
#define OERROR_AND_EXIT(file_name) {perror(file_name); exit(-1);}

/*Avalia os preços (0.0 - 999.99)*/
#define TESTAPRECO(p) ((p)>=0.0 && (p)<=999.99 ? (p) : -1)
/*Avalia numero de unidades compradas*/
#define TESTANMR_UNI(n) ((n)>=1 && (n)<=200 ? (n) : 0)
/*Avalia o mês da compra*/
#define TESTAMES(m) ((m)>=1 && (m)<=12 ? (m) : 0)
/*Avalia o tipo de compra*/
#define TESTATIPO(t) (t=='N' || t=='P' ? t : 0)
/*Avalia a filial */
#define TESTAFILIAL(f) ((f)>=1 && (f)<=3 ? (f) : 0)


/* SUGESTÂO: passar struct para .h e definir os tipos com macros para ser mais fácil alterar se houvewr mudanças */


enum campoVenda {CODIGO_PROD = 0, PRECO, UNIDADES, TIPO_COMPRA, CODIGO_CLIENTE, MES, FILIAL};

typedef struct{
	char codigoProduto[TAM_CODIGOS];
	char codigoCliente[TAM_CODIGOS];
	double preco;
	int nUnidades;
	int mes;
	int filial;
	char tipoCompra;
} venda_t;


int fileToStrArr(char *filename, char strArr[][TAM_CODIGOS], int nlines);
void * pesquisaBin(char *str, char strArr[][TAM_CODIGOS], int len);
int comparaStrings(const void *str1, const void *str2);

void criaVenda(char *campos_venda[NUM_CAMPOS], venda_t *v_ptr);
int criaFvendasVal(FILE *fp, char clientes[][TAM_CODIGOS], int nclientes, char produtos[][TAM_CODIGOS], int nprods);
int validaCamposVenda(char *campos_venda[NUM_CAMPOS]);

int main(){
	int nclientes, nprods, nvendas_val;
	char clientes[NCLIENTES][TAM_CODIGOS];
	char produtos[NPRODUTOS][TAM_CODIGOS];
	FILE *fp;

	nclientes = fileToStrArr(FCLIENTES, clientes, NCLIENTES);	
	nprods = fileToStrArr(FPRODUTOS, produtos, NPRODUTOS);

/*sugestão: passar a abertura de ficheiros e verificação de erros para dentro da função */
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

int fileToStrArr(char * filename, char strArr[][TAM_CODIGOS], int nlinhas){
	int i;
	char * token, linha[MAXLINHA];
	FILE *fp = fopen(filename, "r");

	if(fp == NULL) OERROR_AND_EXIT(filename);
	for(i = 0; i < nlinhas && fgets(linha, MAXLINHA, fp); i++){
        if(strlen(linha) < TAM_CODIGOS){
       		token = strtok(linha, "\r\n");
			strcpy(strArr[i], token);
 	    }	
	}	
	fclose(fp);
	qsort(strArr, i, TAM_CODIGOS * sizeof(char), comparaStrings); 
	return i; /* #strings lidas */
}

/* cria um ficheiro com informação sobre as vendas válidas.
   Devolve: o número de vendas válidas em caso de sucesso. -1 caso contrário. */

int criaFvendasVal(FILE *fp, char clientes[][TAM_CODIGOS], int nclientes, char produtos[][TAM_CODIGOS], int nprods) {
	int nvendas_val;
	enum campoVenda i;
	char linha_venda[MAXLINHA_VENDAS],
	     linha_venda_cpy[MAXLINHA_VENDAS];
	char *campos_venda[7];
	FILE *fdest;
		
	fdest = fopen(FVENDAS_VAL, "w");

	if(fdest == NULL)
		return -1;

	nvendas_val = 0;
	while(fgets(linha_venda, MAXLINHA_VENDAS, fp) != NULL){

		strcpy(linha_venda_cpy, linha_venda);
		campos_venda[0] = strtok(linha_venda, " ");
		for(i = 1; i < 7; ++i)
			campos_venda[i] = strtok(NULL, " \r\n");

		/* Se a estrutura for válida, insere-a no ficheiro */
		/* Sugestão: Passar as pesquisas binárias para dentro da função que valida */

		if(/*validaCamposVenda(campos_venda) && */
		   pesquisaBin(campos_venda[CODIGO_PROD], produtos, nprods) &&
		   pesquisaBin(campos_venda[CODIGO_CLIENTE], clientes, nclientes)){
				fprintf(fdest, "%s", linha_venda_cpy);
				++nvendas_val;
		}
	}

	fclose(fdest);
	return nvendas_val;
}

/* cria uma struct com os dados de uma venda a partir de um
   array de strings com as várias informações sobre a mesma. */
void criaVenda(char *campos_venda[7], venda_t *v_ptr){
	
	strcpy(v_ptr->codigoProduto, campos_venda[CODIGO_PROD]);

	v_ptr->preco = atof(campos_venda[PRECO]);
	v_ptr->nUnidades = atoi(campos_venda[UNIDADES]);
	v_ptr->tipoCompra = campos_venda[TIPO_COMPRA][0]; /* 1o char da string*/

	strcpy(v_ptr->codigoCliente, campos_venda[CODIGO_CLIENTE]);

	v_ptr->mes = atoi(campos_venda[MES]);
	v_ptr->filial = atoi(campos_venda[FILIAL]);
}

void * pesquisaBin(char *str, char strArr[][TAM_CODIGOS], int len){
	return bsearch(str, strArr, len, TAM_CODIGOS, comparaStrings);
}

/* função de comparação utilizada no qsort para ordenar um array de strings */
int comparaStrings(const void *str1, const void *str2){
	return strcmp((char *) str1, (char *) str2);
}

/* Sugestão: criar uma macro para o numero de campos de venda em vez de termos o 7 harcoded */
/* há o perigo de não estarmos a verificar a sintaxe dos campos numericos e a função atof devolver um valor que não devia */
int validaCamposVenda(char *campos_venda[7])
{
	return TESTAPRECO(atof(campos_venda[PRECO])) != -1 &&
		   TESTANMR_UNI(atoi(campos_venda[UNIDADES])) &&
		   TESTAMES(atoi(campos_venda[MES])) &&
		   TESTATIPO(campos_venda[TIPO_COMPRA][0]) &&
		   TESTAFILIAL(atoi(campos_venda[FILIAL]));
}
