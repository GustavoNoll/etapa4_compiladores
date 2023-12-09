/*
 * Integrantes do Grupo V
 * - Bruno Marques Bastos (314518)
 * - Gustavo Lopes Noll (322864)
*/

#pragma once
#include <stdio.h>
#include <string.h>
#include "valor_lexico.h"

typedef struct Nodo {
    meuValorLexico valor_lexico;
    struct Nodo *irmao;
    struct Nodo *filho;
} Nodo;

Nodo *adiciona_nodo(meuValorLexico valor_lexico);
Nodo *adiciona_nodo_by_label(char *label);
Nodo *adiciona_filho(Nodo *nodo, Nodo *filho);
void imprime_arvore(Nodo *nodo, int profundidade);
void _adiciona_ultimo_irmao(Nodo *nodo_irmao, Nodo *novo_irmao);
void libera(void *pai);
void _imprime_nodo(Nodo *nodo);
void _imprime_arestas(Nodo *nodo);
void exporta(void *arvore);
void concat_call(Nodo* nodo);
