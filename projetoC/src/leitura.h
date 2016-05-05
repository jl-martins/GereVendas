/**
 * @file leitura.h
 * @author LI3_Grupo1
 * @date 23 de Abril de 2016
 * @brief Ficheiro com a assinatura de várias funções para usar na leitura e processamento de input.
 */

#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h> /* definição de FILE, necessária para o cabeçalho de leLinha() */

/* Se o buffer do stdin tiver carateres, consome-os até chegar ao '\r', \n' ou EOF */
#define FLUSH_STDIN() { int c; while((c = getchar()) != EOF && (c != '\r' || c != '\n')); }

/**
 * Lê, a partir de 'stream' uma linha com o tamanho especificado e guarda-a no buffer 
 * passado como argumento, sem o(s) caratere(s) de newline. São lidos no máximo 'tamanho' - 1
 * carateres, porque o útlimo caratere é o '\0'. Se for lido o EOF antes de qualquer outra carater,
 * a função leLinha() devolve NULL, se não, devolve a linha lida à função chamadora. Se ficarem
 * caratéres no buffer do stdin após a leitura da linha, é feito o flush do stdin.
 */
char* leLinha(char buffer[], int tamanho, FILE* stream);

/** 
 * @brief Devolve o índice do primeiro carater de uma string que não é um espaço.
 * A definição de espaço está de acordo com a da função isspace() de <ctype.h>.
 */
int avancaEspacosInicio(char str[]);

/** @brief Lê um valor inteiro e faz o flush do stdin, se necessário. */
int leInt();

/** @brief Lê um double e faz o flush do stdin, se necessário. */
double leDouble();

/** @brief Lê um char e faz o flush do stdin, se necessário. */
int leChar();

#endif
