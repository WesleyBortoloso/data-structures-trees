#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1
#define BLACK 0

typedef struct Node {
    int id;
    char name[100];
    int color;
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
    Node *nil;
} RBTree;

Node* createNode(RBTree *tree, int id, char *name) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->id = id;
    strcpy(node->name, name);
    node->color = RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

RBTree* createTree() {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = (Node*)malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->root = tree->nil;
    return tree;
}

void leftRotate(RBTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTree *tree, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void insertFixup(RBTree *tree, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void insert(RBTree *tree, int id, char *name) {
    Node *z = createNode(tree, id, name);
    Node *y = tree->nil;
    Node *x = tree->root;
    while (x != tree->nil) {
        y = x;
        if (z->id < x->id)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == tree->nil)
        tree->root = z;
    else if (z->id < y->id)
        y->left = z;
    else
        y->right = z;
    insertFixup(tree, z);
}

Node* search(RBTree *tree, int id) {
    Node *current = tree->root;
    while (current != tree->nil && id != current->id) {
        if (id < current->id)
            current = current->left;
        else
            current = current->right;
    }
    return current;
}

void printTree(Node *node, Node *nil, int depth) {
    if (node != nil) {
        printTree(node->right, nil, depth + 1);
        for (int i = 0; i < depth; i++) printf("   ");
        printf("%d (%s)\n", node->id, node->color == RED ? "R" : "B");
        printTree(node->left, nil, depth + 1);
    }
}

int main() {
    RBTree *tree = createTree();
    int choice, id;
    char name[100];
    
    do {
        printf("\nMenu:\n");
        printf("1. Inserir funcionário\n");
        printf("2. Buscar funcionário\n");
        printf("3. Imprimir árvore\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Digite o ID: ");
                scanf("%d", &id);
                printf("Digite o nome: ");
                scanf("%s", name);
                insert(tree, id, name);
                break;
            case 2:
                printf("Digite o ID do funcionário a buscar: ");
                scanf("%d", &id);
                Node *result = search(tree, id);
                if (result != tree->nil)
                    printf("Funcionário encontrado: ID=%d, Nome=%s\n", result->id, result->name);
                else
                    printf("Funcionário não encontrado.\n");
                break;
            case 3:
                printTree(tree->root, tree->nil, 0);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (choice != 4);
    
    return 0;
}
