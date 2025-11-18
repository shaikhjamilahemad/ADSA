#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define the dimension of the problem
#define N 2 // Fixed dimension for simplicity
#define MAX_ITER 1000
#define EPSILON 1e-6

// --- Linear Algebra Utility Functions (Same as before) ---

double dot_product(const double v1[N], const double v2[N]) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

void mat_vec_mult(const double Q[N][N], const double g[N], double result[N]) {
    for (int i = 0; i < N; i++) {
        result[i] = 0.0;
        for (int j = 0; j < N; j++) {
            result[i] += Q[i][j] * g[j];
        }
    }
}

void update_matrix(double Q[N][N], const double g[N], double alpha, const double Q_orig[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Q[i][j] = Q_orig[i][j] + alpha * g[i] * g[j];
        }
    }
}

void copy_matrix(const double src[N][N], double dest[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// --- Problem-Specific Function (The Feasible Region: Unit Square [0,1] x [0,1]) ---

int SeparationOracle(const double x[N], double g[N]) {
    // Constraint 1: x[0] >= 0 (i.e., -x[0] <= 0)
    if (x[0] < 0.0) {
        g[0] = -1.0; g[1] = 0.0;
        return 1;
    }
    // Constraint 2: x[0] <= 1
    if (x[0] > 1.0) {
        g[0] = 1.0; g[1] = 0.0;
        return 1;
    }
    // Constraint 3: x[1] >= 0
    if (x[1] < 0.0) {
        g[0] = 0.0; g[1] = -1.0;
        return 1;
    }
    // Constraint 4: x[1] <= 1
    if (x[1] > 1.0) {
        g[0] = 0.0; g[1] = 1.0;
        return 1;
    }

    return 0; // Solution found
}

// --- Ellipsoid Algorithm Core (Same as before) ---

void ellipsoid_algorithm(double x[N], double Q[N][N]) {
    double g[N];
    double d[N];
    double Qg[N];
    double g_T_Q_g;
    double Q_k[N][N];
    
    printf("--- Ellipsoid Algorithm Start ---\n");
    printf("Initial Center (x): (%.4f, %.4f)\n", x[0], x[1]);

    for (int k = 0; k < MAX_ITER; k++) {
        if (SeparationOracle(x, g) == 0) {
            printf("\n🎉 Feasible point found at iteration %d:\n", k);
            printf("x = (%.6f, %.6f)\n", x[0], x[1]);
            return;
        }

        mat_vec_mult(Q, g, Qg);
        g_T_Q_g = dot_product(g, Qg);

        if (g_T_Q_g <= EPSILON) {
            printf("\nStopping early (Degenerate ellipsoid or no progress) at iteration %d.\n", k);
            return;
        }
        
        double norm_factor = sqrt(g_T_Q_g);
        double factor_x = 1.0 / (N + 1.0);
        
        for (int i = 0; i < N; i++) {
            d[i] = Qg[i] / norm_factor;
            x[i] = x[i] - factor_x * d[i];
        }

        copy_matrix(Q, Q_k);
        
        double scalar_1 = N * N / (N * N - 1.0);
        double scalar_2 = -2.0 / (N + 1.0);
        
        update_matrix(Q, d, scalar_2, Q_k); 

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                Q[i][j] = scalar_1 * Q[i][j];
            }
        }
        
        if ((k + 1) % 100 == 0) {
            printf("Iteration %d: x = (%.4f, %.4f)\n", k + 1, x[0], x[1]);
        }
    }

    printf("\n❌ Maximum iterations reached (%d). Solution not found.\n", MAX_ITER);
}

// --- Main Function with User Input ---

int main() {
    double R; 
    double x_0[N]; 
    double Q_0[N][N] = {{0}};

    // 1. Get User Input for Initial Radius (R)
    printf("Enter the initial radius (R) for the ellipsoid (must be large enough to contain the solution): ");
    if (scanf("%lf", &R) != 1 || R <= 0) {
        printf("Invalid radius. Exiting.\n");
        return 1;
    }

    // 2. Get User Input for Initial Center (x_0)
    printf("Enter the initial center x[0] (e.g., 0.0): ");
    if (scanf("%lf", &x_0[0]) != 1) {
        printf("Invalid input for x[0]. Exiting.\n");
        return 1;
    }
    printf("Enter the initial center x[1] (e.g., 0.0): ");
    if (scanf("%lf", &x_0[1]) != 1) {
        printf("Invalid input for x[1]. Exiting.\n");
        return 1;
    }

    // 3. Initialize Ellipsoid Matrix Q_0 (R^2 * I)
    // Q_0 is a diagonal matrix where each diagonal element is R^2
    double R_squared = R * R;
    for (int i = 0; i < N; i++) {
        Q_0[i][i] = R_squared;
    }

    // Run the algorithm
    ellipsoid_algorithm(x_0, Q_0);

    return 0;
}