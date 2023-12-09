%{
    /*
    Integrantes do grupo V:
    - Bruno Marques Bastos (314518)
    - Gustavo Lopes Noll (322864)
    */
    #include<stdio.h>
    #include<string.h>
    #include "ast.h"
    #include "main.h"
    #include "functions.h"
    int yylex(void);
    extern void yyerror (char const *s);
    extern int get_line_number (void);
    extern void *arvore;
%}
%define parse.error verbose
%code requires {
    #include "valor_lexico.h"
    #include "ast.h"
}
%union{
    meuValorLexico valor_lexico;
    struct Nodo* ast_no;
}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token TK_ERRO

%token '+'
%token '{'
%token '}'
%token '('
%token ')'
%token '='
%token ','
%token ';'
%token '<'
%token '>'
%token '-'
%token '%'
%token '/'
%token '*'
%token '!'

%type<ast_no> programa
%type<ast_no> elementos
%type<ast_no> elemento
%type<ast_no> definicao_funcao
%type<ast_no> cabecalho_funcao
%type<ast_no> corpo_funcao
%type<ast_no> comandos
%type<ast_no> comando
%type<ast_no> declaracao_variavel_local
%type<ast_no> atribuicao
%type<ast_no> condicao
%type<ast_no> repeticao
%type<ast_no> retorno
%type<ast_no> bloco_comandos
%type<ast_no> chamada_funcao_init
%type<ast_no> argumentos
%type<ast_no> chamada_funcao
%type<ast_no> lista_identificadores
%type<ast_no> primario
%type<ast_no> prec1
%type<ast_no> prec2
%type<ast_no> prec3
%type<ast_no> prec4
%type<ast_no> prec5
%type<ast_no> prec6
%type<ast_no> prec7
%type<ast_no> expressao
%type<ast_no> literais


%%

programa: elementos { $$ = $1; arvore = $$; }
        | /* Vazio */ { $$ = NULL; };

elementos: elemento elementos {
            if($1 != NULL && $2 != NULL){
                $$ = $1;
                adiciona_filho($$, $2);
            }
            else if($1 != NULL){
                $$ = $1;
            }
            else if($2 != NULL){
                $$ = $2;
            }
            else{
                $$ = NULL;
            }

        }
         | elemento { $$ = $1; };

elemento: declaracoes_globais { $$ = NULL; }
        | definicao_funcao { $$ = $1; };

declaracoes_globais: declaracao_variaveis_globais;

declaracao_variaveis_globais: tipo lista_identificadores ';'

tipo: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL

lista_identificadores: TK_IDENTIFICADOR { $$ = adiciona_nodo($1); }
                   | lista_identificadores ',' TK_IDENTIFICADOR { 
                    adiciona_filho($$, adiciona_nodo($3));
                    }
                   | /* Vazio */ { $$ = NULL; };

definicao_funcao: cabecalho_funcao corpo_funcao { 
    $$ = $1;
    if($2 != NULL){
        adiciona_filho($1, $2);
    }
}
               ;

cabecalho_funcao: parametros TK_OC_GE tipo '!' TK_IDENTIFICADOR { $$ = adiciona_nodo($5); }
               | tipo '!' TK_IDENTIFICADOR TK_OC_GE tipo '!' TK_IDENTIFICADOR { $$ = adiciona_nodo($7); }
               ;

parametros: '(' lista_parametros ')';

lista_parametros: parametro
               | lista_parametros ',' parametro
               | /* Vazio */
               ;

parametro: tipo TK_IDENTIFICADOR
         ;

corpo_funcao: bloco_comandos { $$ = $1; };

comandos: comando { $$ = $1; }
        | comando comandos { 
            if($1 != NULL && $2 != NULL){
                $$ = $1;
                adiciona_filho($$, $2);
            }
            else if($1 != NULL){
                $$ = $1;
            }
            else if($2 != NULL){
                $$ = $2;
            }
            else{
                $$ = NULL;
	        }
        };

comando: declaracao_variavel_local { $$ = NULL; }
       | atribuicao { $$ = $1; }
       | condicao { $$ = $1; }
       | repeticao { $$ = $1; }
       | retorno { $$ = $1; }
       | bloco_comandos ';' { $$ = $1; }
       | chamada_funcao_init { $$ = $1; }
       ;

declaracao_variavel_local: tipo lista_identificadores ';' { $$ = $2; }
                       ;

