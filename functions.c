/*
Integrantes do grupo V
- Bruno Marques Bastos (314518)
- Gustavo Lopes Noll (322864)
*/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "valor_lexico.h"
#include "ast.h"
#include "functions.h"

static int line_number = 1; // Variável estática para rastrear o número da linha

// Implementação da função para obter o número da linha
int get_line_number(void)
{
    return line_number;
}

// Função para incrementar o número da linha
void increment_line_number(void)
{
    line_number++;
}

// Identificação de erro
void yyerror(const char *s)
{
    printf("Erro na linha %d: %s\n", get_line_number(), s);
}

/* Recebe uma tabela de simbolos e uma entrada, e insere as informacoes da entrada na tabela. */
void insereEntradaTabela(Tabela** tabela, meuValorLexico valor_lexico)
{
    Tabela* novo = (Tabela*)malloc(sizeof(Tabela));
    novo->info = (meuValorLexico*)malloc(sizeof(meuValorLexico));

    novo->info->valor_token = strdup(valor_lexico.valor_token);
    novo->info->linha = valor_lexico.linha;
    novo->info->natureza_token = valor_lexico.natureza_token;
    novo->info->tipo = valor_lexico.tipo;
    novo->info->tamanho_token = valor_lexico.tamanho_token;

    novo->proximo = NULL;

    if (*tabela == NULL)
        *tabela = novo;
    else
    {
        Tabela* atual = *tabela;
        while (atual->proximo != NULL)
            atual = atual->proximo;
        atual->proximo = novo;
    }
}

/* Recebe uma lista de tabela de simbolos e uma entrada, e insere a entrada na ultima tabela de simbolos da lista. */
void insereUltimaTabela(Lista_tabelas** lista_tabelas, meuValorLexico valor_lexico) 
{   
    if (*lista_tabelas == NULL || valor_lexico.valor_token == NULL) {
        return;
    }
    
    Lista_tabelas* atual = *lista_tabelas;
    
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    
   insereEntradaTabela(&(atual->tabela_simbolos), valor_lexico);
}

/* Insere uma nova tabela de simbolos na lista de tabela de simbolos. */
void pushTabela(Lista_tabelas **lista, Tabela *nova_tabela)
{
    //printf("EMPILHOU\n");
    
    Lista_tabelas* novo = (Lista_tabelas*)malloc(sizeof(Lista_tabelas));
    novo->tabela_simbolos = nova_tabela;
    novo->proximo = NULL;

    if (*lista == NULL)
    {
        novo->anterior = NULL;
        *lista = novo;
    }

    else
    {
        Lista_tabelas* atual = *lista;
        while (atual->proximo != NULL)
            atual = atual->proximo;
        atual->proximo = novo;
        novo->anterior = atual;
    }
}
/* Destroi uma tabela de simbolos, liberando o espaco de memoria referente a ela. */
void destroiTabela(Tabela** tabela) 
{
    Tabela* atual = *tabela;
    Tabela* proximo;

    while (atual != NULL) 
    {
        proximo = atual->proximo;
        free(atual->info->valor_token);
        free(atual->info);
        free(atual);
        atual = proximo;
    }

    *tabela = NULL;
}
/* Remove a ultima tabela de simbolos que foi adicionada na lista de tabela de simbolos. */
void popTabela(Lista_tabelas **lista_tabelas)
{
    //printf("DESEMPILHOU\n");
    
    if (*lista_tabelas == NULL) 
        return;
    
    Lista_tabelas* atual = *lista_tabelas;
    
    while (atual->proximo != NULL)
        atual = atual->proximo;
    
    if (atual->anterior == NULL) 
        *lista_tabelas = NULL;
        
    else 
        atual->anterior->proximo = NULL;
    
    destroiTabela(&(atual->tabela_simbolos));
    free(atual);
}


/* Destroi todas as tabelas de simbolos de uma lista de tabelas de simbolos. */
void destroiListaTabelas(Lista_tabelas** lista_tabelas) 
{
    Lista_tabelas* atual = *lista_tabelas;
    Lista_tabelas* proximo;

    while (atual != NULL) 
    {
        proximo = atual->proximo;
        destroiTabela(&(atual->tabela_simbolos));
        free(atual);
        atual = proximo;
    }

    *lista_tabelas = NULL;
}

