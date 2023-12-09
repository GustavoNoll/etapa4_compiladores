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
    NAO_DEFINIDO
} tipo_t;


typedef struct valorLexico
{
    int linha;
    tipo_t tipo;
    char *valor_token;
} meuValorLexico;

meuValorLexico define_yyval(char* yytext, tipo_t tipo, int num_lines);
void libera_vl(meuValorLexico valor_lexico);
