#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CRUZAMENTOS 1010

typedef struct {
    int primeiro;
    int segundo;
} Par;

typedef struct {
    int *dados;
    int tamanho;
    int capacidade;
} Lista;

typedef struct {
    int pai, rank;
} ConjuntoUniao;

ConjuntoUniao conjunto[MAX_CRUZAMENTOS];
Lista pilha;
int N, M, pai_dfs[MAX_CRUZAMENTOS], menor_dfs[MAX_CRUZAMENTOS], numero_dfs[MAX_CRUZAMENTOS], visitado[MAX_CRUZAMENTOS];
int resposta, contador, flag, permitido;
Lista ruas_direcionadas[MAX_CRUZAMENTOS], ruas_nao_direcionadas[MAX_CRUZAMENTOS];

// Função para inicializar uma lista dinâmica
void lista_inicializar(Lista *lista, int capacidade) {
    lista->dados = (int*)malloc(capacidade * sizeof(int));
    lista->tamanho = 0;
    lista->capacidade = capacidade;
}

// Função para adicionar um elemento à lista dinâmica
void lista_adicionar(Lista *lista, int valor) {
    if (lista->tamanho == lista->capacidade) {
        lista->capacidade *= 2;
        lista->dados = (int*)realloc(lista->dados, lista->capacidade * sizeof(int));
    }
    lista->dados[lista->tamanho++] = valor;
}

// Função para limpar a lista dinâmica
void lista_limpar(Lista *lista) {
    lista->tamanho = 0;
}

// Função para acessar um elemento da lista dinâmica
int lista_obter(Lista *lista, int indice) {
    return lista->dados[indice];
}

// Função para leitura de inteiros de forma eficiente
int lerInteiro() {
    bool negativo = false;
    int resultado = 0;
    char ch = getchar();
    while (true) {
        if (ch == '-')
            break;
        if (ch >= '0' && ch <= '9')
            break;
        ch = getchar();
    }
    if (ch == '-')
        negativo = true;
    else
        resultado = ch - '0';
    while (true) {
        ch = getchar();
        if (ch < '0' || ch > '9')
            break;
        resultado = resultado * 10 + (ch - '0');
    }
    if (negativo)
        return -resultado;
    else
        return resultado;
}

// Função para inicializar estruturas para um novo caso de teste
void inicializar() {
    for (int i = 0; i < N; ++i) {
        conjunto[i].pai = i;
        conjunto[i].rank = visitado[i] = 0;
        pai_dfs[i] = menor_dfs[i] = numero_dfs[i] = 0;
        lista_limpar(&ruas_direcionadas[i]);
        lista_limpar(&ruas_nao_direcionadas[i]);
    }
    pilha.tamanho = 0;
}

// Função para unir dois conjuntos
void unir(int x, int y) {
    if (conjunto[x].rank > conjunto[y].rank)
        conjunto[y].pai = x;
    else {
        conjunto[x].pai = y;
        if (conjunto[x].rank == conjunto[y].rank)
            conjunto[y].rank += 1;
    }
}

// Função para encontrar o conjunto de um elemento
int encontrarConjunto(int x) {
    while (conjunto[x].pai != x)
        x = conjunto[x].pai;
    return x;
}

// Função para verificar se dois elementos estão no mesmo conjunto
bool mesmoConjunto(int x, int y) {
    return encontrarConjunto(x) == encontrarConjunto(y);
}

// Algoritmo para verificar conectividade
void verificaConexao(int u) {
    menor_dfs[u] = numero_dfs[u] = contador++;
    lista_adicionar(&pilha, u);
    visitado[u] = true;
    for (int i = 0; i < ruas_direcionadas[u].tamanho; ++i) {
        int v = lista_obter(&ruas_direcionadas[u], i);
        if (!numero_dfs[v])
            verificaConexao(v);
        if (visitado[v])
            menor_dfs[u] = (menor_dfs[u] < menor_dfs[v]) ? menor_dfs[u] : menor_dfs[v];
    }
    if (menor_dfs[u] == numero_dfs[u]) {
        int tamanho_componente = 0;
        while (true) {
            int vertice = pilha.dados[--pilha.tamanho];
            tamanho_componente++;
            if (u == vertice)
                break;
        }
        if (tamanho_componente == N)
            flag = true;
    }
}

// Função para detectar pontes e verificar a conectividade das ruas
void detectarPonte(int u) {
    menor_dfs[u] = numero_dfs[u] = contador++;
    for (int i = 0; i < ruas_nao_direcionadas[u].tamanho; ++i) {
        int v = lista_obter(&ruas_nao_direcionadas[u], i);
        if (!numero_dfs[v]) {
            resposta++;
            pai_dfs[v] = u;
            detectarPonte(v);
            if (menor_dfs[v] > numero_dfs[u])
                if (mesmoConjunto(u, v))
                    permitido = true;
            menor_dfs[u] = (menor_dfs[u] < menor_dfs[v]) ? menor_dfs[u] : menor_dfs[v];
        } else if (v != pai_dfs[u]) {
            menor_dfs[u] = (menor_dfs[u] < numero_dfs[v]) ? menor_dfs[u] : numero_dfs[v];
        }
    }
}

int main() {
    for (int i = 0; i < MAX_CRUZAMENTOS; i++) {
        lista_inicializar(&ruas_direcionadas[i], 10);
        lista_inicializar(&ruas_nao_direcionadas[i], 10);
    }
    lista_inicializar(&pilha, 10);
    
    int cruzamentoA, cruzamentoB, tipo;
    while (scanf("%d %d", &N, &M) != EOF) {
        inicializar();
        resposta = contador = flag = permitido = 0;
        for (int i = 0; i < M; ++i) {
            cruzamentoA = lerInteiro();
            cruzamentoB = lerInteiro();
            tipo = lerInteiro();
            lista_adicionar(&ruas_direcionadas[cruzamentoA - 1], cruzamentoB - 1);
            lista_adicionar(&ruas_nao_direcionadas[cruzamentoA - 1], cruzamentoB - 1);
            lista_adicionar(&ruas_nao_direcionadas[cruzamentoB - 1], cruzamentoA - 1);
            if (tipo == 2)
                lista_adicionar(&ruas_direcionadas[cruzamentoB - 1], cruzamentoA - 1);
            else
                unir(cruzamentoB - 1, cruzamentoA - 1);
        }
        verificaConexao(0);
        if (flag)
            printf("-\n");
        else {
            for (int i = 0; i < N; ++i)
                menor_dfs[i] = numero_dfs[i] = 0;
            contador = 0;
            detectarPonte(0);
            if (resposta != N)
                printf("*\n");
            else {
                if (permitido)
                    printf("2\n");
                else
                    printf("1\n");
            }
        }
    }
    
    for (int i = 0; i < MAX_CRUZAMENTOS; i++) {
        free(ruas_direcionadas[i].dados);
        free(ruas_nao_direcionadas[i].dados);
    }
    free(pilha.dados);

    return 0;
}
