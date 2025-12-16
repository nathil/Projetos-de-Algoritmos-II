#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEXT_RED "\033[0;31m"
#define TEXT_RESET "\033[0m"

#define PRETO 0
#define VERMELHO 1

/**
 * Estrutura que representa um nó da árvore.
 */
typedef struct no {
    int valor;
    struct no *esquerdo, *direito, *pai;
    short cor; // 1 para vermelho e 0 para preto
} No;

/**
 * Cria uma nova instância da estrutura nó
 * @param valor Valor a ser armazenado no nó
 * @return Nó alocado e inicializado com o valor passado
 */
No* novoNo(const int valor) {
    No* no = malloc(sizeof(No));

    if (no) {
        no->valor = valor;
        no->esquerdo = NULL;
        no->direito = NULL;
        no->pai = NULL;
        no->cor = VERMELHO; // Todos os nós criados são inicialmente vermelhos
    } else {
        printf("ERRO: não foi possível alocar memória para a criação de um novo nó.\n");
    }

    return no;
}

/**
 * Realiza a rotação à esquerda de uma árvore
 * @param p Pivô da rotação, deve ter um filho à direita para realizar a rotação
 * @return Nova raiz, após realizada a rotação
 */
No* rotacaoEsquerda(No *p) {
    if (p == NULL || p->direito == NULL) {
        printf("ERRO: A árvore não cumpre os requisitos de uma rotação à esquerda.");
        return NULL;
    }

    // Seleção do nó direito ao pivô
    No *u = p->direito;

    // Mudança de pai da subarvore t2, para realização da rotação
    No *t2 = u->esquerdo;
    t2->pai = p;
    p->direito = t2;

    // Mudando raiz de p para u, finalizando a rotação
    u->pai = p->pai;
    u->esquerdo = p;
    p->pai = u;

    // Retornando nova raiz
    return u;
}

/**
 * Realiza a rotação à direita de uma árvore
 * @param p Pivô da rotação, deve ter um filho à esquerda para realizar a rotação
 * @return Nova raiz, após realizada a rotação
 */
No* rotacaoDireita(No *p) {
    if (p == NULL || p->esquerdo == NULL) {
        printf("ERRO: A árvore não cumpre os requisitos de uma rotação à direita.");
        return NULL;
    }

    // Seleção do nó direito ao pivô
    No *u = p->esquerdo;

    // Mudança de pai da subarvore t2, para realização da rotação
    No *t2 = u->esquerdo;
    t2->pai = p;
    p->esquerdo = t2;

    // Mudando raiz de p para u, finalizando a rotação
    u->pai = p->pai;
    u->direito = p;
    p->pai = u;

    // Retornando nova raiz
    return u;
}

/**
 * Realiza a rotação dupla à esquerda de uma árvore
 * @param p Pivô da rotação, deve conter um filho a direita, e esse deve ter um filho a esquerda para ser realizada a rotação dupla à esquerda
 * @return Nova raiz, após a rotação ser realizada
 */
No* rotacaoDuplaEsquerda(No *p) {
    // A rotação dupla à esquerda consiste em uma rotação à direita do filho direito do pivô
    p->direito = rotacaoDireita(p->direito);

    // Seguido de uma rotação à esquerda do pivô
    return rotacaoEsquerda(p);
}

/**
 * Realiza a rotação dupla à direita de uma árvore
 * @param p Pivô da rotação, deve conter um filho a esquerda, e esse deve ter um filho a direita para ser realizada a rotação dupla à direita
 * @return Nova raiz, após a rotação ser realizada
 */
No* rotacaoDuplaDireita(No *p) {
    // A rotação dupla à esquerda consiste em uma rotação à esquerda do filho esquerdo do pivô
    p->esquerdo = rotacaoEsquerda(p->esquerdo);

    // Seguido de uma rotação à direita do pivô
    return rotacaoDireita(p);
}

/**
 * Função de inserção de nó em árvore binária tradicional
 * @param raiz Nó inicial da árvore em que o elemento será inserido
 * @param novoNo Nó que será inserido na árvore
 * @return Nova raiz da árvore
 */
No* inserirNo(No* raiz, No* novoNo) {
    // Caso o nó não esteja inicializado corretamente, ele não é inserido na árvore
    if (!novoNo) {
        printf("ERRO: Novo nó inválido");
    }

    // Caso a árvore esteja vazia (raiz == NULL), o nó será a nova raiz;
    else if (raiz == NULL) {
        raiz = novoNo;
    }

    // Caso a árvore não esteja vazia, será procurado um nó com espaço para armazenar o nó inserido, seguindo as regras das árvores binárias
    else {
        novoNo->pai = raiz;

        // A inserção do nó é realizada de forma recursiva
        if (novoNo->valor < raiz->valor) {
            raiz->esquerdo = inserirNo(raiz->esquerdo, novoNo);
        } else {
            raiz->direito = inserirNo(raiz->direito, novoNo);
        }
    }

    // A árvore com a raiz atualizada (caso necessário), é retornada
    return raiz;
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

            raiz = insercaoAjuste(raiz, avo);
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

No* inserirNoRN(No *raiz, const int valor) {
    No* no = novoNo(valor);
    raiz = inserirNo(raiz, no);
    raiz = insercaoAjuste(raiz, no);

    return raiz;
}

/**
 * Busca um nó com valor correspondente na árvore
 * @param raiz Nó inicial da busca
 * @param valor Valor que será buscado na árvore
 * @return O nó com o valor correspondente ou NULL, caso ele não esteja presente na árvore
 */
No* pesquisaNo(No *raiz, const int valor) {
    // Caso base - valor não encontrado
    if (raiz == NULL) return NULL;

    // Caso base - valor encontrado
    if (raiz->valor == valor) return raiz;

    // Caso recursivo - continuar pesquisa para a direita ou esquerda, dependendo do valor do nó
    return pesquisaNo(valor < raiz->valor ? raiz->esquerdo : raiz->direito, valor);
}

void freeArvore(No *raiz) {
    if (raiz == NULL) return;

    freeArvore(raiz->esquerdo);
    freeArvore(raiz->direito);
    free(raiz);
}

/**
 * Calcula a altura da árvore
 * @param raiz Nó inicial para o cálculo da altura
 * @return Altura da árvore
 */
int alturaNo(const No *raiz) {
    // Calcula a altura da subarvore da esquerda da raiz, caso a subarvore esteja vazia (NULL), sua altura é 0
    const int alturaEsquerdo = raiz->esquerdo ? alturaNo(raiz->esquerdo) : 0;

    // Calcula a altura da subarvore da esquerda da raiz, caso a subarvore esteja vazia (NULL), sua altura é 0
    const int alturaDireita = raiz->direito ? alturaNo(raiz->direito) : 0;

    // A altura da árvore atual é a altura da sua maior subarvore + 1
    return (alturaDireita > alturaEsquerdo ? alturaDireita : alturaEsquerdo) + 1;
}

void imprimeArvore(No *raiz) {
    if (raiz == NULL) {
        printf("A árvore está vazia.\n");
        return;
    }

    const int altura = alturaNo(raiz);

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
                    printf(TEXT_RED);
                }
                printf("%" XSTR(UNIT) "d" TEXT_RESET, no->valor);
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
