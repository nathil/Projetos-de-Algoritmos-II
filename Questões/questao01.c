#include <stdio.h>
#include <stdlib.h>

/* Alunos:  
Murilo Henrique Conde da Luz 
Nathielly Neves de Castro */


 /* funções essenciais */
  
typedef struct no{
    /* Nó para a árvore avl*/
    int valor; 
    struct no *esquerdo, *direito;
    int altura; 
}No;

No* novoNo(int num){
    /*Função que cria um novo nó*/
    No *novo = malloc(sizeof(No));
    
    if(novo){
        novo->valor = num;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    } else {
        printf("\nERRO");

    }
    return novo; 
}

int maior(int subArv1, int subArv2){
    /*Calcula qual é a subárvore que tem a maior altura*/
    return (subArv1 > subArv2) ? subArv1:subArv2; 
}

int alturaNo(const No *no){
    /*Retorna o valor da altura de um nó*/
    if(no == NULL){
        return -1; 
    } else {
        return no->altura;
    }
}

int fatorBalanceamento(No *no){
    if(no){
        return (alturaNo(no->esquerdo) - alturaNo(no->direito)); 
    } else {
        return 0; 
    }
}

/* rotações simples */

No* rotacaoEsq(No *raiz){
    No *u, *v; 

    u = raiz->direito; 
    v = u->esquerdo;
    
    u->esquerdo = raiz; 
    raiz->direito = v;     

    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1; 
    u->altura = maior(alturaNo(u->esquerdo), alturaNo(u->direito)) + 1; 

    return u; 
}

No* rotacaoDir(No *raiz){
    No *u, *v; 

    u = raiz->esquerdo; 
    v = u->direito; 

    u->direito = raiz; 
    raiz->esquerdo = v; 

    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1; 
    u->altura = maior(alturaNo(u->esquerdo), alturaNo(u->direito)) + 1;  

    return u;   
}

/* rotações duplas */


No* rotacaoEsqDir(No *raiz){
    raiz->esquerdo = rotacaoEsq(raiz->esquerdo); 
    return rotacaoDir(raiz); 
}

No* rotacaoDirEsq(No *raiz){
    raiz->direito = rotacaoDir(raiz->direito); 
    return rotacaoEsq(raiz); 
}



/* Balanceamento */
No* balancear(No *raiz){
    int fatorB = fatorBalanceamento(raiz); 

    if(fatorB < -1 && fatorBalanceamento(raiz->direito) <= 0){ //rotação à esquerda
        raiz = rotacaoEsq(raiz); 
    } else if(fatorB > 1 && fatorBalanceamento(raiz->esquerdo) >= 0){ //rotação à direita
        raiz = rotacaoDir(raiz); 
    } else if(fatorB > 1 && fatorBalanceamento(raiz->esquerdo) < 0 ){ //rotação duplai à esquerda
        raiz = rotacaoEsqDir(raiz); 
    } else if(fatorB < -1 && fatorBalanceamento(raiz->direito) > 0){ //rotação dupla à direita
        raiz = rotacaoDirEsq(raiz); 
    }

    return raiz; 
}

/* Inserção */

No* insercao(No *raiz, int num){
    if(raiz == NULL){
        return novoNo(num); 
    } else {
        if(num < raiz->valor){
            raiz->esquerdo = insercao(raiz->esquerdo, num); 
        } else if(num > raiz->valor){
            raiz->direito = insercao(raiz->direito, num);
        } else {
            printf("A insercao nao foi realizada, pois %d já existe", num); 
        }
    }

    raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1; 
    raiz = balancear(raiz); 

    return raiz; 
}


/* Remoção */
No* remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("O valor nao foi encontrado"); 
    } else {
        if(raiz->valor == chave){
            if(raiz->esquerdo == NULL && raiz->direito == NULL){
                free(raiz);
                printf("Elemento removido: %d! \n", chave); 
                return NULL;
            } else { //removendo nós que têm dois filhos
                if(raiz->esquerdo != NULL && raiz->direito != NULL){
                    No *aux = raiz->esquerdo; 
                    
                    while (aux->direito != NULL){
                        aux = aux->direito;
                    }
                    raiz->valor = aux->valor; 
                    aux->valor = chave; 
                    printf("Elemento trocado: %d! \n", chave); 
                    raiz->esquerdo = remover(raiz->esquerdo, chave); 
                    return raiz; 
                } else {
                    No *aux;
                    
                    if(raiz->esquerdo != NULL){
                        aux = raiz->esquerdo; 
                    } else {
                        aux = raiz->direito;
                    }
                    free(raiz); 
                    printf("Elemento com 1 filho removido: %d! \n", chave); 
                    return aux; 
                }
            }
        } else {
            if(chave < raiz->valor){ //remover nós com apenas 1 filho
                raiz->esquerdo = remover(raiz->esquerdo, chave);
            } else {
                raiz->direito = remover(raiz->direito, chave); 
            }
        }
        
        raiz->altura = maior(alturaNo(raiz->esquerdo), alturaNo(raiz->direito)) + 1; 
        raiz = balancear(raiz); 
        
        return raiz; 
    }
}

/*mostrando a árvore*/
void imprimir(No *raiz, int nivel){
    if(raiz){
        imprimir(raiz->direito, nivel + 1);
        printf("\n\n");

        for(int i=0; i<nivel; i++){
            printf("\t");
        }

        printf("%d", raiz->valor);
        imprimir(raiz->esquerdo, nivel + 1); 
    }
}

void preOrdem(No *raiz){
    printf("%d ", raiz->valor); 
    
    if(raiz->esquerdo != NULL){
        preOrdem(raiz->esquerdo); 
    } 
    if(raiz->direito != NULL){
        preOrdem(raiz->direito); 
    } 
}

int main(){
    int escolha, valor; 
    No *raiz = NULL; 

    do{
        printf("\n0 - Sair\n1 - Inserir\n2 - Remover\n3 - Imprimir\n4 - Mostrar em Pre-Ordem\n"); 
        scanf("%d", &escolha); 

        switch (escolha){
        case 0:
            printf("Finalizando...");
            break;
        
        case 1:
            printf("\nInforme o valor que deseja inserir:");    
            scanf("%d", &valor);
            raiz = insercao(raiz, valor);
            break;
        
        case 2:
            printf("\nInforme o valor que deseja remover:");    
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;
        
        case 3:
            imprimir(raiz, 1); 
            break;
        
        case 4: 
            preOrdem(raiz); 
            break;
        
        default:
            printf("\nOpcao invalida!!!!");  
        }

    }while (escolha != 0); 
    
    return 0; 
}