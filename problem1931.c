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

Estrada *adj[MAXN];
int grau[MAXN];

void inicializaGrafo(int n) {
    for (int i = 0; i < n; i++) {
        adj[i] = NULL;
        grau[i] = 0;
    }
}

void adicionaEstrada(int u, int v, int pedagio) {
    Estrada *novoAdjU = realloc(adj[u], (grau[u] + 1) * sizeof(Estrada));
    if (novoAdjU == NULL) {
        fprintf(stderr, "Erro de alocação para adj[%d]\n", u);
        exit(EXIT_FAILURE);
    }
    adj[u] = novoAdjU;
    adj[u][grau[u]].cidade = v;
    adj[u][grau[u]].pedagio = pedagio;
    grau[u]++;

    Estrada *novoAdjV = realloc(adj[v], (grau[v] + 1) * sizeof(Estrada));
    if (novoAdjV == NULL) {
        fprintf(stderr, "Erro de alocação para adj[%d]\n", v);
        exit(EXIT_FAILURE);
    }
    adj[v] = novoAdjV;
    adj[v][grau[v]].cidade = u;
    adj[v][grau[v]].pedagio = pedagio;
    grau[v]++;
}

void swap(No *a, No *b) {
    No temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(No *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent].custo <= heap[index].custo) break;
        swap(&heap[parent], &heap[index]);
        index = parent;
    }
}

void heapifyDown(No *heap, int size, int index) {
    while (2 * index + 1 < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (heap[left].custo < heap[smallest].custo) smallest = left;
        if (right < size && heap[right].custo < heap[smallest].custo) smallest = right;

        if (smallest == index) break;
        swap(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

void push(No **heap, int *size, int *capacity, No value) {
    if (*size >= *capacity) {
        *capacity *= 2;
        *heap = realloc(*heap, *capacity * sizeof(No));
        if (*heap == NULL) {
            fprintf(stderr, "Erro de alocação de memória para o heap\n");
            exit(EXIT_FAILURE);
        }
    }
    (*heap)[(*size)++] = value;
    heapifyUp(*heap, *size - 1);
}

No pop(No *heap, int *size) {
    No root = heap[0];
    heap[0] = heap[--(*size)];
    heapifyDown(heap, *size, 0);
    return root;
}

int dijkstraModificado(int n) {
    int dist[MAXN][2];
    for (int i = 0; i < n; i++) {
        dist[i][0] = INF;
        dist[i][1] = INF;
    }
    dist[0][0] = 0;

    int heapCapacity = 2 * MAXN;
    No *heap = malloc(heapCapacity * sizeof(No));
    if (heap == NULL) {
        fprintf(stderr, "Erro de alocação de memória para o heap\n");
        exit(EXIT_FAILURE);
    }
    int heapSize = 0;
    push(&heap, &heapSize, &heapCapacity, (No){0, 0, 0}); 

    while (heapSize > 0) {
        No u = pop(heap, &heapSize);

        if (u.custo > dist[u.cidade][u.paridade])
            continue;

        for (int i = 0; i < grau[u.cidade]; i++) {
            Estrada v = adj[u.cidade][i];
            int novoCusto = u.custo + v.pedagio;
            int novaParidade = 1 - u.paridade;

            if (novoCusto < dist[v.cidade][novaParidade]) {
                dist[v.cidade][novaParidade] = novoCusto;
                push(&heap, &heapSize, &heapCapacity, (No){novoCusto, v.cidade, novaParidade});
            }
        }
    }

    free(heap);

    int resultado = dist[n - 1][0] == INF ? -1 : dist[n - 1][0];
    return resultado;
}

int main() {
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) {
        fprintf(stderr, "Erro na leitura da entrada.\n");
        return 1;
    }

    inicializaGrafo(N);

    for (int i = 0; i < M; i++) {
        int c1, c2, g;
        if (scanf("%d %d %d", &c1, &c2, &g) != 3) {
            fprintf(stderr, "Erro na leitura de estrada %d.\n", i + 1);
            return 1;
        }
        adicionaEstrada(c1 - 1, c2 - 1, g);
    }

    int resultado = dijkstraModificado(N);
    printf("%d\n", resultado);
    
    for (int i = 0; i < N; i++) {
        free(adj[i]);
    }

    return 0;
}
