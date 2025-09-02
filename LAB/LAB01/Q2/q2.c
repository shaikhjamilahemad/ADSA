#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

int binary_search(const int *arr, int size, int target, ll *count) {
    int start = 0, end = size - 1;
    while (start <= end) {
        int mid = start + ((end - start) >> 1);
        (*count)++;
        if (target < arr[mid]) end = mid - 1;
        else if (target > arr[mid]) start = mid + 1;
        else return mid;
    }
    return -1;
}

int ternary_search(const int *arr, int size, int target, ll *count) {
    int start = 0, end = size - 1;
    while (start <= end) {
        int gap = (end - start) / 3;
        int m1 = start + gap, m2 = end - gap;
        (*count)++; if (target == arr[m1]) return m1;
        (*count)++; if (target == arr[m2]) return m2;
        if (target < arr[m1]) end = m1 - 1;
        else if (target > arr[m2]) start = m2 + 1;
        else { start = m1 + 1; end = m2 - 1; }
    }
    return -1;
}

int main(void) {
    printf("%-8s  %-10s  %-10s\n", "n", "binary", "ternary");

    for (int size = 1024; size <= 1 << 20; size <<= 1) {
        int *arr = malloc((size_t)size * sizeof *arr);
        for (int i = 0; i < size; ++i) arr[i] = i * 2;
        int target = -1;
        ll bc = 0, tc = 0;

        binary_search(arr, size, target, &bc);
        ternary_search(arr, size, target, &tc);

        printf("%-8d  %-10lld  %-10lld\n", size, bc, tc);
        free(arr);
    }
    return 0;
}
