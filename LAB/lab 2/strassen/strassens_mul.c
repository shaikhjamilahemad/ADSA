#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

/* ---------- helpers: matrix create/print/free ---------- */

static ll **create_matrix(size_t r, size_t c, int zero_init) {
    ll **m = (ll **)malloc(r * sizeof(ll *));
    if (!m) { fprintf(stderr, "alloc fail\n"); exit(1); }
    for (size_t i = 0; i < r; ++i) {
        m[i] = (ll *) (zero_init ? calloc(c, sizeof(ll)) : malloc(c * sizeof(ll)));
        if (!m[i]) { fprintf(stderr, "alloc fail\n"); exit(1); }
    }
    return m;
}

static void free_matrix(ll **m, size_t r) {
    if (!m) return;
    for (size_t i = 0; i < r; ++i) free(m[i]);
    free(m);
}

static void print_matrix(ll **m, size_t r, size_t c) {
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) printf("%4lld ", m[i][j]);
        printf("\n");
    }
}

/* ---------- basic ops on k x k blocks ---------- */

static void add_block(ll **A, ll **B, ll **C, size_t k) {
    for (size_t i = 0; i < k; ++i)
        for (size_t j = 0; j < k; ++j)
            C[i][j] = A[i][j] + B[i][j];
}

static void sub_block(ll **A, ll **B, ll **C, size_t k) {
    for (size_t i = 0; i < k; ++i)
        for (size_t j = 0; j < k; ++j)
            C[i][j] = A[i][j] - B[i][j];
}

static void naive_mul_block(ll **A, ll **B, ll **C, size_t k) {
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            ll s = 0;
            for (size_t t = 0; t < k; ++t) s += A[i][t] * B[t][j];
            C[i][j] = s;
        }
    }
}

/* Copy a kxk sub-block from src starting at (r0,c0) into dst[0..k-1][0..k-1] */
static void copy_block(ll **src, size_t r0, size_t c0, ll **dst, size_t k) {
    for (size_t i = 0; i < k; ++i)
        memcpy(dst[i], &src[r0 + i][c0], k * sizeof(ll));
}

/* Write a kxk block from src into dst at (r0,c0) */
static void write_block(ll **src, ll **dst, size_t r0, size_t c0, size_t k) {
    for (size_t i = 0; i < k; ++i)
        memcpy(&dst[r0 + i][c0], src[i], k * sizeof(ll));
}

/* ---------- Strassen recursive ---------- */

