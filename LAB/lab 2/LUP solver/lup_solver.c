#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 
 * LUP Decomposition (Gaussian elimination with partial pivoting)
 * Input: Matrix A
 * Output: Permutation matrix P, lower triangular matrix L, and upper triangular matrix U
 */

#define AIDX(A, n, i, j) (A[(i)*(n) + (j)])

// Function to swap rows in matrix A
static void swap_rows(double *A, int n, int r1, int r2) {
    if (r1 == r2) return;
    for (int j = 0; j < n; ++j) {
        double tmp = AIDX(A, n, r1, j);
        AIDX(A, n, r1, j) = AIDX(A, n, r2, j);
        AIDX(A, n, r2, j) = tmp;
    }
}

// Function to print a matrix
static void print_matrix(const char *name, const double *A, int n) {
    printf("%s =\n", name);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%12.6f ", AIDX(A, n, i, j));
        }
        printf("\n");
    }
}

// Gaussian elimination with partial pivoting; stores L below diag and U on/above diag in A; permutation in Pvec
static int lup_decompose(double *A, int n, int *Pvec, double tol) {
    for (int i = 0; i < n; ++i) Pvec[i] = i;

    for (int k = 0; k < n; ++k) {
        int p = k; double maxA = fabs(AIDX(A, n, k, k));
        for (int i = k + 1; i < n; ++i) {
            double v = fabs(AIDX(A, n, i, k));
            if (v > maxA) { maxA = v; p = i; }
        }
        if (maxA < tol) return 0; // singular/near-singular

        if (p != k) {
            swap_rows(A, n, p, k);
            int tp = Pvec[p]; Pvec[p] = Pvec[k]; Pvec[k] = tp;
        }

        for (int i = k + 1; i < n; ++i) {
            AIDX(A, n, i, k) /= AIDX(A, n, k, k);
            double lik = AIDX(A, n, i, k);
            for (int j = k + 1; j < n; ++j) {
                AIDX(A, n, i, j) -= lik * AIDX(A, n, k, j);
            }
        }
    }
    return 1;
}

// Extract explicit L, U, and P from packed A and permutation vector
static void extract_LUP(const double *Apu, int n, const int *Pvec, double *L, double *U, double *P) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            AIDX(L, n, i, j) = (i == j) ? 1.0 : (i > j ? AIDX(Apu, n, i, j) : 0.0);
            AIDX(U, n, i, j) = (i <= j) ? AIDX(Apu, n, i, j) : 0.0;
        }
    }
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) AIDX(P, n, i, j) = 0.0;
    for (int i = 0; i < n; ++i) AIDX(P, n, i, Pvec[i]) = 1.0;
}

// Forward/back substitution for PAx=b using packed LU in A and permutation Pvec
void lup_solve(const double *A, int n, const int *Pvec, const double *b, double *x) {
    double *y = (double*)malloc(n * sizeof(double));
    // Forward: Ly = Pb
    for (int i = 0; i < n; ++i) {
        double sum = b[Pvec[i]];
        for (int j = 0; j < i; ++j) sum -= AIDX(A, n, i, j) * y[j];
        y[i] = sum; // L has unit diagonal
    }
    // Back: Ux = y
    for (int i = n - 1; i >= 0; --i) {
        double sum = y[i];
        for (int j = i + 1; j < n; ++j) sum -= AIDX(A, n, i, j) * x[j];
        x[i] = sum / AIDX(A, n, i, i);
    }
    free(y);
}

int main(void) {
    int n;
    printf("Enter the size of the matrix (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    double *A = (double*)malloc(n * n * sizeof(double));
    double *b = (double*)malloc(n * sizeof(double));
    if (!A || !b) { fprintf(stderr, "Allocation failed\n"); return 1; }

    printf("Enter the elements of matrix A (row by row):\n");
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        if (scanf("%lf", &AIDX(A, n, i, j)) != 1) { fprintf(stderr, "Bad input\n"); return 1; }
    }

    printf("Enter the elements of vector b: ");
    for (int i = 0; i < n; ++i) if (scanf("%lf", &b[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int *Pvec = (int*)malloc(n * sizeof(int));
    double tol = 1e-12;

    // Perform LUP decomposition
    if (!lup_decompose(A, n, Pvec, tol)) {
        fprintf(stderr, "Matrix appears singular (pivot < %.1e)\n", tol);
        free(A); free(b); free(Pvec);
        return 1;
    }

    // Extract L, U, and P
    double *L = (double*)malloc(n * n * sizeof(double));
    double *U = (double*)malloc(n * n * sizeof(double));
    double *P = (double*)malloc(n * n * sizeof(double));
    if (!L || !U || !P) { fprintf(stderr, "Allocation failed\n"); return 1; }

    extract_LUP(A, n, Pvec, L, U, P);

    // Print L, U, P matrices
    print_matrix("L", L, n);
    print_matrix("U", U, n);
    print_matrix("P", P, n);

    // Solve Ax = b
    double *x = (double*)malloc(n * sizeof(double));
    lup_solve(A, n, Pvec, b, x);

    // Print solution vector x
    printf("Solution x = [");
    for (int i = 0; i < n; ++i) {
        printf(" %.6f%s", x[i], (i + 1 < n) ? "," : " ");
    }
    printf("]\n");

    // Clean up allocated memory
    free(L); free(U); free(P); free(A); free(b); free(Pvec); free(x);
    return 0;
}
