#include <stdio.h>

// Swapping function
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Choose median as pivot
int medianOfThree(int arr[], int low, int high) {
    int mid = (low + high) / 2;

    // Order the three elements
    if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high]) swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);

    // Now arr[mid] is median
    // Swap with last-1 and use as pivot
    swap(&arr[mid], &arr[high - 1]);
    return arr[high - 1];
}

// Partitioning using median pivot
int partition(int arr[], int low, int high) {
    int pivot = medianOfThree(arr, low, high);
    int i = low;
    int j = high - 1;

    while (1) {
        while (arr[++i] < pivot) {}
        while (arr[--j] > pivot) {}
        if (i < j) {
            swap(&arr[i], &arr[j]);
        } else {
            break;
        }
    }

    swap(&arr[i], &arr[high - 1]);
    return i;
}

// Quick Sort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int arr[] = {34, 7, 23, 32, 5, 62, 19, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    quickSort(arr, 0, n - 1);

    printf("\n\nSorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
