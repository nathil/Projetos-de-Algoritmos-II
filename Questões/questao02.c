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
 * @param exibirMensagem Indica se deve exibir mensagens durante a busca
 * @return O nó com o valor correspondente ou NULL, caso ele não esteja presente na árvore
 */
No* pesquisaNo(No *raiz, const int valor, const int exibirMensagem) {
    // Caso base - valor não encontrado
    if (raiz == NULL) return NULL;

    if (exibirMensagem) {
        wprintf(L"Verificando nó com valor %d...\n", raiz->valor);
    }

    // Caso base - valor encontrado
    if (raiz->valor == valor) return raiz;

    // Caso recursivo - continuar pesquisa para a direita ou esquerda, dependendo do valor do nó
    return pesquisaNo(valor < raiz->valor ? raiz->esquerdo : raiz->direito, valor, exibirMensagem);
}

/**
 * Retorna o menor nó de uma subárvore
 * @param no Raiz da subárvore
 * @return Nó com o menor valor
 */
No* minimo(No *no) {
    while (no && no->esquerdo) {
        no = no->esquerdo;
    }
    return no;
}

/**
 * Substitui um nó por outro na árvore
 * @param raiz Raiz da árvore
 * @param u Nó que será substituído
 * @param v Nó substituto
 * @return Nova raiz da árvore
 */
No* transplantar(No *raiz, No *u, No *v) {
    if (u->pai == NULL) {
        raiz = v;
    } else if (u == u->pai->esquerdo) {
        u->pai->esquerdo = v;
    } else {
        u->pai->direito = v;
    }

    if (v != NULL) {
        v->pai = u->pai;
    }

    return raiz;
}

/**
 * Ajusta a árvore rubro-negra após remoção
 * @param raiz Raiz da árvore
 * @param x Nó que pode violar as propriedades rubro-negras
 * @return Raiz ajustada
 */
No* remocaoAjuste(No *raiz, No *x) {
    while (x != raiz && (x == NULL || x->cor == PRETO)) {
        No *pai = x ? x->pai : NULL;

        if (x == pai->esquerdo) {
            No *irmao = pai->direito;

            if (irmao && irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                pai->cor = VERMELHO;
                raiz = rotacaoEsquerda(pai);
                irmao = pai->direito;
            }

            if ((!irmao->esquerdo || irmao->esquerdo->cor == PRETO) &&
                (!irmao->direito || irmao->direito->cor == PRETO)) {
                irmao->cor = VERMELHO;
                x = pai;
            } else {
                if (!irmao->direito || irmao->direito->cor == PRETO) {
                    if (irmao->esquerdo) irmao->esquerdo->cor = PRETO;
                    irmao->cor = VERMELHO;
                    raiz = rotacaoDireita(irmao);
                    irmao = pai->direito;
                }

                irmao->cor = pai->cor;
                pai->cor = PRETO;
                if (irmao->direito) irmao->direito->cor = PRETO;
                raiz = rotacaoEsquerda(pai);
                x = raiz;
            }
        } else {
            No *irmao = pai->esquerdo;

            if (irmao && irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                pai->cor = VERMELHO;
                raiz = rotacaoDireita(pai);
                irmao = pai->esquerdo;
            }

            if ((!irmao->direito || irmao->direito->cor == PRETO) &&
                (!irmao->esquerdo || irmao->esquerdo->cor == PRETO)) {
                irmao->cor = VERMELHO;
                x = pai;
            } else {
                if (!irmao->esquerdo || irmao->esquerdo->cor == PRETO) {
                    if (irmao->direito) irmao->direito->cor = PRETO;
                    irmao->cor = VERMELHO;
                    raiz = rotacaoEsquerda(irmao);
                    irmao = pai->esquerdo;
                }

                irmao->cor = pai->cor;
                pai->cor = PRETO;
                if (irmao->esquerdo) irmao->esquerdo->cor = PRETO;
                raiz = rotacaoDireita(pai);
                x = raiz;
            }
        }
    }

    if (x) x->cor = PRETO;
    return raiz;
}

/**
 * Remove um nó da árvore Rubro-Negra
 * @param raiz Raiz da árvore
 * @param valor Valor a ser removido
 * @return Nova raiz da árvore
 */
