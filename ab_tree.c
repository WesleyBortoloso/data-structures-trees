#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *esq, *dir;
} No;

No* criarNo(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }
    novo->valor = valor;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserir(No* raiz, int valor) {
    if (raiz == NULL) return criarNo(valor);
    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    return raiz;
}

void exibirEmOrdem(No* raiz) {
    if (raiz != NULL) {
        exibirEmOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        exibirEmOrdem(raiz->dir);
    }
}

void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    No* raiz = NULL;
    int elementos[] = {5, 12, 2, 7, 3, 1, 10, 8, 9, 4, 6, 11};
    int tamanho = sizeof(elementos) / sizeof(elementos[0]);

    for (int i = 0; i < tamanho; i++) {
        raiz = inserir(raiz, elementos[i]);
    }

    printf("Árvore Binária de Busca (Em-Ordem):\n");
    exibirEmOrdem(raiz);
    printf("\n");

    liberarArvore(raiz);
    return 0;
}
