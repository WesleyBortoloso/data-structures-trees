#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_CIDADES 100

typedef struct Conexao {
    int destino;
    struct Conexao* prox;
} Conexao;

typedef struct Cidade {
    char nome[MAX_NOME];
    Conexao* adj;
} Cidade;

typedef struct Grafo {
    Cidade* cidades;
    int qtd;
    int capacidade;
} Grafo;

// Funções
Grafo* criarGrafo(int capacidade);
void liberarGrafo(Grafo* g);
void adicionarCidade(Grafo* g, const char* nome);
void inserirConexao(Grafo* g, const char* origem, const char* destino);
void removerConexao(Grafo* g, const char* origem, const char* destino);
void imprimirGrafo(Grafo* g);
void dfs(Grafo* g, const char* inicio);
void bfs(Grafo* g, const char* inicio);

int encontrarIndice(Grafo* g, const char* nome);
void dfsRec(Grafo* g, int atual, int* visitado);

// Implementações

Grafo* criarGrafo(int capacidade) {
    Grafo* g = malloc(sizeof(Grafo));
    g->cidades = malloc(sizeof(Cidade) * capacidade);
    g->qtd = 0;
    g->capacidade = capacidade;
    return g;
}

void liberarGrafo(Grafo* g) {
    for (int i = 0; i < g->qtd; i++) {
        Conexao* atual = g->cidades[i].adj;
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
        if (strcmp(g->cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void adicionarCidade(Grafo* g, const char* nome) {
    if (encontrarIndice(g, nome) != -1) return;
    if (g->qtd >= g->capacidade) return;

    strcpy(g->cidades[g->qtd].nome, nome);
    g->cidades[g->qtd].adj = NULL;
    g->qtd++;
}

void inserirConexao(Grafo* g, const char* origem, const char* destino) {
    adicionarCidade(g, origem);
    adicionarCidade(g, destino);

    int iOrigem = encontrarIndice(g, origem);
    int iDestino = encontrarIndice(g, destino);

    Conexao* nova = malloc(sizeof(Conexao));
    nova->destino = iDestino;
    nova->prox = g->cidades[iOrigem].adj;
    g->cidades[iOrigem].adj = nova;
}

void removerConexao(Grafo* g, const char* origem, const char* destino) {
    int iOrigem = encontrarIndice(g, origem);
    int iDestino = encontrarIndice(g, destino);

    if (iOrigem == -1 || iDestino == -1) return;

    Conexao* atual = g->cidades[iOrigem].adj;
    Conexao* anterior = NULL;

    while (atual) {
        if (atual->destino == iDestino) {
            if (anterior) anterior->prox = atual->prox;
            else g->cidades[iOrigem].adj = atual->prox;

            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

void imprimirGrafo(Grafo* g) {
    printf("\n--- Grafo Atual ---\n");
    for (int i = 0; i < g->qtd; i++) {
        printf("%s -> ", g->cidades[i].nome);
        Conexao* atual = g->cidades[i].adj;
        while (atual) {
            printf("%s ", g->cidades[atual->destino].nome);
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("-------------------\n");
}

void dfsRec(Grafo* g, int atual, int* visitado) {
    visitado[atual] = 1;
    printf("%s ", g->cidades[atual].nome);

    Conexao* adj = g->cidades[atual].adj;
    while (adj) {
        if (!visitado[adj->destino])
            dfsRec(g, adj->destino, visitado);
        adj = adj->prox;
    }
}

void dfs(Grafo* g, const char* inicio) {
    int idx = encontrarIndice(g, inicio);
    if (idx == -1) {
        printf("Cidade não encontrada!\n");
        return;
    }

    int* visitado = calloc(g->qtd, sizeof(int));
    printf("DFS a partir de %s: ", inicio);
    dfsRec(g, idx, visitado);
    printf("\n");
    free(visitado);
}

void bfs(Grafo* g, const char* inicio) {
    int idx = encontrarIndice(g, inicio);
    if (idx == -1) {
        printf("Cidade não encontrada!\n");
        return;
    }

    int* visitado = calloc(g->qtd, sizeof(int));
    int* fila = malloc(sizeof(int) * g->qtd);
    int frente = 0, tras = 0;

    fila[tras++] = idx;
    visitado[idx] = 1;

    printf("BFS a partir de %s: ", inicio);
    while (frente < tras) {
        int atual = fila[frente++];
        printf("%s ", g->cidades[atual].nome);

        Conexao* adj = g->cidades[atual].adj;
        while (adj) {
            if (!visitado[adj->destino]) {
                fila[tras++] = adj->destino;
                visitado[adj->destino] = 1;
            }
            adj = adj->prox;
        }
    }

    printf("\n");
    free(visitado);
    free(fila);
}

void menu() {
    printf("\n===== MENU =====\n");
    printf("1. Inserir conexão\n");
    printf("2. Remover conexão\n");
    printf("3. Imprimir grafo\n");
    printf("4. Executar DFS\n");
    printf("5. Executar BFS\n");
    printf("0. Sair\n");
    printf("================\n");
    printf("Escolha uma opção: ");
}

int main() {
    Grafo* g = criarGrafo(MAX_CIDADES);
    int opcao;
    char origem[MAX_NOME], destino[MAX_NOME];

    do {
        menu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Cidade de origem: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';

                printf("Cidade de destino: ");
                fgets(destino, MAX_NOME, stdin);
                destino[strcspn(destino, "\n")] = '\0';

                inserirConexao(g, origem, destino);
                break;
            case 2:
                printf("Cidade de origem: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';

                printf("Cidade de destino: ");
                fgets(destino, MAX_NOME, stdin);
                destino[strcspn(destino, "\n")] = '\0';

                removerConexao(g, origem, destino);
                break;
            case 3:
                imprimirGrafo(g);
                break;
            case 4:
                printf("Cidade inicial para DFS: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';
                dfs(g, origem);
                break;
            case 5:
                printf("Cidade inicial para BFS: ");
                fgets(origem, MAX_NOME, stdin);
                origem[strcspn(origem, "\n")] = '\0';
                bfs(g, origem);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    liberarGrafo(g);
    return 0;
}
