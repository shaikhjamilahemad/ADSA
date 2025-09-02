#include <stdio.h>
#include <math.h>
#include <string.h>

#define SIZE 12


const char *names[SIZE] = {
    "1/n", "log2(n)", "12*sqrt(n)", "50*sqrt(n)",
    "n^0.51", "2^32 * n", "n*log2(n)", "100n^2+6n",
    "n^2-324", "2n^3", "n^(log2 n)", "3^n"
};


double calc(int idx, double n) {
    switch(idx) {
        case 0:  return 1.0 / n;                               // 1/n
        case 1:  return log(n) / log(2);                        // log2 n
        case 2:  return 12.0 * sqrt(n);                         // 12√n
        case 3:  return 50.0 * sqrt(n);                         // 50√n
        case 4:  return pow(n, 0.51);                           // n^0.51
        case 5:  return pow(2, 32) * n;                         // 2^32 * n
        case 6:  return n * (log(n) / log(2));                  // n log2 n
        case 7:  return 100.0 * n * n + 6.0 * n;                // 100n^2 + 6n
        case 8:  return n * n - 324.0;                          // n^2 - 324
        case 9:  return 2.0 * pow(n, 3);                        // 2n^3
        case 10: return pow(n, (log(n) / log(2)));              // n^(log2 n)
        case 11: return pow(3, n);                              // 3^n (will overflow for big n!)
    }
    return 0;
}


void swap_int(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

int main() {
    double n = 1e6;   
    int order[SIZE];

   
    for (int i = 0; i < SIZE; i++) order[i] = i;

    // Bubble sort based on function values
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            double v1 = calc(order[j], n);
            double v2 = calc(order[j+1], n);
            if (v1 > v2) swap_int(&order[j], &order[j+1]);
        }
    }

    printf("\n=== Functions Sorted by Growth ===\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%2d) %-15s\n", i + 1, names[order[i]]);
    }



    return 0;
}