/* Verifica os seguintes erros de semantica:
ERR_UNDECLARED - Caso o identificador não tenha sido declarado no seu uso;
ERR_FUNCTION   - Caso o identificador dito como funcao esteja sendo usado como variavel. */
void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, meuValorLexico identificador)
{
    Lista_tabelas *lista_atual = lista_tabelas;
    int achou_funcao = 0;

    while (lista_atual != NULL)
    {
	Tabela *tabela_atual = lista_atual->tabela_simbolos;

	while (tabela_atual != NULL)
	{
	    if (strcmp(identificador.valor_token, tabela_atual->info->valor_token) == 0)
	    {
	    	if (tabela_atual->info->natureza_token == FUNCAO)
			achou_funcao = 1;
	    	
	    	else if (tabela_atual->info->natureza_token == IDENTIFICADOR)
			return;	
	    }
	        
	    tabela_atual = tabela_atual->proximo;
	}

	lista_atual = lista_atual->proximo;
    }
    
    if (achou_funcao == 1)
    {
	printf("ERRO DE SEMANTICA - LINHA %d - FUNCAO '%s' SENDO USADA COMO VARIAVEL\n", identificador.linha, identificador.valor_token);
	exit(ERR_FUNCTION);   
    }
    
    else
    {
	printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", identificador.linha, identificador.valor_token);
	exit(ERR_UNDECLARED);   
    }
}

/* Verifica os seguintes erros de semantica, nos casos de uma chama de funcao no codigo sendo analisado:
ERR_VARIABLE   - Caso o identificador dito variável seja usado como uma função,deve-se lançar o erro ERR_VARIABLE
ERR_UNDECLARED - Caso o identificador não tenha sido declarado no seu uso; */
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha)
{
	Lista_tabelas *lista_atual = lista_tabelas;

	while (lista_atual != NULL)
	{
        	Tabela *tabela_atual = lista_atual->tabela_simbolos;

		while (tabela_atual != NULL)
		{
		    if (strcmp(valor_token, tabela_atual->info->valor_token) == 0)
		    {
                if (tabela_atual->info->natureza_token != FUNCAO)
                {
                    printf("ERRO DE SEMANTICA - LINHA %d - VARIAVEL '%s' SENDO USADA COMO FUNCAO\n", linha, valor_token);
                    exit(ERR_VARIABLE);
                }else {
                    return;
                }     	
		    }
		        
		    tabela_atual = tabela_atual->proximo;
		}

        	lista_atual = lista_atual->proximo;
    	}
    
	printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", linha, valor_token);
	exit(ERR_UNDECLARED);
}

/* Verifica o seguinte erro de semantica:
ERR_DECLARED - Nos casos em que um identificador ja declarado esteja sendo redeclarado. */
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, meuValorLexico identificador) {
    Lista_tabelas *lista_atual = lista_tabelas;

    // Verificações globais (fora de funções)
    while (lista_atual != NULL && lista_atual->proximo != NULL) {
        lista_atual = lista_atual->proximo;
    }

    if (lista_atual != NULL) {
        Tabela *tabela_atual = lista_atual->tabela_simbolos;

        while (tabela_atual != NULL) {
            if (strcmp(identificador.valor_token, tabela_atual->info->valor_token) == 0) {
                printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", identificador.linha, identificador.valor_token);
                exit(ERR_DECLARED);
            }

            tabela_atual = tabela_atual->proximo;
        }
    }

    // Verificações locais (dentro de funções)
    lista_atual = lista_tabelas;
    
    while (lista_atual != NULL) {
        Tabela *tabela_atual = lista_atual->tabela_simbolos;

        while (tabela_atual != NULL) {
            if (identificador.natureza_token != FUNCAO &&
                strcmp(identificador.valor_token, tabela_atual->info->valor_token) == 0) {
                printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", identificador.linha, identificador.valor_token);
                exit(ERR_DECLARED);
            }

            tabela_atual = tabela_atual->proximo;
        }

        lista_atual = lista_atual->proximo;
    }
}

/* Recebe um valor inteiro referente a um tipo, e retorna uma string indicando o tipo. */
char* obtemNomeTipo (int valor_tipo)
{
	char* nome_tipo = calloc(6, sizeof(char));
	if (valor_tipo == INT)
		strcpy(nome_tipo,"int");
	else if (valor_tipo == FLOAT)
		strcpy(nome_tipo,"float");
	else if (valor_tipo == BOOL)
		strcpy(nome_tipo,"bool");
	return nome_tipo;
}

/* Recebe uma string qualificando uma chamada de funcao com "call " concatenado, e obtem o nome da funcao sendo chamada. */
char* obtemNomeFuncao(char* nomeChamadaFuncao)
{
	char *nomeFuncao = calloc(50,sizeof(char));
	strcpy(nomeFuncao,nomeChamadaFuncao+5);
	return nomeFuncao;
}

