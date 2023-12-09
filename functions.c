/*
Integrantes do grupo V
- Bruno Marques Bastos (314518)
- Gustavo Lopes Noll (322864)
*/

#include <stdio.h>

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

