#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>

/*
Alunos:
Murilo Henrique Conde da Luz
Nathielly Neves de Castro
*/

/* ============================================================
   DEFINIÇÃO DA ESTRUTURA DO NÓ
   ============================================================ */

/**
 * Estrutura que representa um nó da árvore AVL.
 * Cada nó armazena:
 * - um valor inteiro
 * - ponteiros para os filhos esquerdo e direito
 * - a altura do nó (necessária para o balanceamento AVL)
 */
typedef struct no {
    int valor;
    struct no *esquerdo, *direito;
    int altura;
} No;

/* ============================================================
   FUNÇÕES AUXILIARES BÁSICAS
   ============================================================ */

/**
 * Cria e inicializa um novo nó da árvore AVL.
 * @param num Valor a ser armazenado no nó
 * @return Ponteiro para o novo nó criado
 */
No* novoNo(int num) {
    No *novo = malloc(sizeof(No));

    if (novo) {
        novo->valor = num;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0; // nó folha inicia com altura 0
    } else {
        wprintf(L"\nERRO ao alocar memória");
    }

    return novo;
}

/**
 * Retorna o maior valor entre dois inteiros.
 * Utilizado no cálculo da altura dos nós.
 */
int maior(int subArv1, int subArv2) {
    return (subArv1 > subArv2) ? subArv1 : subArv2;
}

/**
 * Retorna a altura de um nó.
 * @param no Ponteiro para o nó
 * @return Altura do nó ou -1 se for NULL
 */
int alturaNo(const No *no) {
    if (no == NULL) {
        return -1;
    } else {
        return no->altura;
    }
}

/**
 * Calcula o fator de balanceamento de um nó AVL.
 * fator = altura(esquerda) - altura(direita)
 */
int fatorBalanceamento(No *no) {
    if (no) {
        return alturaNo(no->esquerdo) - alturaNo(no->direito);
    } else {
        return 0;
    }
}

/* ============================================================
   ROTAÇÕES SIMPLES
   ============================================================ */

/**
 * Realiza rotação simples à esquerda.
 * Corrige desbalanceamento do tipo Direita-Direita (RR).
 */
No* rotacaoEsq(No *raiz) {
    No *u = raiz->direito;
    No *v = u->esquerdo;

    // Ajuste dos ponteiros
    u->esquerdo = raiz;
    raiz->direito = v;

    // Atualização das alturas
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;
    u->altura = maior(alturaNo(u->esquerdo), alturaNo(u->direito)) + 1;

    return u; // nova raiz da subárvore
}

/**
 * Realiza rotação simples à direita.
 * Corrige desbalanceamento do tipo Esquerda-Esquerda (LL).
 */
No* rotacaoDir(No *raiz) {
    No *u = raiz->esquerdo;
    No *v = u->direito;

    // Ajuste dos ponteiros
    u->direito = raiz;
    raiz->esquerdo = v;

    // Atualização das alturas
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;
    u->altura = maior(alturaNo(u->esquerdo), alturaNo(u->direito)) + 1;

    return u; // nova raiz da subárvore
}

/* ============================================================
   ROTAÇÕES DUPLAS
   ============================================================ */

/**
 * Rotação dupla Esquerda-Direita (LR).
 */
No* rotacaoEsqDir(No *raiz) {
    raiz->esquerdo = rotacaoEsq(raiz->esquerdo);
    return rotacaoDir(raiz);
}

/**
 * Rotação dupla Direita-Esquerda (RL).
 */
No* rotacaoDirEsq(No *raiz) {
    raiz->direito = rotacaoDir(raiz->direito);
    return rotacaoEsq(raiz);
}

/* ============================================================
   BALANCEAMENTO DA ÁRVORE AVL
   ============================================================ */

/**
 * Verifica o fator de balanceamento e aplica a rotação adequada.
 */
No* balancear(No *raiz) {
    int fatorB = fatorBalanceamento(raiz);

    // Caso Direita-Direita
    if (fatorB < -1 && fatorBalanceamento(raiz->direito) <= 0) {
        raiz = rotacaoEsq(raiz);
    }
    // Caso Esquerda-Esquerda
    else if (fatorB > 1 && fatorBalanceamento(raiz->esquerdo) >= 0) {
        raiz = rotacaoDir(raiz);
    }
    // Caso Esquerda-Direita
    else if (fatorB > 1 && fatorBalanceamento(raiz->esquerdo) < 0) {
        raiz = rotacaoEsqDir(raiz);
    }
    // Caso Direita-Esquerda
    else if (fatorB < -1 && fatorBalanceamento(raiz->direito) > 0) {
        raiz = rotacaoDirEsq(raiz);
    }

    return raiz;
}

/* ============================================================
   INSERÇÃO NA ÁRVORE AVL
   ============================================================ */

/**
 * Insere um valor na árvore AVL.
 * Após a inserção, a árvore é balanceada.
 */
