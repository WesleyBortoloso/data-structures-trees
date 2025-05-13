#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

typedef struct Conexao {
    int destino;
    struct Conexao* prox;
} Conexao;

typedef struct Cidade {
    char nome[TAM_NOME];
    Conexao* lista_adj;
} Cidade;

typedef struct Grafo {
    Cidade* cidades;
    int qtd;
    int capacidade;
} Grafo;

Grafo* criarGrafo(int capacidade);
void liberarGrafo(Grafo* g);
int encontrarIndice(Grafo* g, const char* nome);
void adicionarCidade(Grafo* g, const char* nome);
void inserirConexao(Grafo* g, const char* origem, const char* destino);
void removerConexao(Grafo* g, const char* origem, const char* destino);
void buscarConexoes(Grafo* g, const char* origem);
void imprimirGrafo(Grafo* g);

Grafo* criarGrafo(int capacidade) {
    Grafo* g = malloc(sizeof(Grafo));
    g->cidades = malloc(sizeof(Cidade) * capacidade);
    g->qtd = 0;
    g->capacidade = capacidade;
    return g;
}

void liberarGrafo(Grafo* g) {
    for (int i = 0; i < g->qtd; i++) {
        Conexao* atual = g->cidades[i].lista_adj;
        while (atual) {
            Conexao* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->cidades);
    free(g);
}

int encontrarIndice(Grafo* g, const char* nome) {
    for (int i = 0; i < g->qtd; i++) {
        if (strcmp(g->cidades[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void adicionarCidade(Grafo* g, const char* nome) {
    if (encontrarIndice(g, nome) != -1) return;
    if (g->qtd >= g->capacidade) {
        g->capacidade *= 2;
        g->cidades = realloc(g->cidades, sizeof(Cidade) * g->capacidade);
    }
    strcpy(g->cidades[g->qtd].nome, nome);
    g->cidades[g->qtd].lista_adj = NULL;
    g->qtd++;
}

void inserirConexao(Grafo* g, const char* origem, const char* destino) {
    adicionarCidade(g, origem);
    adicionarCidade(g, destino);

    int i = encontrarIndice(g, origem);
    int j = encontrarIndice(g, destino);

    Conexao* atual = g->cidades[i].lista_adj;
    while (atual) {
        if (atual->destino == j) return;
        atual = atual->prox;
    }

    Conexao* nova = malloc(sizeof(Conexao));
    nova->destino = j;
    nova->prox = g->cidades[i].lista_adj;
    g->cidades[i].lista_adj = nova;
}

void removerConexao(Grafo* g, const char* origem, const char* destino) {
    int i = encontrarIndice(g, origem);
    int j = encontrarIndice(g, destino);
    if (i == -1 || j == -1) return;

    Conexao* atual = g->cidades[i].lista_adj;
    Conexao* ant = NULL;
    while (atual) {
        if (atual->destino == j) {
            if (ant) ant->prox = atual->prox;
            else g->cidades[i].lista_adj = atual->prox;
            free(atual);
            return;
        }
        ant = atual;
        atual = atual->prox;
    }
}

void buscarConexoes(Grafo* g, const char* origem) {
    int i = encontrarIndice(g, origem);
    if (i == -1) {
        printf("Cidade não encontrada.\n");
        return;
    }

    printf("Conexões partindo de %s:\n", origem);
    Conexao* atual = g->cidades[i].lista_adj;
    while (atual) {
        printf(" -> %s\n", g->cidades[atual->destino].nome);
        atual = atual->prox;
    }
}

void imprimirGrafo(Grafo* g) {
    printf("\nGrafo");
    for (int i = 0; i < g->qtd; i++) {
        printf("%s:", g->cidades[i].nome);
        Conexao* atual = g->cidades[i].lista_adj;
        while (atual) {
            printf(" -> %s", g->cidades[atual->destino].nome);
            atual = atual->prox;
        }
        printf("\n");
    }
}

int main() {
    Grafo* grafo = criarGrafo(5);
    int opcao;
    char origem[TAM_NOME], destino[TAM_NOME];

    do {
        printf("\n1 Inserir conexão\n2 Remover conexão\n3 Buscar conexões\n4 Imprimir grafo\n0 Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Cidade de origem: "); fgets(origem, TAM_NOME, stdin);
                printf("Cidade de destino: "); fgets(destino, TAM_NOME, stdin);
                origem[strcspn(origem, "\n")] = 0;
                destino[strcspn(destino, "\n")] = 0;
                inserirConexao(grafo, origem, destino);
                break;
            case 2:
                printf("Cidade de origem: "); fgets(origem, TAM_NOME, stdin);
                printf("Cidade de destino: "); fgets(destino, TAM_NOME, stdin);
                origem[strcspn(origem, "\n")] = 0;
                destino[strcspn(destino, "\n")] = 0;
                removerConexao(grafo, origem, destino);
                break;
            case 3:
                printf("Cidade de origem: "); fgets(origem, TAM_NOME, stdin);
                origem[strcspn(origem, "\n")] = 0;
                buscarConexoes(grafo, origem);
                break;
            case 4:
                imprimirGrafo(grafo);
                break;
        }
    } while (opcao != 0);

    liberarGrafo(grafo);
    return 0;
}
