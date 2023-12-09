%{
    /*
    Integrantes do grupo V
    - Bruno Marques Bastos (314518)
    - Gustavo Lopes Noll (322864)
    */
    #include<stdio.h>
    #include<string.h>
    #include "ast.h"
    #include "parser.tab.h"
    #include "valor_lexico.h"
    #include "functions.h"
    #define YYSTYPE valorLexico
%}

%option noyywrap

identificador [a-z_][a-z_0-9]*

literal_inteiro [0-9]+
literal_float [0-9]*\.[0-9]+

%x comentario


%%

"int" { return TK_PR_INT;}
"float" { return TK_PR_FLOAT;}
"bool" { return TK_PR_BOOL;}

"if" {return TK_PR_IF;}
"else" {return TK_PR_ELSE;}
"while" {return TK_PR_WHILE;}
"return" {return TK_PR_RETURN;}

">=" { return TK_OC_GE;}
"<=" { return TK_OC_LE;}
"==" { return TK_OC_EQ;}
"!=" { return TK_OC_NE;}
"&" { return TK_OC_AND;}
"|" { return TK_OC_OR;}
"<"  { return (int) '<'; }
">"  { return (int) '>'; }

"-"  {  return (int) '-'; }
"!"  {  return (int) '!'; }
"*" {  return (int) '*'; }
"/"  {  return (int) '/'; }
"%"  {  return (int) '%'; }
"+" {  return (int) '+'; }

"{" { return (int) '{'; }
"}" { return (int) '}'; }
"(" { return (int) '('; }
")" { return (int) ')'; }
"=" { return (int) '='; }
","  { return (int) ','; }
";"  { return (int) ';'; }


true { yylval.valor_lexico = define_yyval(yytext, LITERAL, get_line_number()); return TK_LIT_TRUE;}
false { yylval.valor_lexico = define_yyval(yytext, LITERAL, get_line_number()); return TK_LIT_FALSE;}
{identificador} { yylval.valor_lexico = define_yyval(yytext, IDENTIFICADOR, get_line_number()); return TK_IDENTIFICADOR;}
{literal_float} { yylval.valor_lexico = define_yyval(yytext, LITERAL, get_line_number()); return TK_LIT_FLOAT;}
{literal_inteiro} { yylval.valor_lexico = define_yyval(yytext, LITERAL, get_line_number());  return TK_LIT_INT;}

\n  { increment_line_number(); }

"//".* {
  /* Ignoracomentário de linha */
}

"/*" {
    BEGIN(comentario);
}
<comentario>
{
    "*/" {BEGIN(INITIAL);}
    \n {increment_line_number();}
    . {} /* Ignora comentário de bloco */
}
[ \t\r\n] { } // Ignorar espaços e tabulações
. {return TK_ERRO;}

%%
