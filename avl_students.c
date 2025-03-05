#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aluno {
    int matricula;
    char nome[100];
    struct Aluno* esquerda;
    struct Aluno* direita;
    int altura;
} Aluno;

int altura(Aluno* node) {
    if (node == NULL)
        return 0;
    return node->altura;
}

int fatorBalanceamento(Aluno* node) {
    if (node == NULL)
        return 0;
    return altura(node->esquerda) - altura(node->direita);
}

Aluno* minimo(Aluno* node) {
    Aluno* atual = node;
    while (atual && atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

Aluno* rotacionarDireita(Aluno* y) {
    Aluno* x = y->esquerda;
    Aluno* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));
    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));

    return x;
}

Aluno* rotacionarEsquerda(Aluno* x) {
    Aluno* y = x->direita;
    Aluno* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = 1 + (altura(x->esquerda) > altura(x->direita) ? altura(x->esquerda) : altura(x->direita));
    y->altura = 1 + (altura(y->esquerda) > altura(y->direita) ? altura(y->esquerda) : altura(y->direita));

    return y;
}

Aluno* rotacionarDireitaEsquerda(Aluno* node) {
    node->direita = rotacionarDireita(node->direita);
    return rotacionarEsquerda(node);
}

Aluno* rotacionarEsquerdaDireita(Aluno* node) {
    node->esquerda = rotacionarEsquerda(node->esquerda);
    return rotacionarDireita(node);
}

Aluno* inserirAluno(Aluno* node, int matricula, const char* nome) {
    if (node == NULL) {
        Aluno* novoAluno = (Aluno*)malloc(sizeof(Aluno));
        novoAluno->matricula = matricula;
        strcpy(novoAluno->nome, nome);
        novoAluno->esquerda = novoAluno->direita = NULL;
        novoAluno->altura = 1;
        return novoAluno;
    }

    if (matricula < node->matricula) {
        node->esquerda = inserirAluno(node->esquerda, matricula, nome);
    } else if (matricula > node->matricula) {
        node->direita = inserirAluno(node->direita, matricula, nome);
    } else {
        return node;
    }

    node->altura = 1 + (altura(node->esquerda) > altura(node->direita) ? altura(node->esquerda) : altura(node->direita));

    int balanceamento = fatorBalanceamento(node);

    if (balanceamento > 1 && matricula < node->esquerda->matricula)
        return rotacionarDireita(node);

    if (balanceamento < -1 && matricula > node->direita->matricula)
        return rotacionarEsquerda(node);

    if (balanceamento > 1 && matricula > node->esquerda->matricula)
        return rotacionarEsquerdaDireita(node);

    if (balanceamento < -1 && matricula < node->direita->matricula)
        return rotacionarDireitaEsquerda(node);

    return node;
}

Aluno* buscarAluno(Aluno* root, int matricula) {
    if (root == NULL || root->matricula == matricula)
        return root;

    if (matricula < root->matricula)
        return buscarAluno(root->esquerda, matricula);

    return buscarAluno(root->direita, matricula);
}

Aluno* removerAluno(Aluno* root, int matricula) {
    if (root == NULL)
        return root;

    if (matricula < root->matricula)
        root->esquerda = removerAluno(root->esquerda, matricula);
    else if (matricula > root->matricula)
        root->direita = removerAluno(root->direita, matricula);
    else {
        if (root->esquerda == NULL || root->direita == NULL) {
            Aluno* temp = root->esquerda ? root->esquerda : root->direita;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Aluno* temp = minimo(root->direita);
            root->matricula = temp->matricula;
            strcpy(root->nome, temp->nome);
            root->direita = removerAluno(root->direita, temp->matricula);
        }
    }

    if (root == NULL)
        return root;

    root->altura = 1 + (altura(root->esquerda) > altura(root->direita) ? altura(root->esquerda) : altura(root->direita));

    int balanceamento = fatorBalanceamento(root);

    if (balanceamento > 1 && fatorBalanceamento(root->esquerda) >= 0)
        return rotacionarDireita(root);

    if (balanceamento > 1 && fatorBalanceamento(root->esquerda) < 0)
        return rotacionarEsquerdaDireita(root);

    if (balanceamento < -1 && fatorBalanceamento(root->direita) <= 0)
        return rotacionarEsquerda(root);

    if (balanceamento < -1 && fatorBalanceamento(root->direita) > 0)
        return rotacionarDireitaEsquerda(root);

    return root;
}

void imprimirArvore(Aluno* root) {
    if (root != NULL) {
        imprimirArvore(root->esquerda);
        printf("Matrícula: %d, Nome: %s\n", root->matricula, root->nome);
        imprimirArvore(root->direita);
    }
}

int main() {
    Aluno* root = NULL;

    root = inserirAluno(root, 12345, "Alice");
    root = inserirAluno(root, 67890, "Bob");
    root = inserirAluno(root, 54321, "Charlie");
    root = inserirAluno(root, 98765, "David");

    printf("Árvore AVL:\n");
    imprimirArvore(root);

    int matriculaBusca = 12345;
    Aluno* alunoEncontrado = buscarAluno(root, matriculaBusca);
    if (alunoEncontrado != NULL)
        printf("\nAluno encontrado: Matrícula: %d, Nome: %s\n", alunoEncontrado->matricula, alunoEncontrado->nome);
    else
        printf("\nAluno não encontrado.\n");

    root = removerAluno(root, 67890);
    printf("\nÁrvore AVL após remoção:\n");
    imprimirArvore(root);

    return 0;
}
