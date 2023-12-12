/*
  Integrantes do grupo V
  - Bruno Marques Bastos (314518)
  - Gustavo Lopes Noll (322864)
*/
#include "valor_lexico.h"
#include "ast.h"

#define ERR_UNDECLARED 10 //2.2
#define ERR_DECLARED 11 //2.2
#define ERR_VARIABLE 20 //2.3
#define ERR_FUNCTION 21 //2.3

int get_line_number(void);
void increment_line_number(void);
void yyerror(const char *s);

/* FUNCOES PARA MANIPULACOES DAS TABELAS DE SIMBOLOS */
void insereEntradaTabela (Tabela** tabela, meuValorLexico valor_lexico);
void insereUltimaTabela(Lista_tabelas** lista_tabelas, meuValorLexico valor_lexico);
void popTabela(Lista_tabelas **lista);
void pushTabela(Lista_tabelas** lista, Tabela *nova_tabela);
void destroiTabela(Tabela** tabela);
void destroiListaTabelas(Lista_tabelas** lista_tabelas);
void imprimeTabela(Tabela *tabela);
void imprimeUltimaTabela(Lista_tabelas* lista_tabelas);
void imprimeTodasTabelas(Lista_tabelas *lista_tabelas);

void verificaERR_UNDECLARED_FUNCTION(Lista_tabelas *lista_tabelas, meuValorLexico identificador);
void verificaERR_DECLARED(Lista_tabelas *lista_tabelas, meuValorLexico identificador);
void verificaERR_VARIABLE_UNDECLARED_chamadafuncao(Lista_tabelas *lista_tabelas, char *valor_token, int linha_token);
int infereTipo(int tipo1, int tipo2);
int verificaTipo(char *tipo_token);
int infereTipoExpressao(Nodo *raiz);
int obtemTipo(Lista_tabelas *lista_tabelas, meuValorLexico identificador);
int infereTamanho(int tipo_token);
char* obtemNomeFuncao(char* nomeChamadaFuncao);

