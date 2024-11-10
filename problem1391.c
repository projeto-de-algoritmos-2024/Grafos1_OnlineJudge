#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_N 500
#define MAX_M 10000
#define INF 1000000000

// Estrutura para representar uma aresta
typedef struct {
    int destino;
    int peso;
} Aresta;

// Estrutura para lista de adjacência
typedef struct No {
    Aresta aresta;
    struct No* prox;
} No;

// Estrutura para fila de prioridade
typedef struct {
    int vertice;
    int distancia;
} ElementoPQ;

// Variáveis globais
No* grafo[MAX_N];
No* grafo_reverso[MAX_N];
No* grafo_auxiliar[MAX_N];
int dist_s[MAX_N];    // Distâncias a partir de s
int dist_t[MAX_N];    // Distâncias até t
int visitado[MAX_N];
ElementoPQ pq[MAX_M];
int tam_pq;

void trocar(ElementoPQ* a, ElementoPQ* b) {
    ElementoPQ temp = *a;
    *a = *b;
    *b = temp;
}

void inserir_pq(int vertice, int distancia) {
    int i = tam_pq++;
    pq[i].vertice = vertice;
    pq[i].distancia = distancia;

    while (i > 0) {
        int pai = (i - 1) / 2;
        if (pq[pai].distancia <= pq[i].distancia) break;
        trocar(&pq[i], &pq[pai]);
        i = pai;
    }
}

ElementoPQ remover_pq() {
    ElementoPQ resultado = pq[0];
    pq[0] = pq[--tam_pq];
    int i = 0;

    while (1) {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < tam_pq && pq[esq].distancia < pq[menor].distancia)
            menor = esq;
        if (dir < tam_pq && pq[dir].distancia < pq[menor].distancia)
            menor = dir;

        if (menor == i) break;
        trocar(&pq[i], &pq[menor]);
        i = menor;
    }

    return resultado;
}

void adicionar_aresta(No** grafo, int origem, int destino, int peso) {
    No* novo = (No*)malloc(sizeof(No));
    novo->aresta.destino = destino;
    novo->aresta.peso = peso;
    novo->prox = grafo[origem];
    grafo[origem] = novo;
}

void dijkstra(No** grafo, int n, int inicio, int* dist) {
    tam_pq = 0;

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visitado[i] = 0;
    }

    dist[inicio] = 0;
    inserir_pq(inicio, 0);

    while (tam_pq > 0) {
        ElementoPQ atual = remover_pq();
        int v = atual.vertice;

        if (visitado[v]) continue;
        visitado[v] = 1;

        for (No* no = grafo[v]; no != NULL; no = no->prox) {
            int u = no->aresta.destino;
            int peso = no->aresta.peso;

            if (dist[u] > dist[v] + peso) {
                dist[u] = dist[v] + peso;
                inserir_pq(u, dist[u]);
            }
        }
    }
}

int resolver(int n, int m, int s, int t) {
    // Limpa os grafos
    for (int i = 0; i < n; i++) {
        grafo[i] = NULL;
        grafo_reverso[i] = NULL;
        grafo_auxiliar[i] = NULL;
    }

    // Lê e constrói os grafos
    for (int i = 0; i < m; i++) {
        int u, v, p;
        scanf("%d %d %d", &u, &v, &p);
        adicionar_aresta(grafo, u, v, p);
        adicionar_aresta(grafo_reverso, v, u, p);
    }

    // Passo A: Dijkstra de s para todos os vértices
    dijkstra(grafo, n, s, dist_s);

    // Se não existe caminho até t, retorna -1
    if (dist_s[t] == INF) return -1;

    // Passo B: Dijkstra de t para todos os vértices no grafo reverso
    dijkstra(grafo_reverso, n, t, dist_t);

    // Passo C: Constrói grafo auxiliar sem as arestas dos caminhos mínimos
    for (int v = 0; v < n; v++) {
        for (No* no = grafo[v]; no != NULL; no = no->prox) {
            int u = no->aresta.destino;
            int peso = no->aresta.peso;

            // Verifica se a aresta não faz parte de nenhum caminho mínimo
            if (dist_s[v] + peso + dist_t[u] > dist_s[t]) {
                adicionar_aresta(grafo_auxiliar, v, u, peso);
            }
        }
    }

    // Passo D: Dijkstra no grafo auxiliar
    dijkstra(grafo_auxiliar, n, s, dist_s);

    // Retorna a distância ou -1 se não houver caminho
    return dist_s[t] == INF ? -1 : dist_s[t];
}

int main() {
    int n, m;

    while (1) {
        scanf("%d %d", &n, &m);
        if (n == 0 && m == 0) break;

        int s, t;
        scanf("%d %d", &s, &t);

        printf("%d\n", resolver(n, m, s, t));

        // Libera a memória alocada
        for (int i = 0; i < n; i++) {
            No *atual = grafo[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
            atual = grafo_reverso[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
            atual = grafo_auxiliar[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }
    }

    return 0;
}
