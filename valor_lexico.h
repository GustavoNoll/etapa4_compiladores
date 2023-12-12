/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/
#pragma once
typedef enum nat_tipo
{
    IDENTIFICADOR,
    LITERAL,
    FUNCAO,
    NAO_DEFINIDO
} nat_tipo;

typedef enum tipo
{
    INT,
    FLOAT,
    BOOL,
} tipo;

typedef struct valorLexico
{
    int linha;
    tipo tipo;
    char *valor_token;
    nat_tipo natureza_token;
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

meuValorLexico define_yyval(char* yytext, nat_tipo tipo, int num_lines, int tamanho_token);
void libera_vl(meuValorLexico valor_lexico);
