#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char isbn[20];  
    struct No *esq, *dir;
} No;

No* criarNo(char *isbn);
No* inserir(No* raiz, char *isbn);
No* buscar(No* raiz, char *isbn);
No* remover(No* raiz, char *isbn);
No* menorNo(No* raiz);
void preOrdem(No* raiz);
void emOrdem(No* raiz);
void posOrdem(No* raiz);
void liberarArvore(No* raiz);
void exibirArvore(No* raiz, int nivel);

No* criarNo(char *isbn) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }
    strcpy(novo->isbn, isbn);
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserir(No* raiz, char *isbn) {
    if (raiz == NULL) return criarNo(isbn);
    if (strcmp(isbn, raiz->isbn) < 0)
        raiz->esq = inserir(raiz->esq, isbn);
    else if (strcmp(isbn, raiz->isbn) > 0)
        raiz->dir = inserir(raiz->dir, isbn);
    return raiz;
}

No* buscar(No* raiz, char *isbn) {
    if (raiz == NULL || strcmp(isbn, raiz->isbn) == 0)
        return raiz;
    if (strcmp(isbn, raiz->isbn) < 0)
        return buscar(raiz->esq, isbn);
    return buscar(raiz->dir, isbn);
}

No* menorNo(No* raiz) {
    while (raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

No* remover(No* raiz, char *isbn) {
    if (raiz == NULL) return NULL;

    if (strcmp(isbn, raiz->isbn) < 0)
        raiz->esq = remover(raiz->esq, isbn);
    else if (strcmp(isbn, raiz->isbn) > 0)
        raiz->dir = remover(raiz->dir, isbn);
    else {
        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        No* temp = menorNo(raiz->dir);
        strcpy(raiz->isbn, temp->isbn);
        raiz->dir = remover(raiz->dir, temp->isbn);
    }
    return raiz;
}

void preOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%s ", raiz->isbn);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%s ", raiz->isbn);
        emOrdem(raiz->dir);
    }
}

void posOrdem(No* raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%s ", raiz->isbn);
    }
}

void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

void exibirArvore(No* raiz, int nivel) {
    if (raiz == NULL)
        return;

    exibirArvore(raiz->dir, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("    ");  

    printf("|-- %s\n", raiz->isbn);

    exibirArvore(raiz->esq, nivel + 1);
}

int main() {
    No* raiz = NULL;
    int opcao;
    char isbn[20];

    do {
        printf("\nMenu:\n");
        printf("1. Inserir livro\n");
        printf("2. Buscar livro\n");
        printf("3. Remover livro\n");
        printf("4. Exibir árvore formatada\n");
        printf("5. Exibir percurso Pré-Ordem\n");
        printf("6. Exibir percurso Em-Ordem\n");
        printf("7. Exibir percurso Pós-Ordem\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o ISBN: ");
                fgets(isbn, 20, stdin);
                strtok(isbn, "\n");
                raiz = inserir(raiz, isbn);
                break;
            case 2:
                printf("Digite o ISBN para buscar: ");
                fgets(isbn, 20, stdin);
                strtok(isbn, "\n");
                if (buscar(raiz, isbn))
                    printf("Livro encontrado!\n");
                else
                    printf("Livro não encontrado.\n");
                break;
            case 3:
                printf("Digite o ISBN do livro a remover: ");
                fgets(isbn, 20, stdin);
                strtok(isbn, "\n");
                raiz = remover(raiz, isbn);
                printf("Livro removido, se existia.\n");
                break;
            case 4:
                printf("\nEstrutura da Árvore:\n");
                exibirArvore(raiz, 0);
                break;
            case 5:
                printf("\nPercurso Pré-Ordem: ");
                preOrdem(raiz);
                printf("\n");
                break;
            case 6:
                printf("\nPercurso Em-Ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;
            case 7:
                printf("\nPercurso Pós-Ordem: ");
                posOrdem(raiz);
                printf("\n");
                break;
            case 8:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 8);

    liberarArvore(raiz);
    return 0;
}
