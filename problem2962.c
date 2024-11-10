#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SENSORES 1000
#define EPSILON 1e-9

typedef struct {
    int x, y;
    int sensibilidade;
} Sensor;

// Estrutura para representar o grafo usando lista de adjacência
typedef struct {
    bool** adj;  // Matriz de adjacência
    int V;       // Número de vértices
} Grafo;

// Função para criar o grafo
Grafo* criar_grafo(int V) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->V = V;
    g->adj = (bool**)malloc(V * sizeof(bool*));
    for (int i = 0; i < V; i++) {
        g->adj[i] = (bool*)calloc(V, sizeof(bool));
    }
    return g;
}

// Função para liberar a memória do grafo
void liberar_grafo(Grafo* g) {
    for (int i = 0; i < g->V; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

// Verifica se dois círculos se interceptam
bool circulos_interceptam(Sensor s1, Sensor s2) {
    double dist = sqrt(pow(s1.x - s2.x, 2.0) + pow(s1.y - s2.y, 2.0));
    return dist <= s1.sensibilidade + s2.sensibilidade;
}

// Verifica se um círculo intercepta uma parede
bool circulo_intercepta_parede(Sensor s, int M, int N, int parede) {
    // parede: 0=esquerda, 1=inferior, 2=direita, 3=superior
    switch(parede) {
        case 0: // Parede esquerda (x = 0)
            return s.x <= s.sensibilidade;
        case 1: // Parede inferior (y = 0)
            return s.y <= s.sensibilidade;
        case 2: // Parede direita (x = M)
            return M - s.x <= s.sensibilidade;
        case 3: // Parede superior (y = N)
            return N - s.y <= s.sensibilidade;
    }
    return false;
}

// Implementação da busca em profundidade (DFS)
bool dfs(Grafo* g, bool* visitado, int v, int K) {
    visitado[v] = true;

    // Se alcançamos a parede oposta (K+3 ou K+4)
    if (v == K+3 || v == K+4) {
        return true;
    }

    for (int i = 0; i < g->V; i++) {
        if (g->adj[v][i] && !visitado[i]) {
            if (dfs(g, visitado, i, K)) {
                return true;
            }
        }
    }
    return false;
}

// Função principal que resolve o problema
bool verifica_roubo_possivel(int M, int N, int K, Sensor sensores[]) {
    // Criar grafo com K sensores + 4 paredes
    Grafo* g = criar_grafo(K + 4);

    // Adicionar arestas entre sensores que se interceptam
    for (int i = 0; i < K; i++) {
        for (int j = i + 1; j < K; j++) {
            if (circulos_interceptam(sensores[i], sensores[j])) {
                g->adj[i][j] = g->adj[j][i] = true;
            }
        }
    }

    // Adicionar arestas entre sensores e paredes
    for (int i = 0; i < K; i++) {
        for (int p = 0; p < 4; p++) {
            if (circulo_intercepta_parede(sensores[i], M, N, p)) {
                g->adj[i][K+p] = g->adj[K+p][i] = true;
            }
        }
    }

    // Realizar DFS a partir das paredes K+1 e K+2
    bool* visitado = (bool*)calloc(g->V, sizeof(bool));
    bool resultado = false;

    // DFS a partir da parede esquerda (K+1)
    resultado = dfs(g, visitado, K+0, K);

    // Se não encontrou caminho, tenta a partir da parede inferior (K+2)
    if (!resultado) {
        for (int i = 0; i < g->V; i++) visitado[i] = false;
        resultado = dfs(g, visitado, K+1, K);
    }

    free(visitado);
    liberar_grafo(g);
    return !resultado;  // Retorna true se NÃO existe barreira
}

int main() {
    int M, N, K;
    scanf("%d %d %d", &M, &N, &K);

    Sensor* sensores = (Sensor*)malloc(K * sizeof(Sensor));
    for (int i = 0; i < K; i++) {
        scanf("%d %d %d", &sensores[i].x, &sensores[i].y, &sensores[i].sensibilidade);
    }

    printf("%c\n", verifica_roubo_possivel(M, N, K, sensores) ? 'S' : 'N');

    free(sensores);
    return 0;
}