No* insercao(No *raiz, int num) {
    if (raiz == NULL) {
        return novoNo(num);
    }

    if (num < raiz->valor) {
        raiz->esquerdo = insercao(raiz->esquerdo, num);
    } else if (num > raiz->valor) {
        raiz->direito = insercao(raiz->direito, num);
    } else {
        wprintf(L"A inserção não foi realizada, pois %d já existe\n", num);
        return raiz;
    }

    // Atualiza altura e balanceia
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;
    raiz = balancear(raiz);

    return raiz;
}

/* ============================================================
   REMOÇÃO NA ÁRVORE AVL
   ============================================================ */

/**
 * Remove um valor da árvore AVL.
 * Após a remoção, a árvore é balanceada.
 */
No* remover(No *raiz, int chave) {
    if (raiz == NULL) {
        wprintf(L"O valor não foi encontrado\n");
        return NULL;
    }

    if (chave < raiz->valor) {
        raiz->esquerdo = remover(raiz->esquerdo, chave);
    } else if (chave > raiz->valor) {
        raiz->direito = remover(raiz->direito, chave);
    } else {
        // Nó encontrado
        if (raiz->esquerdo == NULL && raiz->direito == NULL) {
            free(raiz);
            return NULL;
        }
        else if (raiz->esquerdo != NULL && raiz->direito != NULL) {
            // Nó com dois filhos: troca pelo predecessor
            No *aux = raiz->esquerdo;
            while (aux->direito != NULL) {
                aux = aux->direito;
            }
            raiz->valor = aux->valor;
            raiz->esquerdo = remover(raiz->esquerdo, aux->valor);
        }
        else {
            // Nó com apenas um filho
            No *aux = (raiz->esquerdo) ? raiz->esquerdo : raiz->direito;
            free(raiz);
            return aux;
        }
    }

    // Atualiza altura e balanceia
    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1;
    raiz = balancear(raiz);

    return raiz;
}

/* ============================================================
   FUNÇÕES DE PESQUISA
   ============================================================ */

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

/* ============================================================
   FUNÇÃO DE PERCURSO PRÉ-ORDEM
   ============================================================ */

/**
 * Realiza o percurso pré-ordem na árvore AVL.
 * @param raiz Ponteiro para a raiz da árvore
 */
void preOrdem(No *raiz){
    wprintf(L"%d ", raiz->valor);

    if(raiz->esquerdo != NULL){
        preOrdem(raiz->esquerdo);
    }
    if(raiz->direito != NULL){
        preOrdem(raiz->direito);
    }
}

/* ============================================================
   FUNÇÕES DE IMPRESSÃO
   ============================================================ */

/**
 * Calcula o comprimento (quantidade de caracteres) de um número inteiro.
 * É utilizado para alinhar corretamente os valores dos nós na impressão.
 * Considera números negativos e o valor zero.
 */
int comprimento(int numero) {
    const int sinal = numero < 0; // Verifica se o número é negativo
    int contador = 0;

    // Caso especial: o número zero possui comprimento 1
    if (numero == 0) {
        contador = 1;
    } else {
        // Conta quantos dígitos o número possui
        while (numero != 0) {
            numero = numero / 10; // Remove o último dígito
            contador++;           // Incrementa o contador de dígitos
        }
    }

    // Retorna o comprimento total (dígitos + possível sinal)
    return sinal + contador;
}

/**
 * Calcula o maior comprimento entre os valores armazenados nos nós da árvore.
 * Esse valor é utilizado para definir o espaçamento horizontal da impressão.
 * @param raiz Raiz da árvore
 * @return Maior quantidade de caracteres entre os valores dos nós
 */
int maiorComprimento(const No *raiz) {
    // Caso base: árvore vazia
    if (raiz == NULL) {
        return 0;
    }

    // Comprimento do valor armazenado no nó atual
    const int compr = comprimento(raiz->valor);

    // Comprimento máximo das subárvores esquerda e direita
    const int comprEsq = maiorComprimento(raiz->esquerdo);
    const int comprDir = maiorComprimento(raiz->direito);

    // Retorna o maior valor entre os três
    return (compr > comprEsq)
        ? (compr > comprDir ? compr : comprDir)
        : (comprEsq > comprDir ? comprEsq : comprDir);
}

/**
 * Centraliza uma string em um espaço total definido.
 * Utiliza caracteres de preenchimento à esquerda e à direita.
 * É usada para alinhar valores e símbolos na impressão da árvore.
 *
 * @param str String a ser centralizada
 * @param tam Tamanho real da string
 * @param total Espaço total disponível
 * @param ajustarEsq Indica como o arredondamento do espaçamento será feito
 * @param fillEsq Caractere de preenchimento à esquerda
 * @param fillDir Caractere de preenchimento à direita
 */
void center(const wchar_t *str, const int tam, const int total,
            const int ajustarEsq, const wchar_t *fillEsq, const wchar_t *fillDir) {

    // Calcula o espaço restante para preenchimento
    const double pad = (double) (total - tam) / 2.0;

    // Define quantos caracteres serão colocados à esquerda e à direita
    const int padEsq = (int) (ajustarEsq ? floor(pad) : ceil(pad));
    const int padDir = (int) (ajustarEsq ? ceil(pad) : floor(pad));

    // Preenchimento à esquerda
    for (int p = 0; p < padEsq; p++) wprintf(L"%ls", fillEsq);

    // Impressão da string centralizada
    wprintf(L"%ls", str);

    // Preenchimento à direita
    for (int p = 0; p < padDir; p++) wprintf(L"%ls", fillDir);
}