/* Recebe uma tabela de simbolos, e imprime cada simbolo com suas caracteristicas. */
void imprimeTabela(Tabela *tabela)
{
	Tabela* atual = tabela;

	while (atual != NULL) 
	{
		printf("VALOR: %s\n", atual->info->valor_token);
		printf("TIPO: %s\n", obtemNomeTipo(atual->info->tipo));
		printf("NATUREZA: %d\n", atual->info->natureza_token);
		printf("LINHA: %d\n", atual->info->linha);
		printf("TAMANHO: %d\n\n", atual->info->tamanho_token);
		atual = atual->proximo;
	}
}

/* Recebe uma lista de tabelas de simbolos, e imprime a ultima tabela inserida na lista. */
void imprimeUltimaTabela(Lista_tabelas *lista_tabelas)
{
	Lista_tabelas *atual = lista_tabelas;
    	while (atual->proximo != NULL)
        	atual = atual->proximo;
	printf("TABELA LOCAL:\n\n");
        imprimeTabela(atual->tabela_simbolos);
	printf("------------------\n");
}

void imprimeTodasTabelas(Lista_tabelas *lista_tabelas)
{
    Lista_tabelas *atual = lista_tabelas;
    while (atual != NULL){
        printf("TABELA LOCAL:\n\n");
        imprimeTabela(atual->tabela_simbolos);
        printf("------------------\n");
        atual = atual->proximo;
    }
}

/* Recebe dois inteiros representando um tipo de duas variaveis envolvidas em uma operacao,
e infere qual sera o tipo do resultado da operacao. */
int infereTipo(int tipo1, int tipo2)
{
	if (tipo1 == FLOAT || tipo2 == FLOAT)
	{
		//printf("INFERIU FLOAT\n");
		return FLOAT;
	}
		
	else if (tipo1 == INT || tipo2 == INT)
	{
		//printf("INFERIU INT\n");
		return INT;
	}
	
	else if (tipo1 == BOOL || tipo2 == BOOL)
	{
		//printf("INFERIU BOOL\n");
		return BOOL;
	}
	
	else
		return -1;	
}

/* Recebe um nodo raiz de uma expressao, e decide qual sera o tipo do nodo, a partir
da inferencia de tipos aplicada na expressao. */
int infereTipoExpressao(Nodo *raiz) 
{
    int tipo_encontrado = -1;
    
    if (raiz != NULL) 
    {
        //printf("ACHOU %d\n", raiz->info->tipo);
        
        // Atualiza o tipo encontrado com o tipo do nodo atual
        tipo_encontrado = raiz->valor_lexico.tipo;


        Nodo *filhoAtual = raiz->filho;
        while (filhoAtual != NULL) {
            tipo_encontrado = infereTipo(tipo_encontrado, infereTipoExpressao(filhoAtual));
            filhoAtual = filhoAtual->irmao;  // Move para o próximo filho
        }
    }
    
    return tipo_encontrado;
}     

/* Recebe uma string representando um tipo, e retorna o valor inteiro que
corresponde a esse tipo. */
int verificaTipo(char *tipo)
{
	if (strcmp(tipo,"int") == 0)
		return INT;
	else if (strcmp(tipo,"float") == 0)
		return FLOAT;
	else if (strcmp(tipo,"bool") == 0)
		return BOOL;
	else
		return -1;
}

/* Recebe um identificador, e obtem seu tipo, que esta informado na ultima tabela de simbolos inserida. */
int obtemTipo(Lista_tabelas *lista_tabelas, meuValorLexico identificador)
{
    Lista_tabelas *lista_atual = lista_tabelas;
    int tipo_atual = -1;

    while (lista_atual != NULL)
    {
        Tabela *tabela_atual = lista_atual->tabela_simbolos;

        while (tabela_atual != NULL)
        {
            if (strcmp(identificador.valor_token, tabela_atual->info->valor_token) == 0)
		tipo_atual = tabela_atual->info->tipo; 
            tabela_atual = tabela_atual->proximo;
        }

        lista_atual = lista_atual->proximo;
    }
    
    return tipo_atual;	
}

/* Recebe um inteiro que caracteriza um tipo, e retorna o tamanho em memoria ocupado por esse tipo. */
int infereTamanho(int tipo)
{
	if (tipo == INT)
		return 4;
	else if (tipo == FLOAT)
		return 4;
	else if (tipo == BOOL)
		return 1;
	else
		return 0;
}