#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define UNIT 2
#define STR(x) #x
#define XSTR(s) STR(s)

#define STRRED "\033[0;31m"
#define STRRESET "\033[0m"

#define PRETO 0
#define VERMELHO 1

void spaces(const int num) {
    for (int i = 0; i < num; i++) {
        printf(" ");
    }
}

typedef struct no {
    int valor;
    struct no *esquerdo, *direito, *pai;
    short cor; // 1 para vermelho e 0 para preto
    int altura;
} No;

No* novoNo(const int valor) {
    No* no = malloc(sizeof(No));

    if (no) {
        no->valor = valor;
        no->esquerdo = NULL;
        no->direito = NULL;
        no->pai = NULL;
        no->cor = VERMELHO;
        no->altura = 0;
    } else {
        printf("ERRO: não foi possível alocar memória para a criação de um novo nó.\n");
    }

    return no;
}

No* inserirNo(No* raiz, No* novoNo) {
    if (!novoNo) {
        printf("ERRO: Novo nó inválido");
        return raiz;
    }

    if (raiz == NULL) {
        raiz = novoNo;
    } else {
        novoNo->pai = raiz;

        if (novoNo->valor < raiz->valor) {
            raiz->esquerdo = inserirNo(raiz->esquerdo, novoNo);
        } else {
            raiz->direito = inserirNo(raiz->direito, novoNo);
        }
    }

    const int alturaEsquerda = raiz->esquerdo ? raiz->esquerdo->altura : 0;
    const int alturaDireita = raiz->direito ? raiz->direito->altura : 0;
    raiz->altura = (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita) + 1;

    return raiz;
}

No* rotacaoEsquerda(No *p) {
    if (p == NULL) {
        printf("ERRO: p não existe");
        return NULL;
    }

    No *u = p->direito;

    if (u == NULL) {
        printf("ERRO: u não existe");
        return NULL;
    }

    No *t1 = p->esquerdo;
    No *t2 = u->esquerdo;
    No *t3 = u->direito;

    u->pai = p->pai;
    p->pai = u;

    p->esquerdo = t1;
    p->direito = t2;
    u->esquerdo = p;
    u->direito = t3;

    return u;
}

No* rotacaoDireita(No *p) {
    if (p == NULL) {
        printf("ERRO: p não existe");
        return NULL;
    }

    No *u = p->esquerdo;

    if (u == NULL) {
        printf("ERRO: u não existe");
        return NULL;
    }

    No *t1 = u->esquerdo;
    No *t2 = u->direito;
    No *t3 = p->direito;

    u->pai = p->pai;
    p->pai = u;

    u->esquerdo = t1;
    u->direito = p;
    p->esquerdo = t2;
    p->direito = t3;

    return u;
}

No* rotacaoDuplaEsquerda(No *p) {
    p->direito = rotacaoDireita(p->direito);
    return rotacaoEsquerda(p);
}

No* rotacaoDuplaDireita(No *p) {
    p->esquerdo = rotacaoEsquerda(p->esquerdo);
    return rotacaoDireita(p);
}

No* insercaoAjuste(No *raiz, No *no) {
    if (!no) {
        printf("ERRO: nó não definido.\n");
    } else if (!no->pai) {
        no->cor = PRETO;
    } else if (no->pai->cor == VERMELHO) {
        No* pai = no->pai;
        No* avo = pai->pai;
        No* tio = avo->esquerdo == pai ? avo->direito : avo->esquerdo;
        No* bisavo = avo->pai;

        if (tio != NULL && tio->cor == VERMELHO) {
            pai->cor = PRETO;
            avo->cor = VERMELHO;
            tio->cor = PRETO;

            insercaoAjuste(raiz, avo);
        } else {
            No *novoAvo = NULL;
            if (avo->esquerdo == pai && pai->esquerdo == no) {
                novoAvo = rotacaoDireita(avo);
                pai->cor = PRETO;
                avo->cor = VERMELHO;
            } else if (avo->direito == pai && pai->direito == no) {
                novoAvo = rotacaoEsquerda(avo);
                pai->cor = PRETO;
                avo->cor = VERMELHO;
            } else if (avo->esquerdo == pai && pai->direito == no) {
                novoAvo = rotacaoDuplaDireita(avo);
                no->cor = PRETO;
                avo->cor = VERMELHO;
            } else {
                no->cor = PRETO;
                avo->cor = VERMELHO;
                novoAvo = rotacaoDuplaEsquerda(avo);
            }

            if (bisavo == NULL) {
                raiz = novoAvo;
            } else if (bisavo->esquerdo == avo) {
                bisavo->esquerdo = novoAvo;
            } else {
                bisavo->direito = novoAvo;
            }
        }
    }

    return raiz;
}

No* inserirNoRN(No* raiz, const int valor) {
    No* no = novoNo(valor);
    raiz = inserirNo(raiz, no);
    raiz = insercaoAjuste(raiz, no);

    return raiz;
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
                    printf(STRRED);
                }
                printf("%" XSTR(UNIT) "d" STRRESET, no->valor);
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
    No* raiz = NULL;

    raiz = inserirNoRN(raiz, 4);
    raiz = inserirNoRN(raiz, 1);
    raiz = inserirNoRN(raiz, 6);
    raiz = inserirNoRN(raiz, 0);
    raiz = inserirNoRN(raiz, -1);
    raiz = inserirNoRN(raiz, 3);
    raiz = inserirNoRN(raiz, 2);
    raiz = inserirNoRN(raiz, 5);

    imprimeArvore(raiz);
    freeArvore(raiz);
    return 0;
}
