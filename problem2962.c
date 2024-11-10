#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SENSORES 1000
#define TRUE 1
#define FALSE 0

typedef struct {
    int x, y, s; // Coordenadas (x, y) e sensibilidade (raio) do sensor
} Sensor;

Sensor sensores[MAX_SENSORES];
int M, N, K; // Dimensões do salão e número de sensores
int visitado_inicial = FALSE, visitado_final = FALSE;

// Função para calcular a distância entre dois pontos (x1, y1) e (x2, y2)
double distancia(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Função para verificar se um ponto está dentro do alcance de qualquer sensor
int esta_fora_do_alcance(int x, int y) {
    for (int i = 0; i < K; i++) {
        if (distancia(x, y, sensores[i].x, sensores[i].y) <= sensores[i].s) {
            return FALSE;
        }
    }
    return TRUE;
}

// Função recursiva DFS para encontrar um caminho seguro
void dfs(int x, int y) {
    // Se já chegou ao ponto final
    if (x == M && y == N) {
        visitado_final = TRUE;
        return;
    }

    // Marca posição inicial como visitada
    if (x == 0 && y == 0) {
        visitado_inicial = TRUE;
    }

    // Movimentos possíveis
    int movimentos[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    for (int i = 0; i < 4; i++) {
        int nx = x + movimentos[i][0];
        int ny = y + movimentos[i][1];

        // Verifica se a nova posição é válida, está dentro dos limites e fora do alcance dos sensores
        if (nx >= 0 && nx <= M && ny >= 0 && ny <= N && esta_fora_do_alcance(nx, ny)) {
            dfs(nx, ny); // Faz a busca recursiva
            if (visitado_final) return; // Para a busca se encontrou o final
        }
    }
}

int main() {
    // Leitura das dimensões do salão e do número de sensores
    scanf("%d %d %d", &M, &N, &K);

    // Leitura dos sensores
    for (int i = 0; i < K; i++) {
        scanf("%d %d %d", &sensores[i].x, &sensores[i].y, &sensores[i].s);
    }

    // Inicia a DFS a partir do ponto inicial (0, 0)
    if (esta_fora_do_alcance(0, 0)) {
        dfs(0, 0);
    }

    // Verifica se conseguimos visitar o ponto final
    if (visitado_final) {
        printf("S\n"); // É possível roubar a pintura
    } else {
        printf("N\n"); // Não é possível
    }

    return 0;
}
