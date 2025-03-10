#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int matricula;
    char nome[100];
    struct Node *esq;
    struct Node *dir;
    int altura;
} Node;

int altura(Node *n) {
    if (n == NULL)
        return 0;
    return n->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* novoNo(int matricula, char *nome) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->matricula = matricula;
    strcpy(node->nome, nome);
    node->esq = node->dir = NULL;
    node->altura = 1;
    return node;
}

Node* rotacaoDireita(Node *y) {
    Node *x = y->esq;
    Node *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

Node* rotacaoEsquerda(Node *x) {
    Node *y = x->dir;
    Node *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

int fatorBalanceamento(Node *n) {
    if (n == NULL)
        return 0;
    return altura(n->esq) - altura(n->dir);
}

Node* inserir(Node* node, int matricula, char *nome) {
    if (node == NULL)
        return novoNo(matricula, nome);

    if (matricula < node->matricula)
        node->esq = inserir(node->esq, matricula, nome);
    else if (matricula > node->matricula)
        node->dir = inserir(node->dir, matricula, nome);
    else
        return node;

    node->altura = 1 + max(altura(node->esq), altura(node->dir));

    int balance = fatorBalanceamento(node);

    if (balance > 1 && matricula < node->esq->matricula)
        return rotacaoDireita(node);

    if (balance < -1 && matricula > node->dir->matricula)
        return rotacaoEsquerda(node);

    if (balance > 1 && matricula > node->esq->matricula) {
        node->esq = rotacaoEsquerda(node->esq);
        return rotacaoDireita(node);
    }

    if (balance < -1 && matricula < node->dir->matricula) {
        node->dir = rotacaoDireita(node->dir);
        return rotacaoEsquerda(node);
    }

    return node;
}

Node* noMinimoValor(Node* node) {
    Node* atual = node;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

Node* remover(Node* root, int matricula) {
    if (root == NULL)
        return root;

    if (matricula < root->matricula)
        root->esq = remover(root->esq, matricula);
    else if (matricula > root->matricula)
        root->dir = remover(root->dir, matricula);
    else {
        if ((root->esq == NULL) || (root->dir == NULL)) {
            Node *temp = root->esq ? root->esq : root->dir;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            Node* temp = noMinimoValor(root->dir);
            root->matricula = temp->matricula;
            strcpy(root->nome, temp->nome);
            root->dir = remover(root->dir, temp->matricula);
        }
    }

    if (root == NULL)
        return root;

    root->altura = 1 + max(altura(root->esq), altura(root->dir));
    int balance = fatorBalanceamento(root);

    if (balance > 1 && fatorBalanceamento(root->esq) >= 0)
        return rotacaoDireita(root);

    if (balance > 1 && fatorBalanceamento(root->esq) < 0) {
        root->esq = rotacaoEsquerda(root->esq);
        return rotacaoDireita(root);
    }

    if (balance < -1 && fatorBalanceamento(root->dir) <= 0)
        return rotacaoEsquerda(root);

    if (balance < -1 && fatorBalanceamento(root->dir) > 0) {
        root->dir = rotacaoDireita(root->dir);
        return rotacaoEsquerda(root);
    }

    return root;
}

Node* buscar(Node* root, int matricula) {
    if (root == NULL || root->matricula == matricula)
        return root;
    if (matricula < root->matricula)
        return buscar(root->esq, matricula);
    return buscar(root->dir, matricula);
}

void imprimir(Node* root) {
    if (root != NULL) {
        imprimir(root->esq);
        printf("Matrícula: %d, Nome: %s\n", root->matricula, root->nome);
        imprimir(root->dir);
    }
}

void imprimirArvore(Node* root, int espaco) {
    if (root == NULL)
        return;
    espaco += 10;
    imprimirArvore(root->dir, espaco);
    for (int i = 10; i < espaco; i++)
        printf(" ");
    printf("[%d] %s\n", root->matricula, root->nome);
    imprimirArvore(root->esq, espaco);
}

int main() {
    Node *root = NULL;
    int opcao, matricula;
    char nome[100];

    do {
        printf("\nMenu:\n");
        printf("1. Inserir aluno\n");
        printf("2. Remover aluno\n");
        printf("3. Buscar aluno\n");
        printf("4. Listar alunos\n");
        printf("5. Visualizar árvore AVL\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a matrícula: ");
                scanf("%d", &matricula);
                printf("Digite o nome: ");
                scanf("%s", nome);
                root = inserir(root, matricula, nome);
                break;
            case 2:
                printf("Digite a matrícula a ser removida: ");
                scanf("%d", &matricula);
                root = remover(root, matricula);
                break;
            case 3:
                printf("Digite a matrícula para buscar: ");
                scanf("%d", &matricula);
                Node* aluno = buscar(root, matricula);
                if (aluno != NULL)
                    printf("Aluno encontrado - Matrícula: %d, Nome: %s\n", aluno->matricula, aluno->nome);
                else
                    printf("Aluno não encontrado.\n");
                break;
            case 4:
                printf("\nLista de alunos:\n");
                imprimir(root);
                break;
            case 5:
                printf("\nImprimir árvore:\n");
                imprimirArvore(root, 0);
                break;
        }
    } while (opcao != 6);

    return 0;
}
