#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char name[50];
    char neighbors[MAX][50];
    int count;
} Node;

Node nodes[MAX];
int nodeCount = 0;

// Find node by name or create new one
int getNodeIndex(char *name) {
    for (int i = 0; i < nodeCount; i++)
        if (strcmp(nodes[i].name, name) == 0)
            return i;
    strcpy(nodes[nodeCount].name, name);
    nodes[nodeCount].count = 0;
    return nodeCount++;
}

int main() {
    FILE *file = fopen("input.sif", "r");
    char src[50], rel[50], dst[50];
    while (fscanf(file, "%s %s %s", src, rel, dst) == 3) {
        int s = getNodeIndex(src);
        int d = getNodeIndex(dst);
        strcpy(nodes[s].neighbors[nodes[s].count++], nodes[d].name);
    }
    fclose(file);

    printf("Graph:\n");
    for (int i = 0; i < nodeCount; i++) {
        printf("%s -> ", nodes[i].name);
        for (int j = 0; j < nodes[i].count; j++)
            printf("%s ", nodes[i].neighbors[j]);
        printf("\n");
    }

    return 0;
}
