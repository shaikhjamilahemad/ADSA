#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 50   // maximum number of vertices per set

int nU, nV;               // number of vertices in sets U and V
int graph[MAX][MAX];      // adjacency matrix: graph[u][v] = 1 if edge (u,v) exists
int matchV[MAX];          // matchV[v] = u means V[v] is matched with U[u]
bool visited[MAX];        // visited[v] = true if vertex v in V has been checked

// DFS-based function to find if U[u] can be matched
bool bpm(int u) {
    for (int v = 0; v < nV; v++) {
        if (graph[u][v] && !visited[v]) {
            visited[v] = true;

            // If v is not matched yet OR previously matched vertex can be reassigned
            if (matchV[v] == -1 || bpm(matchV[v])) {
                matchV[v] = u;
                return true;
            }
        }
    }
    return false;
}

// Function to return size of maximum bipartite matching
int maxBPM() {
    memset(matchV, -1, sizeof(matchV)); // initially no vertex in V is matched
    int result = 0;

    for (int u = 0; u < nU; u++) {
        memset(visited, false, sizeof(visited));
        if (bpm(u))
            result++;
    }

    return result;
}

int main() {
    printf("Enter number of vertices in set U and V: ");
    scanf("%d %d", &nU, &nV);

    printf("Enter adjacency matrix (U x V):\n");
    for (int i = 0; i < nU; i++) {
        for (int j = 0; j < nV; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    int maxMatch = maxBPM();

    printf("\nMaximum Matching possible = %d\n", maxMatch);
    printf("\nMatched pairs (U -> V):\n");

    for (int v = 0; v < nV; v++) {
        if (matchV[v] != -1) {
            printf("U%d -> V%d\n", matchV[v], v);
        }
    }

    return 0;
}