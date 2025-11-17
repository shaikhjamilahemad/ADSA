#include <stdio.h>
#include <stdlib.h>

#define MAX 3   // Max keys in a node
#define MIN 1   // Min keys in a node

struct BTreeNode {
    int val[MAX + 1];
    int count;
    struct BTreeNode *link[MAX + 1];
};

struct BTreeNode *root;

// Create a new node
struct BTreeNode *createNode(int val, struct BTreeNode *child) {
    struct BTreeNode *newNode;
    newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Insert a value into a node
void addValToNode(int val, int pos, struct BTreeNode *node, struct BTreeNode *child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// Split a node if full
void splitNode(int val, int *pval, int pos, struct BTreeNode *node, struct BTreeNode *child, struct BTreeNode **newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    (*newNode)->count = MAX - median;
    node->count = median;

    if (pos <= MIN)
        addValToNode(val, pos, node, child);
    else
        addValToNode(val, pos - median, *newNode, child);

    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Set value into node recursively
int setValue(int val, int *pval, struct BTreeNode *node, struct BTreeNode **child) {
    int pos;
    if (node == NULL) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1])
        pos = 0;
    else {
        for (pos = node->count; (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            printf("Duplicates not allowed!\n");
            return 0;
        }
    }

    if (setValue(val, pval, node->link[pos], child)) {
        if (node->count < MAX)
            addValToNode(*pval, pos, node, *child);
        else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Insert value
void insert(int val) {
    int flag, i;
    struct BTreeNode *child;

    flag = setValue(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

// Display B-Tree
void display(struct BTreeNode *node) {
    int i;
    if (node != NULL) {
        for (i = 0; i < node->count; i++) {
            display(node->link[i]);
            printf("%d ", node->val[i + 1]);
        }
        display(node->link[i]);
    }
}

// Delete function
void deleteDemo(struct BTreeNode *node, int key) {
    int i;
    if (node == NULL)
        return;

    for (i = 1; i <= node->count; i++) {
        if (node->val[i] == key) {
            for (; i < node->count; i++)
                node->val[i] = node->val[i + 1];
            node->count--;
            printf("\nDeleted %d (simple demo, no rebalance)\n", key);
            return;
        }
    }
    for (i = 0; i <= node->count; i++)
        deleteDemo(node->link[i], key);
}

int main() {
    int i;
    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};

    printf("Inserting values into B-Tree:\n");
    for (i = 0; i < 8; i++)
        insert(values[i]);

    printf("B-Tree after insertion:\n");
    display(root);

    deleteDemo(root, 6);
    printf("\nB-Tree after deleting 6:\n");
    display(root);

    return 0;
}
