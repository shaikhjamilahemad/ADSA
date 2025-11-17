#include <stdio.h>

// Recursive Binary Search
int binarySearchRecursive(int arr[], int low, int high, int key) {
    if (low > high) {
        return -1;
    }

    int mid = (low + high) / 2;

    if (arr[mid] == key) {
        return mid;
    }
    else if (arr[mid] < key) {
        return binarySearchRecursive(arr, mid + 1, high, key);
    }
    else {
        return binarySearchRecursive(arr, low, mid - 1, key);
    }
}

int main() {
    int arr[] = {3, 6, 9, 15, 20, 36, 42, 57, 73, 88};
    int n = sizeof(arr) / sizeof(arr[0]);
    int key = 15;

    int result = binarySearchRecursive(arr, 0, n - 1, key);

    printf("Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nSearching for key = %d\n", key);

    if (result != -1) {
        printf("Key %d found at index %d.\n", key, result);
    } else {
        printf("Key %d not found in the array.\n", key);
    }

    return 0;
}
