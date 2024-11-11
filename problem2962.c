#include <stdio.h>
#include <math.h>

#define MAX_SENSORES 1000
#define MAX_N 1004

/*O Union-Find é usado para agrupar sensores e paredes em conjuntos. Sensores que estão conectados entre si ou com as paredes são unidos no mesmo conjunto.
A operação unir é usada para agrupar sensores ou sensores e paredes, enquanto a operação encontrar é usada para verificar se dois sensores ou um sensor e uma parede estão no mesmo conjunto.
No final, se a entrada ou saída do retângulo estiver conectada a qualquer sensor ou parede, o código imprime "N" (não é possível realizar o roubo), caso contrário, imprime "S" (é possível realizar o roubo).
Isso permite que o código determine se um "caminho" de sensores ou paredes bloqueia a entrada ou saída.*/

// Estrutura para o algoritmo Union-Find
typedef struct {
    int pai[MAX_N];
    int rank[MAX_N];
} UnionFind;

// Função para inicializar o Union-Find
void inicializar_union_find(UnionFind *uf, int n) {
    for (int i = 0; i < n; i++) {
        uf->pai[i] = i;
        uf->rank[i] = 0;
    }
}

// Função para encontrar o representante de um conjunto
int encontrar(UnionFind *uf, int n) {
    if (uf->pai[n] != n) {
        uf->pai[n] = encontrar(uf, uf->pai[n]);
    }
    return uf->pai[n];
}

// Função para unir dois conjuntos
void unir(UnionFind *uf, int a, int b) {
    int pai_a = encontrar(uf, a);
    int pai_b = encontrar(uf, b);

    if (uf->rank[pai_a] > uf->rank[pai_b]) {
        uf->pai[pai_b] = pai_a;
    } else if (uf->rank[pai_a] < uf->rank[pai_b]) {
        uf->pai[pai_a] = pai_b;
    } else {
        uf->pai[pai_b] = pai_a;
        uf->rank[pai_a]++;
    }
}

// Função para verificar se dois pontos estão no mesmo conjunto
int estao_conectados(UnionFind *uf, int a, int b) {
    return encontrar(uf, a) == encontrar(uf, b);
}

// Função para calcular a distância entre dois pontos
double distancia(int p1[3], int p2[3]) {
    return sqrt(
        (p1[0] - p2[0]) * (p1[0] - p2[0]) +
        (p1[1] - p2[1]) * (p1[1] - p2[1])
    );
}

int main() {
    int m, n, k;
    int sensores[MAX_SENSORES][3];

    // Leitura das dimensões e número de sensores
    scanf("%d %d %d", &m, &n, &k);

    // Inicializa a estrutura Union-Find para k sensores + 4 "paredes" (topo, direita, baixo, esquerda)
    UnionFind union_find;
    inicializar_union_find(&union_find, k + 4);

    // Leitura das informações dos sensores
    for (int i = 0; i < k; i++) {
        scanf("%d %d %d", &sensores[i][0], &sensores[i][1], &sensores[i][2]);

        // Sensor detecta a parede do topo
        if (sensores[i][1] + sensores[i][2] >= n)
            unir(&union_find, 0, i + 4);

        // Sensor detecta a parede da direita
        if (sensores[i][0] + sensores[i][2] >= m)
            unir(&union_find, 1, i + 4);

        // Sensor detecta a parede de baixo
        if (sensores[i][1] - sensores[i][2] <= 0)
            unir(&union_find, 2, i + 4);

        // Sensor detecta a parede da esquerda
        if (sensores[i][0] - sensores[i][2] <= 0)
            unir(&union_find, 3, i + 4);
    }

    // Verifica se há interseção entre sensores
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            if (distancia(sensores[i], sensores[j]) <= sensores[i][2] + sensores[j][2])
                unir(&union_find, i + 4, j + 4);
        }
    }

    // Verifica se a entrada ou saída estão conectadas a alguma parede ou sensor
    if (estao_conectados(&union_find, 0, 1) || estao_conectados(&union_find, 0, 2) ||
        estao_conectados(&union_find, 3, 1) || estao_conectados(&union_find, 3, 2))
        printf("N\n");  // Se houver conexão, o roubo não é possível
    else
        printf("S\n");  // Se não houver conexão, o roubo é possível

    return 0;
}
