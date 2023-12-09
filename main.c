/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/
#include <stdio.h>
#include <string.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  exporta (arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
