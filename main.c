/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);
Lista_tabelas *lista_tabelas = NULL;
Tabela *tabela_global = NULL;
Tabela *tabela_escopo = NULL;
int tipo_atual = -1;

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  exporta (arvore);
  libera(arvore);
  yylex_destroy();
  return ret;
}
