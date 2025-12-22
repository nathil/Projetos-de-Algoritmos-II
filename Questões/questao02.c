#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define TEXT_RED L"\033[0;31m"
#define TEXT_RESET L"\033[0m"

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
        wprintf(L"ERRO: não foi possível alocar memória para a criação de um novo nó.\n");
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
        wprintf(L"ERRO: A árvore não cumpre os requisitos de uma rotação à esquerda.");
        return NULL;
    }

    // Seleção do nó direito ao pivô
    No *u = p->direito;

    // Mudança de pai da subarvore t2, para realização da rotação
    No *t2 = u->esquerdo;
    p->direito = t2;
    if (t2) {
        t2->pai = p;
    }

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
        wprintf(L"ERRO: A árvore não cumpre os requisitos de uma rotação à direita.");
        return NULL;
    }

    // Seleção do nó direito ao pivô
    No *u = p->esquerdo;

    // Mudança de pai da subarvore t2, para realização da rotação
    No *t2 = u->direito;
    p->esquerdo = t2;
    if (t2) {
        t2->pai = p;
    }


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
    if (novoNo == NULL) {
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

/**
 * Ajusta a árvore rubro-negra onde um nó foi inserido
 * @param raiz Origem da árvore onde o nó será ajustado
 * @param no Nó onde será iniciado os ajustes
 * @return Raiz da árvore ajustada seguindo as regras de inserção de árvores rubro-negra
 */
No* insercaoAjuste(No *raiz, No *no) {
    // Caso o nó não exista, mostra um erro e retorna a árvore
    if (!no) {
        wprintf(L"ERRO: nó não definido.\n");
    }
    // Caso o nó inserido seja a raiz, transforme o nó de vermelho para preto
    else if (!no->pai) {
        no->cor = PRETO;
    }
    // Casos onde o pai é vermelho
    else if (no->pai->cor == VERMELHO) {
        No* pai = no->pai;
        No* avo = pai->pai;
        No* tio = avo->esquerdo == pai ? avo->direito : avo->esquerdo;
        No* bisavo = avo->pai;

        // Caso o pai e o tio forem vermelhos, troca a cor do pai, tio e avô, e verifica se o avô precisa de ajuste
        if (tio != NULL && tio->cor == VERMELHO) {
            pai->cor = PRETO;
            avo->cor = VERMELHO;
            tio->cor = PRETO;

            raiz = insercaoAjuste(raiz, avo);
        }

        // Caso o pai seja vermelho, e o tio seja preto, aplicamos uma das rotações e mudamos a cor no novo avô e do irmão do nó inserido
        else {
            No *novoAvo = NULL;
            if (avo->esquerdo == pai && pai->esquerdo == no) {
                novoAvo = rotacaoDireita(avo);
            } else if (avo->direito == pai && pai->direito == no) {
                novoAvo = rotacaoEsquerda(avo);
            } else if (avo->esquerdo == pai && pai->direito == no) {
                novoAvo = rotacaoDuplaDireita(avo);
            } else {
                novoAvo = rotacaoDuplaEsquerda(avo);
            }

            novoAvo->cor = PRETO;
            avo->cor = VERMELHO;

            // Caso o avô seja a raiz, retorna o novo avô, senão, apenas modificamos o filho do avo
            novoAvo->pai = bisavo;
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

/**
 * Insere um valor na árvore Rubro-Negra
 * @param raiz A raiz da árvore onde será inserido o valor
 * @param valor Valor que será inserido na árvore
 * @return Raiz da árvore com o valor inserido
 */
No* inserirNoRN(No *raiz, const int valor) {
    // Para inserir o valor, criamos um nó vermelho
    No* no = novoNo(valor);

    // O inserimos na árvore utilizando os critérios de uma árvore binária de busca
    raiz = inserirNo(raiz, no);

    // E ajustamos o balanceamento utilizando os critérios da árvore rubro negra
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

No* sucessor(No *no) {
    if (!no) {
        wprintf(L"O nó não existe");
        return NULL;
    }

    if (!no->esquerdo && !no->direito) {
        return no;
    }

    if (no->esquerdo && !no->direito) {
        return no->esquerdo;
    }

    no = no->direito;
    while (no->esquerdo != NULL) {
        no = no->esquerdo;
    }
    return no;
}

No* removeNo(No *raiz, No *no) {
}

No* removeNoRN(No *raiz, const int valor) {
    No* no = pesquisaNo(raiz, valor);
}

/**
 * Libera os recursos de uma árvore binária
 * @param raiz Raiz da árvore que terá os seus nós liberados
 */
void freeArvore(No *raiz) {
    // Para a liberação a árvore é percorrida de forma recusiva em pós-ordem, liberando as subarvores da esquerda e da direita antes de liberar o nó atual
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

void imprimeArvoreCamadas(No **camada, const int altura) {
    const int nos = 1 << (altura - 1); // 2^(altura-1)
    const int padding = (1 << altura) - 1; // (2^altura) - 1
    No **prox_camada = malloc(sizeof(camada) * 2);

    for (int i = 0; i < nos; i++) {
        const No *no = camada[i];

        for (int p = 0; p < padding; p++) wprintf(L" ");

        if (no) {
            wprintf(no->cor == PRETO ? L"%d" : TEXT_RED "%d" TEXT_RESET, no->valor);
            prox_camada[i*2] = no->esquerdo;
            prox_camada[i*2 + 1] = no->direito;
        } else {
            wprintf(L" ");
            prox_camada[i*2] = NULL;
            prox_camada[i*2 + 1] = NULL;
        }

        for (int p = 0; p < padding; p++) wprintf(L" ");
        wprintf(L" ");
    }

    wprintf(L"\n");
    free(camada);

    if (altura > 1) {
        imprimeArvoreCamadas(prox_camada, altura-1);
    } else {
        free(prox_camada);
    }
}

void imprimeArvore(No *raiz) {
    const int altura = alturaNo(raiz);

    No **camada = malloc(sizeof(No*));
    camada[0] = raiz;

    wprintf(L"Altura: %d\n", altura);

    imprimeArvoreCamadas(camada, altura);
}

// /**
//  * Imprime a árvore no terminal
//  * @param raiz Raiz da árvore que será impressa
//  */
// void imprimeArvore(No *raiz) {
//     if (raiz == NULL) {
//         wprintf(L"A árvore está vazia.\n");
//         return;
//     }
//
//     const int altura = alturaNo(raiz);
//
//     No **nivel_arr = malloc(sizeof(No*));
//     No **prox_nivel_arr = NULL;
//
//     nivel_arr[0] = raiz;
//
//     for (int nivel = 0; nivel < altura; nivel++) {
//         const int itens_nivel = (int) pow(2, nivel);
//         prox_nivel_arr = malloc(sizeof(No*) * itens_nivel * 3);
//
//         const int startPad = ((int) pow(2, altura - (nivel + 2)) - 1) * 3;
//         for (int i = 0; i < startPad + 1; i++) {
//             wprintf(L" ");
//         }
//
//         for (int i = 0; i < itens_nivel; i++) {
//             const No* no = nivel_arr[i];
//
//             if (no) {
//                 if (no->esquerdo) {
//                     wprintf(L"┌");
//                     for (int j = 0; j < startPad + 1; j++) {
//                         wprintf(L"─");
//                     }
//                 } else {
//                     for (int j = 0; j < startPad + 2; j++) {
//                         wprintf(L"e");
//                     }
//                 }
//
//                 if (no->cor) {
//                     wprintf(L"" TEXT_RED);
//                 }
//
//                 const int digits = (no->valor == 0) ? 1 : (int) floor(log10(abs(no->valor))) + 1;
//                 const int front = (int) ceil((3. - digits) / 2.);
//                 const int back = (3 - digits) / 2;
//
//                 for (int j = 0; j < front; j++) {
//                     wprintf(L"f");
//                 }
//
//                 if (no->cor) {
//                     wprintf(TEXT_RED);
//                 }
//                 wprintf(L"%d" TEXT_RESET, no->valor);
//
//                 for (int j = 0; j < back; j++) {
//                     wprintf(L"b");
//                 }
//
//                 if (no->direito) {
//                     for (int j = 0; j < startPad + 1; j++) {
//                         wprintf(L"─");
//                     }
//                     wprintf(L"┐");
//                 } else {
//                     for (int j = 0; j < startPad + 2; j++) {
//                         wprintf(L"d");
//                     }
//                 }
//             } else {
//                 for (int j = 0; j < 2 * startPad + 3; j++) {
//                     wprintf(L"   ");
//                 }
//             }
//
//             const int middlePad = ((int) pow(2, altura - (nivel + 1)) - 1) * 3;
//             for (int j = 0; j < middlePad + 1; j++) {
//                 wprintf(L"m");
//             }
//
//             prox_nivel_arr[i * 2] = no ? no->esquerdo : NULL;
//             prox_nivel_arr[i * 2 + 1] = no ? no->direito : NULL;
//         }
//
//         wprintf(L"\n");
//         free(nivel_arr);
//         nivel_arr = prox_nivel_arr;
//         prox_nivel_arr = NULL;
//     }
//
//     free(nivel_arr);
// }

void preOrdem(const No *raiz){
    wprintf(L"%d ", raiz->valor);

    if(raiz->esquerdo != NULL){
        preOrdem(raiz->esquerdo);
    }
    if(raiz->direito != NULL){
        preOrdem(raiz->direito);
    }
}

int main() {
    // Set locale to support wide characters
    setlocale(LC_ALL, "");

#ifdef _WIN32
    // For Windows, specifically set the console output mode
    // _O_U16TEXT might need a #define _O_U16TEXT 0x20000 on some older compilers
    _setmode(_fileno(stdout), _O_U16TEXT);
#else
    // For POSIX systems, fwide(stdout, 1) can set the stream to wide orientation
    fwide(stdout, 1);
#endif

    int escolha, valor;
    No *raiz = NULL;
    raiz = inserirNoRN(raiz, 1);
    raiz = inserirNoRN(raiz, 2);
    raiz = inserirNoRN(raiz, 3);
    raiz = inserirNoRN(raiz, 4);
    raiz = inserirNoRN(raiz, 5);

    imprimeArvore(raiz);

    wprintf(L"%d", sucessor(raiz)->valor);

    wprintf(L"\n\n\n\n");
/*
    do{
        wprintf(L"\n0 - Sair\n1 - Inserir\n2 - Remover\n3 - Imprimir\n4 - Mostrar em Pre-Ordem\n");
        wprintf(L"Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha){
            case 0:
                wprintf(L"Finalizando...");
                break;

            case 1:
                wprintf(L"\nInforme o valor que deseja inserir:");
                scanf("%d", &valor);
                raiz = inserirNoRN(raiz, valor);
                break;

            case 2:
                wprintf(L"\nInforme o valor que deseja remover:");
                scanf("%d", &valor);
                raiz = removeNoRN(raiz, valor);
                break;

            case 3:
                imprimeArvore(raiz);
                break;

            case 4:
                preOrdem(raiz);
                break;

            default:
                wprintf(L"\nOpcao invalida!!!!");
        }

    }while (escolha != 0);
*/
    freeArvore(raiz);
    return 0;
}
