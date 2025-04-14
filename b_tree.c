#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define T 3
#define MAX_CHAVES (2 * T - 1)
#define MIN_CHAVES (T - 1)
#define MAX_FILHOS (2 * T)

typedef struct {
    int codigo;
    char nome[100];
} Produto;

typedef struct NoB {
    int n;                     
    Produto chaves[MAX_CHAVES];
    struct NoB *filhos[MAX_FILHOS];
    int folha;           
} NoB;

typedef struct {
    NoB *raiz;
} ArvoreB;


ArvoreB* criar_arvore_b();
NoB* criar_no();
void liberar_arvore(NoB *no);

void inserir(ArvoreB *arvore, Produto produto);
void inserir_nao_cheio(NoB *no, Produto produto);
void dividir_filho(NoB *pai, int i, NoB *filho);

void remover(ArvoreB *arvore, int codigo);
void remover_no(NoB *no, int codigo);
int encontrar_indice_chave(NoB *no, int codigo);
Produto encontrar_predecessor(NoB *no);
Produto encontrar_sucessor(NoB *no);
void remover_de_folha(NoB *no, int indice);
void remover_de_nao_folha(NoB *no, int indice);
void preencher(NoB *no, int indice);
void emprestar_do_anterior(NoB *no, int indice);
void emprestar_do_proximo(NoB *no, int indice);
void fundir(NoB *no, int indice);

Produto* buscar(NoB *raiz, int codigo);

void imprimir_arvore(NoB *no, int nivel);
void imprimir_arvore_formato(ArvoreB *arvore);

