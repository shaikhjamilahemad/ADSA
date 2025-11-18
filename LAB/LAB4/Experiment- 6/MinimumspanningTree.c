#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define INF 999999

int graph[MAX][MAX], n;

// ---------- Read Graph in SIF Format ----------
void readSIF() {
    int i, j, weight;
    char src[20], dest[20];
    printf("Enter number of vertices: ");
    scanf("%d", &n);
    printf("Enter edges in SIF format (u v w), enter -1 -1 -1 to stop:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            graph[i][j] = INF;

    while (1) {
        scanf("%s %s %d", src, dest, &weight);
        if (weight == -1)
            break;
        int u = src[0] - 'A';  // Convert vertex name (A,B,C,..) to index
        int v = dest[0] - 'A';
        graph[u][v] = graph[v][u] = weight;
    }
}

// ---------- Prim’s Algorithm ----------
void prims() {
    int selected[MAX];
    int i, j, edgeCount = 0, min, x, y;
    int totalCost = 0;

    for (i = 0; i < n; i++)
        selected[i] = 0;

    selected[0] = 1;

    printf("\nEdges in MST using Prim’s Algorithm:\n");
    while (edgeCount < n - 1) {
        min = INF;
        x = 0;
        y = 0;

        for (i = 0; i < n; i++) {
            if (selected[i]) {
                for (j = 0; j < n; j++) {
                    if (!selected[j] && graph[i][j]) {
                        if (min > graph[i][j]) {
                            min = graph[i][j];
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }
        printf("%c - %c : %d\n", x + 'A', y + 'A', graph[x][y]);
        totalCost += graph[x][y];
        selected[y] = 1;
        edgeCount++;
    }
    printf("Total cost of MST (Prim’s): %d\n", totalCost);
}

// ---------- Kruskal’s Algorithm ----------
int parent[MAX];

int find(int i) {
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void union1(int i, int j) {
    int a = find(i);
    int b = find(j);
    parent[a] = b;
}

void kruskal() {
    int i, j, u, v, min, edges = 0, totalCost = 0;

    for (i = 0; i < n; i++)
        parent[i] = i;

    printf("\nEdges in MST using Kruskal’s Algorithm:\n");

    while (edges < n - 1) {
        min = INF;
        u = -1;
        v = -1;

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (find(i) != find(j) && graph[i][j] < min) {
                    min = graph[i][j];
                    u = i;
                    v = j;
                }
            }
        }

        union1(u, v);
        printf("%c - %c : %d\n", u + 'A', v + 'A', min);
        totalCost += min;
        edges++;
    }
    printf("Total cost of MST (Kruskal’s): %d\n", totalCost);
}

// ---------- Main Function ----------
int main() {
    readSIF();
    prims();
    kruskal();
    return 0;
}