/**
 * Imprime um único nó da árvore, incluindo:
 * - o valor do nó
 * - os conectores gráficos (┌ ┐ ─)
 * A posição do nó é calculada com base na altura da árvore.
 *
 * @param no Nó a ser impresso
 * @param altura Altura atual da camada
 * @param compr Largura padrão para cada nó
 * @param ajustarEsq Indica se o alinhamento deve ser ajustado
 */
void imprimeNo(const No *no, const int altura, const int compr, const int ajustarEsq) {
    // Espaçamento necessário entre os nós
    const int halfPad = (1 << (altura - 1)) - 1;

    // Espaços iniciais antes do conector esquerdo
    for (int p = 0; p < halfPad * compr; p++) wprintf(L" ");

    // Conector para o filho esquerdo
    center(no && no->esquerdo ? L"┌" : L" ", 1, compr, 0, L" ", L"─");

    // Linha horizontal entre o nó e o conector
    for (int p = 0; p < halfPad * compr; p++)
        wprintf(no && no->esquerdo ? L"─" : L" ");

    if (no) {
        // Converte o valor do nó para string wide
        wchar_t wideBuffer[32];
        swprintf(wideBuffer, sizeof(wideBuffer) / sizeof(wchar_t), L"%d", no->valor);

        // Centraliza o valor do nó
        const int tam = comprimento(no->valor);
        center(wideBuffer, tam, compr, ajustarEsq,
               no->esquerdo ? L"─" : L" ",
               no->direito ? L"─" : L" ");
    } else {
        // Espaço vazio caso o nó não exista
        for (int p = 0; p < compr; p++) wprintf(L" ");
    }

    // Linha horizontal para o filho direito
    for (int p = 0; p < halfPad * compr; p++)
        wprintf(no && no->direito ? L"─" : L" ");

    // Conector para o filho direito
    center(no && no->direito ? L"┐" : L" ", 1, compr, 1, L"─", L" ");

    // Espaços finais
    for (int p = 0; p < halfPad * compr; p++) wprintf(L" ");
}

/**
 * Imprime uma camada (nível) inteira da árvore AVL.
 * Também prepara a próxima camada com os filhos dos nós atuais.
 *
 * @param camada Vetor de nós da camada atual
 * @param n Quantidade de nós na camada
 * @param altura Altura atual da árvore
 * @param compr Largura padrão dos nós
 * @return Vetor contendo a próxima camada
 */
No** imprimeCamada(No **camada, const int n, const int altura, const int compr) {
    if (!camada) {
        wprintf(L"Erro, camada não alocada\n");
        return camada;
    }

    // Aloca espaço para a próxima camada (dobro de nós)
    No** prox_camada = malloc(sizeof(No*) * n * 2);

    for (int i = 0; i < n; i++) {
        const No* no = camada[i];

        // Imprime o nó atual
        imprimeNo(no, altura, compr, i % 2);

        // Espaçamento entre os nós
        for (int p = 0; p < compr; p++) wprintf(L" ");

        // Preenche a próxima camada com os filhos
        prox_camada[i * 2]     = no ? no->esquerdo : NULL;
        prox_camada[i * 2 + 1] = no ? no->direito  : NULL;
    }

    // Quebra de linha ao final da camada
    wprintf(L"\n");

    // Libera a camada atual
    free(camada);

    return prox_camada;
}

/**
 * Imprime graficamente toda a árvore AVL, nível por nível.
 * Utiliza caracteres Unicode para representar a estrutura.
 *
 * @param raiz Raiz da árvore
 */
void imprimeArvore(No* raiz) {
    // Altura total da árvore
    const int altura = alturaNo(raiz) + 1;

    // Caso a árvore esteja vazia
    if (altura <= 0) {
        wprintf(L"A árvore está vazia.\n");
        return;
    }

    // Determina a largura necessária para impressão
    const int compr = maiorComprimento(raiz);

    // Inicializa a primeira camada com a raiz
    No** camada = malloc(sizeof(No*));
    camada[0] = raiz;

    int n = 1;

    // Imprime cada nível da árvore
    for (int a = altura; a > 0; a--) {
        camada = imprimeCamada(camada, n, a, compr);
        n *= 2;
    }

    // Libera a última camada
    free(camada);
}

int main(){
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
        wscanf(L"%d", &escolha);

        switch (escolha){
        case 0:
            wprintf(L"Finalizando...");
            break;
        
        case 1:
            wprintf(L"\nInforme o valor que deseja inserir:");
            wscanf(L"%d", &valor);
            raiz = insercao(raiz, valor);
            break;
        
        case 2:
            wprintf(L"\nInforme o valor que deseja remover:");
            wscanf(L"%d", &valor);
            raiz = remover(raiz, valor);
            break;

        case 3:
            wprintf(L"\nInforme o valor que deseja pesquisar:");
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
    
    return 0; 
}