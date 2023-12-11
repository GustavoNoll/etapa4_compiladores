/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/
#pragma once
typedef enum tipo
{
    IDENTIFICADOR,
    LITERAL,
    FUNCAO,
    NAO_DEFINIDO
} tipo_t;

/* Constantes para definir a natureza de um nodo da AST, ou de um identificador da linguagem. */
#define LITERAL 0
#define VARIABLE 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7
#define FUNCTION 8


/* Constantes para associar um valor inteiro a cada um dos tres tipos de dados da linguagem. */
#define INT 0
#define FLOAT 1
#define BOOL 2

typedef struct valorLexico
{
    int linha;
    tipo_t tipo;
    char *valor_token;
    int natureza_token;
    int tamanho_token;
} meuValorLexico;

typedef struct tabela
{
	meuValorLexico *info;
	struct tabela *proximo;

} Tabela;

typedef struct lista_tabelas
{
	struct lista_tabelas *proximo;
	struct lista_tabelas *anterior;
	Tabela *tabela_simbolos;

} Lista_tabelas;

meuValorLexico define_yyval(char* yytext, tipo_t tipo, int num_lines, int tamanho_token);
void libera_vl(meuValorLexico valor_lexico);
