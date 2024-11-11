#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#define criar_par make_pair
#define adicionar push_back
#define MAX_NOS 1010

using namespace std;

typedef pair<int, int> ParInt;
typedef vector<int> VetorInt;
typedef vector<ParInt> VetorParInt;

VetorInt pilhaNodos;
int qtdVertices, qtdArestas, paiDfs[MAX_NOS], menorValorDfs[MAX_NOS], numeroDfs[MAX_NOS], visitado[MAX_NOS];
int resposta, contadorDfs, flag, valido;

vector<VetorInt> grafoDirecionado(MAX_NOS), grafoNaoDirecionado(MAX_NOS);

struct Nodo {
    int pai, nivel;
};

typedef struct Nodo UniaoConjunto;

UniaoConjunto conjuntoDisjunto[MAX_NOS];

int lerInteiro()
{
    bool negativo = false;
    int resultado = 0;
    char caractere = getchar_unlocked();
    while (true) {
        if (caractere == '-')
            break;
        if (caractere >= '0' && caractere <= '9')
            break;
        caractere = getchar_unlocked();
    }
    if (caractere == '-')
        negativo = true;
    else
        resultado = caractere - '0';
    while (true) {
        caractere = getchar_unlocked();
        if (caractere < '0' || caractere > '9')
            break;
        resultado = resultado * 10 + (caractere - '0');
    }
    return negativo ? -resultado : resultado;
}

void inicializarGrafo()
{
    for (int i = 0; i < qtdVertices; ++i) {
        conjuntoDisjunto[i].pai = i;
        conjuntoDisjunto[i].nivel = visitado[i] = 0;
        paiDfs[i] = menorValorDfs[i] = numeroDfs[i] = 0;
        grafoDirecionado[i].clear();
        grafoNaoDirecionado[i].clear();
    }
}

void unificarConjuntos(int x, int y)
{
    if (conjuntoDisjunto[x].nivel > conjuntoDisjunto[y].nivel)
        conjuntoDisjunto[y].pai = x;
    else {
        conjuntoDisjunto[x].pai = y;
        if (conjuntoDisjunto[x].nivel == conjuntoDisjunto[y].nivel)
            conjuntoDisjunto[y].nivel++;
    }
}

int encontrarConjunto(int x)
{
    while (conjuntoDisjunto[x].pai != x)
        x = conjuntoDisjunto[x].pai;
    return x;
}

void unirConjuntos(int x, int y)
{
    unificarConjuntos(encontrarConjunto(x), encontrarConjunto(y));
}

bool mesmoConjunto(int x, int y)
{
    return encontrarConjunto(x) == encontrarConjunto(y);
}

void DFS(int u)
{
    menorValorDfs[u] = numeroDfs[u] = contadorDfs++;
    pilhaNodos.adicionar(u);
    visitado[u] = true;
    VetorInt::iterator it;
    for (it = grafoDirecionado[u].begin(); it != grafoDirecionado[u].end(); ++it) {
        if (!numeroDfs[*it])
            DFS(*it);
        if (visitado[*it])
            menorValorDfs[u] = min(menorValorDfs[u], menorValorDfs[*it]);
    }
    if (menorValorDfs[u] == numeroDfs[u]) {
        int totalComponentes = 0;
        while (true) {
            int nodo = pilhaNodos.back();
            pilhaNodos.pop_back();
            totalComponentes++;
            if (u == nodo)
                break;
        }
        if (totalComponentes == qtdVertices)
            flag = true;
    }
}

void encontrarPontosDeCorte(int u)
{
    VetorInt::iterator it;
    menorValorDfs[u] = numeroDfs[u] = contadorDfs++;
    for (it = grafoNaoDirecionado[u].begin(); it != grafoNaoDirecionado[u].end(); ++it) {
        if (!numeroDfs[*it]) {
            resposta++;
            paiDfs[*it] = u;
            encontrarPontosDeCorte(*it);
            if (menorValorDfs[*it] > numeroDfs[u])
                if (mesmoConjunto(u, *it))
                    valido = true;
            menorValorDfs[u] = min(menorValorDfs[u], menorValorDfs[*it]);
        } else if (*it != paiDfs[u])
            menorValorDfs[u] = min(menorValorDfs[u], numeroDfs[*it]);
    }
}

int main()
{
    int origem, destino, tipoAresta;
    while (scanf("%d %d", &qtdVertices, &qtdArestas) != EOF) {
        inicializarGrafo();
        resposta = contadorDfs = flag = valido = 0;
        origem = lerInteiro();
        destino = lerInteiro();
        tipoAresta = lerInteiro();
        for (int i = 0; i < qtdArestas - 1; ++i) {
            origem = lerInteiro();
            destino = lerInteiro();
            tipoAresta = lerInteiro();
            grafoDirecionado[origem - 1].adicionar(destino - 1);
            grafoNaoDirecionado[origem - 1].adicionar(destino - 1);
            grafoNaoDirecionado[destino - 1].adicionar(origem - 1);
            if (tipoAresta == 2)
                grafoDirecionado[destino - 1].adicionar(origem - 1);
            else
                unirConjuntos(destino - 1, origem - 1);
        }
        DFS(0);
        if (flag)
            printf("-\n");
        else {
            for (int i = 0; i < qtdVertices; ++i)
                menorValorDfs[i] = numeroDfs[i] = 0;
            contadorDfs = 0;
            encontrarPontosDeCorte(0);
            if (resposta != qtdVertices)
                printf("*\n");
            else {
                if (valido)
                    printf("2\n");
                else
                    printf("1\n");
            }
        }
    }
    return 0;
}