static void strassen(ll **A, ll **B, ll **C, size_t n) {
    // threshold where naive wins (tune: 32~128 typical)
    if (n <= 64) { naive_mul_block(A, B, C, n); return; }

    size_t k = n / 2;

    // allocate subblocks
    ll **A11 = create_matrix(k, k, 0), **A12 = create_matrix(k, k, 0);
    ll **A21 = create_matrix(k, k, 0), **A22 = create_matrix(k, k, 0);
    ll **B11 = create_matrix(k, k, 0), **B12 = create_matrix(k, k, 0);
    ll **B21 = create_matrix(k, k, 0), **B22 = create_matrix(k, k, 0);

    // slice A, B into quadrants
    copy_block(A, 0,   0,   A11, k);
    copy_block(A, 0,   k,   A12, k);
    copy_block(A, k,   0,   A21, k);
    copy_block(A, k,   k,   A22, k);

    copy_block(B, 0,   0,   B11, k);
    copy_block(B, 0,   k,   B12, k);
    copy_block(B, k,   0,   B21, k);
    copy_block(B, k,   k,   B22, k);

    // Temps
    ll **M1 = create_matrix(k, k, 1), **M2 = create_matrix(k, k, 1);
    ll **M3 = create_matrix(k, k, 1), **M4 = create_matrix(k, k, 1);
    ll **M5 = create_matrix(k, k, 1), **M6 = create_matrix(k, k, 1);
    ll **M7 = create_matrix(k, k, 1);

    ll **T1 = create_matrix(k, k, 1), **T2 = create_matrix(k, k, 1);

    // M1 = (A11 + A22) * (B11 + B22)
    add_block(A11, A22, T1, k);
    add_block(B11, B22, T2, k);
    strassen(T1, T2, M1, k);

    // M2 = (A21 + A22) * B11
    add_block(A21, A22, T1, k);
    strassen(T1, B11, M2, k);

    // M3 = A11 * (B12 - B22)
    sub_block(B12, B22, T2, k);
    strassen(A11, T2, M3, k);

    // M4 = A22 * (B21 - B11)
    sub_block(B21, B11, T2, k);
    strassen(A22, T2, M4, k);

    // M5 = (A11 + A12) * B22
    add_block(A11, A12, T1, k);
    strassen(T1, B22, M5, k);

    // M6 = (A21 - A11) * (B11 + B12)
    sub_block(A21, A11, T1, k);
    add_block(B11, B12, T2, k);
    strassen(T1, T2, M6, k);

    // M7 = (A12 - A22) * (B21 + B22)
    sub_block(A12, A22, T1, k);
    add_block(B21, B22, T2, k);
    strassen(T1, T2, M7, k);

    // C11 = M1 + M4 - M5 + M7
    ll **C11 = create_matrix(k, k, 1);
    ll **C12 = create_matrix(k, k, 1);
    ll **C21 = create_matrix(k, k, 1);
    ll **C22 = create_matrix(k, k, 1);

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < k; ++j) {
            C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C12[i][j] = M3[i][j] + M5[i][j];
            C21[i][j] = M2[i][j] + M4[i][j];
            C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    // stitch C quadrants into C
    write_block(C11, C, 0, 0, k);
    write_block(C12, C, 0, k, k);
    write_block(C21, C, k, 0, k);
    write_block(C22, C, k, k, k);

    // free
    free_matrix(A11, k); free_matrix(A12, k); free_matrix(A21, k); free_matrix(A22, k);
    free_matrix(B11, k); free_matrix(B12, k); free_matrix(B21, k); free_matrix(B22, k);
    free_matrix(M1, k);  free_matrix(M2, k);  free_matrix(M3, k);  free_matrix(M4, k);
    free_matrix(M5, k);  free_matrix(M6, k);  free_matrix(M7, k);
    free_matrix(T1, k);  free_matrix(T2, k);
    free_matrix(C11, k); free_matrix(C12, k); free_matrix(C21, k); free_matrix(C22, k);
}

/* ---------- utilities ---------- */

static size_t next_pow2(size_t x) {
    size_t n = 1;
    while (n < x) n <<= 1;
    return n;
}

/* ---------- main: read, pad, compute, trim/print ---------- */

int main(void) {
    size_t r1, c1, r2, c2;

    printf("Enter rows and cols for Matrix A: ");
    if (scanf("%zu%zu", &r1, &c1) != 2) return 0;
    printf("Enter rows and cols for Matrix B: ");
    if (scanf("%zu%zu", &r2, &c2) != 2) return 0;

    // Determine padded size: handle non-square + incompatibility
    size_t max_dim = r1;
    if (c1 > max_dim) max_dim = c1;
    if (r2 > max_dim) max_dim = r2;
    if (c2 > max_dim) max_dim = c2;

    size_t n = next_pow2(max_dim);  // Strassen prefers power-of-two

    // allocate padded A,B,C
    ll **A = create_matrix(n, n, 1);
    ll **B = create_matrix(n, n, 1);
    ll **C = create_matrix(n, n, 1);

    // read actual A, B into top-left corners
    printf("\nEnter Matrix A (%zux%zu):\n", r1, c1);
    for (size_t i = 0; i < r1; ++i)
        for (size_t j = 0; j < c1; ++j)
            scanf("%lld", &A[i][j]);

    printf("\nEnter Matrix B (%zux%zu):\n", r2, c2);
    for (size_t i = 0; i < r2; ++i)
        for (size_t j = 0; j < c2; ++j)
            scanf("%lld", &B[i][j]);

    // Do Strassen on padded matrices
    strassen(A, B, C, n);

    // Print only the meaningful r1 x c2 top-left block
    printf("\nResultant Matrix (%zux%zu):\n", r1, c2);
    print_matrix(C, r1, c2);

    // cleanup
    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C, n);
    return 0;
}
