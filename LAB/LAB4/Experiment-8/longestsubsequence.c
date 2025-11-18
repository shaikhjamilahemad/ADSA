#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100


int LCS_Length(char X[], char Y[], int m, int n, int dp[MAX][MAX]) {
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                dp[i][j] = 1 + dp[i - 1][j - 1];
            else
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
        }
    }
    return dp[m][n];
}


void print_LCS(char X[], char Y[], int m, int n, int dp[MAX][MAX]) {
    int index = dp[m][n];
    char lcs[index + 1];
    lcs[index] = '\0'; 

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[index - 1] = X[i - 1];
            i--;
            j--;
            index--;
        } else if (dp[i - 1][j] > dp[i][j - 1])
            i--;
        else
            j--;
    }

    printf("\nLongest Common Subsequence (LCS): %s\n", lcs);
    printf("Length of LCS: %d\n", dp[m][n]);
}


void generateAllSubsequences(char *X, int m, char *Y, int n, char *res, int i, int j, int index) {
    if (i == m || j == n) {
        res[index] = '\0';
        if (index > 0)
            printf("%s\n", res);
        return;
    }

    if (X[i] == Y[j]) {
        res[index] = X[i];
        generateAllSubsequences(X, m, Y, n, res, i + 1, j + 1, index + 1);
    }

    generateAllSubsequences(X, m, Y, n, res, i + 1, j, index);
    generateAllSubsequences(X, m, Y, n, res, i, j + 1, index);
}


void printAllCommonSubsequences(char X[], char Y[]) {
    int m = strlen(X);
    int n = strlen(Y);
    char res[MAX];
    printf("\nAll Common Subsequences (may include duplicates):\n");
    generateAllSubsequences(X, m, Y, n, res, 0, 0, 0);
}


int main() {
    char X[MAX], Y[MAX];
    int dp[MAX][MAX];

    printf("Enter first string: ");
    scanf("%s", X);
    printf("Enter second string: ");
    scanf("%s", Y);

    int m = strlen(X);
    int n = strlen(Y);

    int length = LCS_Length(X, Y, m, n, dp);
    print_LCS(X, Y, m, n, dp);
    printAllCommonSubsequences(X, Y);

    return 0;
}
