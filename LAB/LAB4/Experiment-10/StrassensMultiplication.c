#include <stdio.h>
#include <stdlib.h>

// Function to add two matrices
void add(int n, int A[10][10], int B[10][10], int C[10][10]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Function to subtract two matrices
void subtract(int n, int A[10][10], int B[10][10], int C[10][10]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Strassen’s Algorithm (Recursive)
void strassen(int n, int A[10][10], int B[10][10], int C[10][10]) {
    // Base case: 1x1 matrix
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;
    int A11[10][10], A12[10][10], A21[10][10], A22[10][10];
    int B11[10][10], B12[10][10], B21[10][10], B22[10][10];
    int M1[10][10], M2[10][10], M3[10][10], M4[10][10], M5[10][10], M6[10][10], M7[10][10];
    int temp1[10][10], temp2[10][10];
    int C11[10][10], C12[10][10], C21[10][10], C22[10][10];

    // Divide A and B into 4 submatrices
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // M1 = (A11 + A22) * (B11 + B22)
    add(k, A11, A22, temp1);
    add(k, B11, B22, temp2);
    strassen(k, temp1, temp2, M1);

    // M2 = (A21 + A22) * B11
    add(k, A21, A22, temp1);
    strassen(k, temp1, B11, M2);

    // M3 = A11 * (B12 - B22)
    subtract(k, B12, B22, temp1);
    strassen(k, A11, temp1, M3);

    // M4 = A22 * (B21 - B11)
    subtract(k, B21, B11, temp1);
    strassen(k, A22, temp1, M4);

    // M5 = (A11 + A12) * B22
    add(k, A11, A12, temp1);
    strassen(k, temp1, B22, M5);

    // M6 = (A21 - A11) * (B11 + B12)
    subtract(k, A21, A11, temp1);
    add(k, B11, B12, temp2);
    strassen(k, temp1, temp2, M6);

    // M7 = (A12 - A22) * (B21 + B22)
    subtract(k, A12, A22, temp1);
    add(k, B21, B22, temp2);
    strassen(k, temp1, temp2, M7);

    // Calculate C11, C12, C21, C22
    add(k, M1, M4, temp1);
    subtract(k, temp1, M5, temp2);
    add(k, temp2, M7, C11);

    add(k, M3, M5, C12);

    add(k, M2, M4, C21);

    subtract(k, M1, M2, temp1);
    add(k, temp1, M3, temp2);
    add(k, temp2, M6, C22);

    // Combine results into C
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
}

// Function to display a matrix
void printMatrix(int n, int A[10][10]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%4d ", A[i][j]);
        printf("\n");
    }
}

int main() {
    int A[10][10], B[10][10], C[10][10];
    int n;

    printf("Enter the size of the square matrices (power of 2, e.g., 2, 4, 8): ");
    scanf("%d", &n);

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    strassen(n, A, B, C);

    printf("\nResultant Matrix after Strassen Multiplication:\n");
    printMatrix(n, C);

   return 0;
}
