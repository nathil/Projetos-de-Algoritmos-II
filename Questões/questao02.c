#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define UNIT 2
#define STR(x) #x
#define XSTR(s) STR(s)

#define RED "\033[0;31m"
#define RESET "\033[0m"

void spaces(const int num) {
    for (int i = 0; i < num; i++) {
        printf(" ");
    }
}

typedef struct no {
    int valor;
    struct no *esquerdo, *direito;
    short cor; // 1 para vermelho e 0 para preto
    int altura;
} No;

No* novoNo(const int valor) {
    No* no = malloc(sizeof(No));

    if (no) {
        no->valor = valor;
        no->esquerdo = NULL;
        no->direito = NULL;
        no->cor = 1;
        no->altura = 0;
    } else {
        printf("ERRO: não foi possível alocar memória para a criação de um novo nó.\n");
    }

    return no;
}

void freeArvore(No *raiz) {
    if (raiz == NULL) return;

    freeArvore(raiz->esquerdo);
    freeArvore(raiz->direito);
    free(raiz);
}

void imprimeArvore(No *raiz) {
    if (raiz == NULL) {
        printf("A árvore está vazia.\n");
        return;
    }

    const int altura = raiz->altura;

    No **nivel_arr = malloc(sizeof(No*));
    No **prox_nivel_arr = NULL;

    nivel_arr[0] = raiz;

    for (int nivel = 0; nivel < altura; nivel++) {
        const int itens_nivel = (int) pow(2, nivel);
        prox_nivel_arr = malloc(sizeof(No*) * itens_nivel * 2);

        spaces(((int) pow(2, altura - (nivel + 1)) - 1) * UNIT); // Padding

        for (int i = 0; i < itens_nivel; i++) {
            const No* no = nivel_arr[i];

            if (no) {
                if (no->cor) {
                    printf(RED);
                }
                printf("%" XSTR(UNIT) "d" RESET, no->valor);
            } else {
                spaces(UNIT);
            }

            spaces(((int) pow(2, altura - nivel) - 1) * UNIT); // Padding

            prox_nivel_arr[i * 2] = no ? no->esquerdo : NULL;
            prox_nivel_arr[i * 2 + 1] = no ? no->direito : NULL;
        }

        printf("\n");
        free(nivel_arr);
        nivel_arr = prox_nivel_arr;
        prox_nivel_arr = NULL;
    }

    free(nivel_arr);
}

int main() {
    No* raiz = novoNo(4);
    raiz->cor = 0;
    raiz->esquerdo = novoNo(2);
    raiz->esquerdo->esquerdo = novoNo(1);
    raiz->esquerdo->direito = novoNo(3);
    raiz->direito = novoNo(6);
    raiz->direito->esquerdo = novoNo(5);
    raiz->direito->direito = novoNo(7);

    raiz->altura = 3;

    imprimeArvore(raiz);

    freeArvore(raiz);
    return 0;
}