atribuicao: TK_IDENTIFICADOR '=' expressao ';' { 
    $$ = adiciona_nodo_by_label("=");
    Nodo *novo_id = adiciona_nodo($1);
    adiciona_filho($$, novo_id);
    adiciona_filho($$, $3);
    }
    ;

condicao: TK_PR_IF '(' expressao ')' bloco_comandos ';'{ 
            $$ = adiciona_nodo_by_label("if");
            adiciona_filho($$, $3);
            if ($5 != NULL){
                adiciona_filho($$, $5);
            }
        }
        | TK_PR_IF '(' expressao ')' bloco_comandos TK_PR_ELSE bloco_comandos ';'{ 
            $$ = adiciona_nodo_by_label("if");
            adiciona_filho($$, $3);
            if ($5 != NULL){
                adiciona_filho($$, $5);
            }
            if ($7 != NULL){
                adiciona_filho($$, $7);
            }
        }
        ;

repeticao: TK_PR_WHILE '(' expressao ')' bloco_comandos ';'{ 
            $$ = adiciona_nodo_by_label("while");
            adiciona_filho($$, $3);
            if($5 != NULL){
                adiciona_filho($$, $5);
            }
        }
         ;

retorno: TK_PR_RETURN expressao ';' { $$ = adiciona_filho(adiciona_nodo_by_label("return"), $2); }
       ;

bloco_comandos: '{' comandos '}'{ $$ = $2; }
             | '{' '}'{ $$ = NULL;}

chamada_funcao_init: TK_IDENTIFICADOR '(' argumentos ')' ';' { 
            $$ = adiciona_nodo($1);
            concat_call($$);
            adiciona_filho($$, $3);
    };

chamada_funcao: TK_IDENTIFICADOR '(' argumentos ')' {
            $$ = adiciona_nodo($1);
            concat_call($$);
            adiciona_filho($$, $3); }
             ;

argumentos: /* Vazio */ { $$ = NULL; }
         | expressao { $$ = $1; }
         | expressao ',' argumentos { $$ = adiciona_filho($1, $3); $$ = $1;}; 

expressao: prec7 { $$ = $1; }
         ;

prec7: prec6 { $$ = $1; }
    | prec7 TK_OC_OR prec6 { $$ = adiciona_nodo_by_label("|"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec6: prec5 { $$ = $1; }
    | prec6 TK_OC_AND prec5{ $$ = adiciona_nodo_by_label("&"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec5: prec4 { $$ = $1; }
    | prec5 TK_OC_EQ prec4 { $$ = adiciona_nodo_by_label("=="); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec5 TK_OC_NE prec4 { $$ = adiciona_nodo_by_label("!="); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec4: prec3 { $$ = $1; }
    | prec4 TK_OC_LE prec3 { $$ = adiciona_nodo_by_label("<="); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec4 TK_OC_GE prec3 { $$ = adiciona_nodo_by_label(">="); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec4 '<' prec3 { $$ = adiciona_nodo_by_label("<"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec4 '>' prec3 { $$ = adiciona_nodo_by_label(">"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec3: prec2 { $$ = $1; }
    | prec3 '+' prec2 { $$ = adiciona_nodo_by_label("+"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec3 '-' prec2 { $$ = adiciona_nodo_by_label("-"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec2: prec1 { $$ = $1; }
    | prec2 '*' prec1 { $$ = adiciona_nodo_by_label("*"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec2 '/' prec1 { $$ = adiciona_nodo_by_label("/"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    | prec2 '%' prec1 { $$ = adiciona_nodo_by_label("%"); adiciona_filho($$,$1); adiciona_filho($$,$3); }
    ;

prec1: '-' prec1 { $$ = adiciona_nodo_by_label("-"); adiciona_filho($$,$2);}
    | '!' prec1 { $$ = adiciona_nodo_by_label("!"); adiciona_filho($$,$2);}
    | primario { $$ = $1; }
    ;

primario: '(' expressao ')' { $$ = $2; }
        | TK_IDENTIFICADOR { $$ = adiciona_nodo($1); }
        | literais { $$ = $1; }
        | chamada_funcao{ $$ = $1; }
        ;

literais: TK_LIT_INT { $$ = adiciona_nodo($1); }
        | TK_LIT_FLOAT { $$ = adiciona_nodo($1); }
        | TK_LIT_FALSE { $$ = adiciona_nodo($1); }
        | TK_LIT_TRUE { $$ = adiciona_nodo($1); }
        ;

%%
