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
No* novo_grafo[MAX_N];
int dist[MAX_N];
int visitado[MAX_N];
int arestas_caminho_minimo[MAX_N][MAX_N];
ElementoPQ pq[MAX_M];
int tam_pq;

// Funções auxiliares para a fila de prioridade
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

// Função para adicionar aresta ao grafo
void adicionar_aresta(No** grafo, int origem, int destino, int peso) {
    No* novo = (No*)malloc(sizeof(No));
    novo->aresta.destino = destino;
    novo->aresta.peso = peso;
    novo->prox = grafo[origem];
    grafo[origem] = novo;
}

// Implementação do algoritmo de Dijkstra
void dijkstra(No** grafo, int n, int inicio, int fim) {
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

// Função para marcar arestas do caminho mínimo
void marcar_caminhos_minimos(No** grafo, int n, int inicio, int fim) {
    memset(arestas_caminho_minimo, 0, sizeof(arestas_caminho_minimo));
    
    // Primeiro Dijkstra para encontrar distâncias mínimas
    dijkstra(grafo, n, inicio, fim);
    
    // Se não existe caminho, retorna
    if (dist[fim] == INF) return;
    
    // Para cada vértice
    for (int v = 0; v < n; v++) {
        for (No* no = grafo[v]; no != NULL; no = no->prox) {
            int u = no->aresta.destino;
            int peso = no->aresta.peso;
            
            // Se esta aresta faz parte de algum caminho mínimo
            if (dist[v] + peso + dist[u] == dist[fim]) {
                arestas_caminho_minimo[v][u] = 1;
            }
        }
    }
}

// Função principal para resolver o problema
int resolver(int n, int m, int inicio, int fim) {
    // Limpa os grafos
    for (int i = 0; i < n; i++) {
        grafo[i] = NULL;
        novo_grafo[i] = NULL;
    }
    
    // Lê as arestas e constrói o grafo original
    for (int i = 0; i < m; i++) {
        int u, v, p;
        scanf("%d %d %d", &u, &v, &p);
        adicionar_aresta(grafo, u, v, p);
    }
    
    // Marca as arestas que pertencem a caminhos mínimos
    marcar_caminhos_minimos(grafo, n, inicio, fim);
    
    // Se não existe caminho mínimo, retorna -1
    if (dist[fim] == INF) return -1;
    
    // Constrói novo grafo sem as arestas dos caminhos mínimos
    for (int v = 0; v < n; v++) {
        for (No* no = grafo[v]; no != NULL; no = no->prox) {
            int u = no->aresta.destino;
            int peso = no->aresta.peso;
            
            if (!arestas_caminho_minimo[v][u]) {
                adicionar_aresta(novo_grafo, v, u, peso);
            }
        }
    }
    
    // Encontra o menor caminho no novo grafo
    dijkstra(novo_grafo, n, inicio, fim);
    
    // Se não existe caminho, retorna -1
    return dist[fim] == INF ? -1 : dist[fim];
}

int main() {
    int n, m;
    
    while (1) {
        scanf("%d %d", &n, &m);
        if (n == 0 && m == 0) break;
        
        int inicio, fim;
        scanf("%d %d", &inicio, &fim);
        
        printf("%d\n", resolver(n, m, inicio, fim));
        
        // Libera a memória alocada
        for (int i = 0; i < n; i++) {
            No *atual = grafo[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
            atual = novo_grafo[i];
            while (atual != NULL) {
                No *temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }
    }
    
    return 0;
}