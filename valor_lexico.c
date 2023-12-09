/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "valor_lexico.h"

meuValorLexico define_yyval(char* yytext, tipo_t tipo, int num_lines) 
{  
      meuValorLexico valor_lexico;
      valor_lexico.linha=num_lines;
      valor_lexico.tipo=tipo;
      valor_lexico.valor_token = strdup(yytext);
      return valor_lexico;
}

void libera_vl(meuValorLexico valor_lexico)
{
   free(valor_lexico.valor_token);
}