NoB* criar_no() {
    NoB *novoNo = (NoB*)malloc(sizeof(NoB));
    if (!novoNo) {
        perror("Falha ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    novoNo->n = 0;
    novoNo->folha = 1;
    for (int i = 0; i < MAX_FILHOS; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

ArvoreB* criar_arvore_b() {
    ArvoreB *novaArvore = (ArvoreB*)malloc(sizeof(ArvoreB));
     if (!novaArvore) {
        perror("Falha ao alocar memória para a árvore");
        exit(EXIT_FAILURE);
    }
    novaArvore->raiz = criar_no();
    return novaArvore;
}

void liberar_arvore(NoB *no) {
    if (no != NULL) {
        if (!no->folha) {
            for (int i = 0; i <= no->n; i++) {
                liberar_arvore(no->filhos[i]);
            }
        }
        free(no);
    }
}

void imprimir_arvore(NoB *no, int nivel) {
    if (no != NULL) {
        int i;
        for (i = 0; i < nivel; i++) printf("  ");
        printf("Nivel %d (%s): [", nivel, no->folha ? "Folha" : "Interno");
        for (i = 0; i < no->n; i++) {
            printf("%d", no->chaves[i].codigo);
            if (i < no->n - 1) printf(", ");
        }
        printf("] (n=%d)\n", no->n);

        if (!no->folha) {
            for (i = 0; i <= no->n; i++) {
                 imprimir_arvore(no->filhos[i], nivel + 1);
            }
        }
    }
}

void imprimir_arvore_formato(ArvoreB *arvore) {
    printf("\n--- Estrutura da Arvore B ---\n");
    if (arvore == NULL || arvore->raiz == NULL || arvore->raiz->n == 0) {
        printf("Árvore Vazia\n");
    } else {
        imprimir_arvore(arvore->raiz, 0);
    }
     printf("-----------------------------\n");
}


Produto* buscar(NoB *no, int codigo) {
    if (no == NULL) {
        return NULL;
    }

    int i = 0;
    while (i < no->n && codigo > no->chaves[i].codigo) {
        i++;
    }

    if (i < no->n && codigo == no->chaves[i].codigo) {
        return &(no->chaves[i]);
    }

    if (no->folha) {
        return NULL;
    }

    return buscar(no->filhos[i], codigo);
}

void dividir_filho(NoB *pai, int i, NoB *filho) {
    NoB *novoNo = criar_no();
    novoNo->folha = filho->folha;
    novoNo->n = MIN_CHAVES;

    for (int j = 0; j < MIN_CHAVES; j++) {
        novoNo->chaves[j] = filho->chaves[j + T];
    }

    if (!filho->folha) {
        for (int j = 0; j < T; j++) {
            novoNo->filhos[j] = filho->filhos[j + T];
        }
    }

    filho->n = MIN_CHAVES;

    for (int j = pai->n; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[i + 1] = novoNo;

    for (int j = pai->n - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    pai->chaves[i] = filho->chaves[T - 1];

    pai->n++;
}

void inserir_nao_cheio(NoB *no, Produto produto) {
    int i = no->n - 1;

    if (no->folha) {
        while (i >= 0 && produto.codigo < no->chaves[i].codigo) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = produto;
        no->n++;
    }
    else {
        while (i >= 0 && produto.codigo < no->chaves[i].codigo) {
            i--;
        }
        i++;

        if (no->filhos[i]->n == MAX_CHAVES) {
            dividir_filho(no, i, no->filhos[i]);
            if (produto.codigo > no->chaves[i].codigo) {
                i++;
            }
        }
        inserir_nao_cheio(no->filhos[i], produto);
    }
}

void inserir(ArvoreB *arvore, Produto produto) {
    NoB *raiz = arvore->raiz;

    if (raiz->n == MAX_CHAVES) {
        NoB *novaRaiz = criar_no();
        novaRaiz->folha = 0;
        novaRaiz->filhos[0] = raiz;
        arvore->raiz = novaRaiz;

        dividir_filho(novaRaiz, 0, raiz);

        inserir_nao_cheio(novaRaiz, produto);
    }
    else {
        inserir_nao_cheio(raiz, produto);
    }
}


int encontrar_indice_chave(NoB *no, int codigo) {
    int indice = 0;
    while (indice < no->n && no->chaves[indice].codigo < codigo) {
        ++indice;
    }
    return indice;
}

void remover_de_folha(NoB *no, int indice) {
    for (int i = indice + 1; i < no->n; ++i) {
        no->chaves[i - 1] = no->chaves[i];
    }
    no->n--;
}

Produto encontrar_predecessor(NoB *no) {
    while (!no->folha) {
        no = no->filhos[no->n];
    }
    return no->chaves[no->n - 1];
}

Produto encontrar_sucessor(NoB *no) {
    while (!no->folha) {
        no = no->filhos[0];
    }
    return no->chaves[0];
}

void remover_de_nao_folha(NoB *no, int indice) {
    int codigo = no->chaves[indice].codigo;
    NoB *filho_esq = no->filhos[indice];
    NoB *filho_dir = no->filhos[indice + 1];

    if (filho_esq->n >= T) {
        Produto pred = encontrar_predecessor(filho_esq);
        no->chaves[indice] = pred; 
        remover_no(filho_esq, pred.codigo);
    }
    else if (filho_dir->n >= T) {
        Produto succ = encontrar_sucessor(filho_dir);
        no->chaves[indice] = succ;
        remover_no(filho_dir, succ.codigo);
    }
    else {
        fundir(no, indice);
        remover_no(filho_esq, codigo);
    }
}

void emprestar_do_anterior(NoB *no, int indice) {
    NoB *filho = no->filhos[indice];
    NoB *irmao = no->filhos[indice - 1];

    for (int i = filho->n - 1; i >= 0; --i) {
        filho->chaves[i + 1] = filho->chaves[i];
    }
    if (!filho->folha) {
        for (int i = filho->n; i >= 0; --i) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = no->chaves[indice - 1];

    if (!filho->folha) {
        filho->filhos[0] = irmao->filhos[irmao->n];
    }

    no->chaves[indice - 1] = irmao->chaves[irmao->n - 1];

    filho->n++;
    irmao->n--;
}

void emprestar_do_proximo(NoB *no, int indice) {
    NoB *filho = no->filhos[indice];
    NoB *irmao = no->filhos[indice + 1];

    filho->chaves[filho->n] = no->chaves[indice];

    if (!filho->folha) {
        filho->filhos[filho->n + 1] = irmao->filhos[0];
    }

    no->chaves[indice] = irmao->chaves[0];

    for (int i = 1; i < irmao->n; ++i) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }
    if (!irmao->folha) {
        for (int i = 1; i <= irmao->n; ++i) {
            irmao->filhos[i - 1] = irmao->filhos[i];
        }
    }

    filho->n++;
    irmao->n--;
}

void fundir(NoB *no, int indice) {
    NoB *filho_esq = no->filhos[indice];
    NoB *filho_dir = no->filhos[indice + 1];

    filho_esq->chaves[T - 1] = no->chaves[indice];

    for (int i = 0; i < filho_dir->n; ++i) {
        filho_esq->chaves[i + T] = filho_dir->chaves[i];
    }

    if (!filho_esq->folha) {
        for (int i = 0; i <= filho_dir->n; ++i) {
            filho_esq->filhos[i + T] = filho_dir->filhos[i];
        }
    }

    for (int i = indice + 1; i < no->n; ++i) {
        no->chaves[i - 1] = no->chaves[i];
    }
    for (int i = indice + 2; i <= no->n; ++i) {
        no->filhos[i - 1] = no->filhos[i];
    }

    filho_esq->n += filho_dir->n + 1;
    no->n--;

    free(filho_dir);
}


void preencher(NoB *no, int indice) {
    if (indice != 0 && no->filhos[indice - 1]->n >= T) {
        emprestar_do_anterior(no, indice);
    }
    else if (indice != no->n && no->filhos[indice + 1]->n >= T) {
        emprestar_do_proximo(no, indice);
    }
    else {
        if (indice != no->n) {
            fundir(no, indice);
        } else {
            fundir(no, indice - 1);
        }
    }
}

void remover_no(NoB *no, int codigo) {
    int indice = encontrar_indice_chave(no, codigo);

    if (indice < no->n && no->chaves[indice].codigo == codigo) {
        if (no->folha) {
            remover_de_folha(no, indice);
        } else {
            remover_de_nao_folha(no, indice);
        }
    }
    else {
        if (no->folha) {
            printf("Erro: Produto com código %d não encontrado para remoção.\n", codigo);
            return;
        }

        int eh_ultimo_filho = (indice == no->n);

        if (no->filhos[indice]->n < T) {
            preencher(no, indice);
        }

        if (eh_ultimo_filho && indice > no->n) {
             remover_no(no->filhos[indice - 1], codigo);
        } else {
             remover_no(no->filhos[indice], codigo);
        }
    }
}

void remover(ArvoreB *arvore, int codigo) {
    if (!arvore || !arvore->raiz) {
        printf("Árvore vazia, nada a remover.\n");
        return;
    }

    remover_no(arvore->raiz, codigo);

    if (arvore->raiz->n == 0) {
        NoB *raiz_antiga = arvore->raiz;
        if (arvore->raiz->folha) {
             printf("Árvore ficou vazia após remoção.\n");
        } else {
            arvore->raiz = arvore->raiz->filhos[0];
            free(raiz_antiga);
             printf("Raiz atualizada após remoção.\n");
        }
    }
     printf("Remoção do código %d concluída (se existia).\n", codigo);
}


int main() {
    ArvoreB *arvore = criar_arvore_b();
    int opcao, codigo;
    Produto p;
    Produto *produto_encontrado;

    do {
        printf("\n--- Arvore B de Produtos ---\n");
        printf("1. Inserir Produto\n");
        printf("2. Remover Produto\n");
        printf("3. Buscar Produto\n");
        printf("4. Imprimir Arvore\n");
        printf("5. Inserir dados de teste\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o código do produto: ");
                scanf("%d", &p.codigo);
                getchar();
                printf("Digite o nome do produto: ");
                fgets(p.nome, sizeof(p.nome), stdin);
                p.nome[strcspn(p.nome, "\n")] = 0;
                inserir(arvore, p);
                printf("Produto inserido.\n");
                break;
            case 2:
                printf("Digite o código do produto a remover: ");
                scanf("%d", &codigo);
                getchar();
                produto_encontrado = buscar(arvore->raiz, codigo);
                if(produto_encontrado != NULL){
                    remover(arvore, codigo);
                } else {
                     printf("Produto com código %d não encontrado na árvore.\n", codigo);
                }

                break;
            case 3:
                printf("Digite o código do produto a buscar: ");
                scanf("%d", &codigo);
                getchar();
                produto_encontrado = buscar(arvore->raiz, codigo);
                if (produto_encontrado != NULL) {
                    printf("Produto encontrado:\n");
                    printf("  Código: %d\n", produto_encontrado->codigo);
                    printf("  Nome: %s\n", produto_encontrado->nome);
                } else {
                    printf("Produto com código %d não encontrado.\n", codigo);
                }
                break;
            case 4:
                imprimir_arvore_formato(arvore);
                break;
            case 5: {
                printf("Inserindo dados de teste...\n");

                Produto dados_teste[] = {
                    {50, "Laranja"}, {25, "Banana"}, {75, "Maca"}, {10, "Uva"},
                    {30, "Pera"},   {60, "Melao"},  {80, "Kiwi"}, {5, "Limao"},
                    {15, "Manga"},  {28, "Caqui"},  {35, "Coco"},  {55, "Abacate"},
                    {65, "Ameixa"}, {78, "Cereja"}, {85, "Figo"},  {90, "Morango"},
                    {40, "Goiaba"}, {45, "Jaca"},   {58, "Nectarina"}, {70, "Pessego"},
                    {20, "Amora"}, {95, "Framboesa"}, {18, "Acerola"}, {88, "Carambola"},
                    {52, "Maracuja"}, {33, "Pitanga"}, {68, "Tangerina"}, {42, "Jabuticaba"}
                };
                int num_testes = sizeof(dados_teste) / sizeof(dados_teste[0]);

                int inseridos_sucesso = 0;
                int ja_existiam = 0;

                for (int i = 0; i < num_testes; i++) {
                    if(buscar(arvore->raiz, dados_teste[i].codigo) == NULL) {
                        inserir(arvore, dados_teste[i]);
                        printf("  -> Inserido: Cod=%d, Nome=%s\n", dados_teste[i].codigo, dados_teste[i].nome);
                        inseridos_sucesso++;
                    } else {
                         printf("  -> Já existe: Cod=%d (%s)\n", dados_teste[i].codigo, dados_teste[i].nome);
                         ja_existiam++;
                    }
                }

                printf("\nInserção de teste concluída:\n");
                printf("  %d produtos inseridos com sucesso.\n", inseridos_sucesso);
                printf("  %d produtos já existiam na árvore.\n", ja_existiam);

                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    liberar_arvore(arvore->raiz);
    free(arvore);

    return 0;
}