No* removeNoRN(No *raiz, const int valor) {
    No *z = pesquisaNo(raiz, valor, 0);

    if (z == NULL) {
        wprintf(L"Valor não encontrado na árvore.\n");
        return raiz;
    }

    No *y = z;
    No *x = NULL;
    short corOriginal = y->cor;

    if (z->esquerdo == NULL) {
        x = z->direito;
        raiz = transplantar(raiz, z, z->direito);
    } else if (z->direito == NULL) {
        x = z->esquerdo;
        raiz = transplantar(raiz, z, z->esquerdo);
    } else {
        y = minimo(z->direito);
        corOriginal = y->cor;
        x = y->direito;

        if (y->pai == z) {
            if (x) x->pai = y;
        } else {
            raiz = transplantar(raiz, y, y->direito);
            y->direito = z->direito;
            y->direito->pai = y;
        }

        raiz = transplantar(raiz, z, y);
        y->esquerdo = z->esquerdo;
        y->esquerdo->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (corOriginal == PRETO) {
        raiz = remocaoAjuste(raiz, x);
    }

    return raiz;
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
    // Se a árvore não tem nós, então a sua altura é 0
    if (raiz == NULL) {
        return 0;
    }

    // Calcula a altura da subarvore da esquerda da raiz
    const int alturaEsquerdo = alturaNo(raiz->esquerdo);

    // Calcula a altura da subarvore da esquerda da raiz
    const int alturaDireita = alturaNo(raiz->direito);

    // A altura da árvore atual é a altura da sua maior subarvore + 1
    return (alturaDireita > alturaEsquerdo ? alturaDireita : alturaEsquerdo) + 1;
}

int comprimento(int numero) {
    const int sinal = numero < 0;
    int contador = 0;

    if (numero == 0) {
        contador = 1;
    } else {
        // Loop enquanto o número for diferente de zero
        while (numero != 0) {
            numero = numero / 10; // Remove o último dígito
            contador++;           // Incrementa o contador
        }
    }

    return sinal + contador;
}

int maiorComprimento(const No *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    const int compr = comprimento(raiz->valor);
    const int comprEsq = maiorComprimento(raiz->esquerdo);
    const int comprDir = maiorComprimento(raiz->direito);

    return (compr > comprEsq) ? (compr > comprDir ? compr : comprDir) : (comprEsq > comprDir ? comprEsq : comprDir);
}

void center(const wchar_t *str, const int tam, const int total, const int ajustarEsq, const wchar_t *fillEsq, const wchar_t *fillDir) {
    const double pad = (double) (total- tam) / 2.;

    const int padEsq = (int) (ajustarEsq ? floor(pad) : ceil(pad));
    const int padDir = (int) (ajustarEsq ? ceil(pad) : floor(pad));

    for (int p = 0; p < padEsq; p++) wprintf(L"%ls", fillEsq);
    wprintf(L"%ls", str);
    for (int p = 0; p < padDir; p++) wprintf(L"%ls", fillDir);
}

void imprimeNo(const No *no, const int altura, const int compr, const int ajustarEsq) {
    const int halfPad = (1 << (altura-1)) - 1;

    for (int p = 0; p < halfPad * compr; p++) wprintf(L" ");
    center(no && no->esquerdo ? L"┌" : L" ", 1, compr, 0, L" ", L"─");
    for (int p = 0; p < halfPad * compr; p++) wprintf(no && no->esquerdo ? L"─" : L" ");

    if (no) {
        wchar_t wideBuffer[32];
        swprintf(wideBuffer, sizeof(wideBuffer) / sizeof(wchar_t), no->cor == VERMELHO ? TEXT_RED L"%d" TEXT_RESET : L"%d", no->valor);

        const int tam = comprimento(no->valor);
        center(wideBuffer, tam, compr, ajustarEsq, no->esquerdo ? L"─" : L" ", no->direito ? L"─" : L" ");
    } else {
        for (int p = 0; p < compr; p++) wprintf(L" ");
    }

    for (int p = 0; p < halfPad * compr; p++) wprintf(no && no->direito ? L"─" : L" ");
    center(no && no->direito ? L"┐" : L" ", 1, compr, 1, L"─", L" ");
    for (int p = 0; p < halfPad * compr; p++) wprintf(L" ");
}

No** imprimeCamada(No **camada, const int n, const int altura, const int compr) {
    if (!camada) {
        wprintf(L"Erro, camada não alocada");
        return camada;
    }

    No** prox_camada = malloc(sizeof(No*) * n * 2);

    for (int i = 0; i < n; i++) {
        const No* no = camada[i];

        imprimeNo(no, altura, compr, i % 2);
        for (int p = 0; p < compr; p++) wprintf(L" ");

        prox_camada[i * 2] = no ? no->esquerdo : NULL;
        prox_camada[i * 2 + 1] = no ? no->direito : NULL;
    }

    wprintf(L"\n");
    free(camada);
    return prox_camada;
}

void imprimeArvore(No* raiz) {
    const int altura = alturaNo(raiz);
    if (altura <= 0) {
        wprintf(L"A árvore está vazia.\n");
        return;
    }

    const int compr = maiorComprimento(raiz);
    No** camada = malloc(sizeof(No*));
    camada[0] = raiz;

    int n = 1;

    for (int a = altura; a > 0; a--) {
        camada = imprimeCamada(camada, n, a, compr);
        n *= 2;
    }
    free(camada);
}

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

    do{
        wprintf(L"\n0 - Sair\n1 - Inserir\n2 - Remover\n3 - Pesquisar\n4 - Imprimir\n5 - Pré-ordem\n");
        wprintf(L"Escolha uma opção: ");
        wscanf(L"%d", &escolha);

        switch (escolha){
            case 0:
                wprintf(L"Finalizando...");
                break;

            case 1:
                wprintf(L"\nInforme o valor que deseja inserir: ");
                wscanf(L"%d", &valor);
                raiz = inserirNoRN(raiz, valor);
                break;

            case 2:
                wprintf(L"\nInforme o valor que deseja remover: ");
                wscanf(L"%d", &valor);
                raiz = removeNoRN(raiz, valor);
                break;

            case 3:
                wprintf(L"\nInforme o valor que deseja pesquisar: ");
                wscanf(L"%d", &valor);
                const No* resultado = pesquisaNo(raiz, valor, 1);
                if (resultado) {
                    wprintf(L"Valor %d encontrado na árvore.\n", valor);
                } else {
                    wprintf(L"Valor %d não encontrado na árvore.\n", valor);
                }
                break;

            case 4:
                imprimeArvore(raiz);
                break;

            case 5:
                preOrdem(raiz);
                break;

            default:
                wprintf(L"\nOpcao invalida!!!!");
        }

    }while (escolha != 0);

    freeArvore(raiz);
    return 0;
}
