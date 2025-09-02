#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Swap two long long values */
static inline void swap_ll(long long *a, long long *b) {
    long long t = *a; *a = *b; *b = t;
}

/* Sift a[i] down the max-heap a[0..n-1] */
static void sift_down(long long *a, size_t n, size_t i) {
    while (1) {
        size_t left = 2*i + 1;
        if (left >= n) break;                       // no children
        size_t right = left + 1;
        size_t largest = (right < n && a[right] > a[left]) ? right : left;
        if (a[i] >= a[largest]) break;              // heap property ok
        swap_ll(&a[i], &a[largest]);                // push down
        i = largest;
    }
}

/* In-place heapsort (ascending) */
void heapsort(long long *a, size_t n) {
    if (n < 2) return;

    // Build max-heap in O(n)
    for (size_t i = (n - 2) / 2 + 1; i > 0; ) {
        --i;
        sift_down(a, n, i);
    }

    // Extract max one-by-one
    for (size_t end = n; end > 1; ) {
        --end;                      // new last index
        swap_ll(&a[0], &a[end]);    // move max to its final place
        sift_down(a, end, 0);       // restore heap on a[0..end-1]
    }
}

/* Helper to print an array */
static void print_array(const long long *a, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (i) putchar(' ');
        printf("%lld", a[i]);
    }
    putchar('\n');
}

int main(void) {
    /* Hardcoded array — change values freely */
    long long arr[] = { 5, -3, 7, -1, 0, -2, 8, -5, 8, 5 };
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    print_array(arr, n);

    heapsort(arr, n);

    printf("Sorted  : ");
    print_array(arr, n);

    return 0;
}
