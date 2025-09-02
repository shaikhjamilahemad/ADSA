#include <stdio.h>
#include <stdlib.h>

/* Compare sum of two inclusive ranges:
   returns -1 if [a1..a2] is lighter, 1 if heavier, 0 if equal. */
int compareweigh(int arr[], int a1, int a2, int b1, int b2) {
    long long s1 = 0, s2 = 0;
    for (int i = a1; i <= a2; ++i) s1 += arr[i];
    for (int j = b1; j <= b2; ++j) s2 += arr[j];
    if (s1 < s2) return -1;
    if (s1 > s2) return  1;
    return 0;
}

/* Divide-and-conquer finder.
   Returns index of lighter (defective) coin, or -1 if none. */
int FindDefectiveCoin(int arr[], int low, int high) {
    int n = high - low + 1;

    if (n <= 0) return -1;

    if (n == 1) {
        /* If recursion isolates a single coin after an imbalance,
           it's the lighter one; we report it. */
        return low;
    }

    if (n == 2) {
        if (arr[low] < arr[high]) return low;
        if (arr[high] < arr[low]) return high;
        return -1; /* equal => no defective */
    }

    int mid = (low + high) / 2;

    if (n % 2 == 0) {
        /* Even: weigh equal halves [low..mid] vs [mid+1..high] */
        int cmp = compareweigh(arr, low, mid, mid + 1, high);
        if (cmp < 0)  return FindDefectiveCoin(arr, low, mid);
        if (cmp > 0)  return FindDefectiveCoin(arr, mid + 1, high);
        /* Balanced: no lighter coin in either half */
        return -1;
    } else {
        /* Odd: exclude the middle coin mid; weigh left vs right */
        int cmp = compareweigh(arr, low, mid - 1, mid + 1, high);
        if (cmp < 0)  return FindDefectiveCoin(arr, low, mid - 1);     /* left lighter */
        if (cmp > 0)  return FindDefectiveCoin(arr, mid + 1, high);    /* right lighter */

        /* Balanced: all except the middle are genuine.
           Check the middle against any certified genuine coin (e.g., low). */
        if (arr[mid] < arr[low]) return mid;
        return -1; /* middle also equal => no defective */
    }
}

int main(void) {
    int n;
    printf("Enter the size of array (No of Coins)\n");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid n\n");
        return 1;
    }

    int *coin = (int*)malloc((size_t)n * sizeof(int));
    if (!coin) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter the array elements (Weights of coins)\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &coin[i]) != 1) {
            fprintf(stderr, "Invalid weight input\n");
            free(coin);
            return 1;
        }
    }

    int index = FindDefectiveCoin(coin, 0, n - 1);

    if (index == -1) {
        printf("No Defective Coin found\n");
    } else {
        printf("Index of Defective Coin = %d and Weight = %d\n", index, coin[index]);
    }

    free(coin);
    return 0;
}
