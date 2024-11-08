#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 10004
#define INF INT_MAX

typedef struct {
    int cidade;
    int pedagio;
} Estrada;

typedef struct {
    int custo;
    int cidade;
    int paridade;
} No;

int comparador(const void *a, const void *b) {
    No *n1 = (No *)a;
    No *n2 = (No *)b;
    return n1->custo - n2->custo;
}

Estrada *adj[MAXN];
int grau[MAXN];

void inicializaGrafo(int n) {
    for (int i = 0; i < n; i++) {
        adj[i] = NULL;
        grau[i] = 0;
    }
}

void adicionaEstrada(int u, int v, int pedagio) {
    adj[u] = realloc(adj[u], (grau[u] + 1) * sizeof(Estrada));
    adj[u][grau[u]].cidade = v;
    adj[u][grau[u]].pedagio = pedagio;
    grau[u]++;

    adj[v] = realloc(adj[v], (grau[v] + 1) * sizeof(Estrada));
    adj[v][grau[v]].cidade = u;
    adj[v][grau[v]].pedagio = pedagio;
    grau[v]++;
}

int dijkstraModificado(int n) {
    int dist[MAXN][2];
    for (int i = 0; i < n; i++) {
        dist[i][0] = INF;
        dist[i][1] = INF;
    }
    dist[0][0] = 0;

    No heap[2 * MAXN];
    int heapSize = 0;
    heap[heapSize++] = (No){0, 0, 0}; // (custo, cidade, paridade)

    while (heapSize > 0) {
        No u = heap[0];
        heap[0] = heap[--heapSize];
        qsort(heap, heapSize, sizeof(No), comparador);

        if (u.custo > dist[u.cidade][u.paridade])
            continue;

        for (int i = 0; i < grau[u.cidade]; i++) {
            Estrada v = adj[u.cidade][i];
            int novoCusto = u.custo + v.pedagio;
            int novaParidade = 1 - u.paridade;

            if (novoCusto < dist[v.cidade][novaParidade]) {
                dist[v.cidade][novaParidade] = novoCusto;
                heap[heapSize++] = (No){novoCusto, v.cidade, novaParidade};
                qsort(heap, heapSize, sizeof(No), comparador);
            }
        }
    }

    return dist[n - 1][0] == INF ? -1 : dist[n - 1][0];
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    inicializaGrafo(N);

    for (int i = 0; i < M; i++) {
        int c1, c2, g;
        scanf("%d %d %d", &c1, &c2, &g);
        adicionaEstrada(c1, c2, g);
    }

    int resultado = dijkstraModificado(N);
    printf("%d\n", resultado);

    for (int i = 0; i < N; i++) {
        free(adj[i]);
    }

    return 0;
}