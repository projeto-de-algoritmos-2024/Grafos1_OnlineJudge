#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_N 10000

// Estrutura para armazenar uma estrada
typedef struct {
    int cidade;
    int distancia;
} Aresta;

// Estrutura para um nó da lista de adjacência
typedef struct No {
    Aresta aresta;
    struct No* prox;
} No;

No* adj[MAX_N + 1];
int ouro[MAX_N + 1];
int capacidade;
long long distancia_minima = 0;

// Função para adicionar uma estrada entre duas cidades
void adicionar_aresta(int origem, int destino, int distancia) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->aresta.cidade = destino;
    novoNo->aresta.distancia = distancia;
    novoNo->prox = adj[origem];
    adj[origem] = novoNo;
}

// Função DFS para calcular o menor caminho
int dfs(int cidade, int pai) {
    int peso_total = ouro[cidade];

    No* atual = adj[cidade];
    while (atual != NULL) {
        Aresta aresta = atual->aresta;
        if (aresta.cidade != pai) {
            int peso = dfs(aresta.cidade, cidade);
            int viagens = (peso + capacidade - 1) / capacidade;
            distancia_minima += 2LL * viagens * aresta.distancia;
            peso_total += peso;
        }
        atual = atual->prox;
    }

    return peso_total;
}

int main() {
    int N;
    scanf("%d %d", &N, &capacidade);

    // Lê o ouro de cada cidade
    for (int i = 1; i <= N; i++) {
        scanf("%d", &ouro[i]);
    }

    // Lê as estradas e cria a lista de adjacência
    for (int i = 0; i < N - 1; i++) {
        int A, B, D;
        scanf("%d %d %d", &A, &B, &D);
        adicionar_aresta(A, B, D);
        adicionar_aresta(B, A, D);
    }

    // Executa DFS a partir da capital (cidade 1)
    dfs(1, -1);

    // Resultado
    printf("%lld\n", distancia_minima);

    return 0;
}
