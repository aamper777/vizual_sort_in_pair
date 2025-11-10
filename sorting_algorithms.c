#include "sorting_algorithms.h"

// 1. Bubble Sort
void bubble_sort(int *a, int N){
    int x = 0, y = N - 1; 
    int k = 1;  
    while ((x < y) && k > 0) {
        k = 0;
        for (int i = x; i < y; i++){
            if (a[i] > a[i+1]){
                int t = a[i];
                a[i] = a[i+1];
                a[i+1] = t;
                k = 1;      
            }
        }
        y--; 
        for (int i = y; i > x; i--){
            if (a[i-1] > a[i]){
                int t = a[i];
                a[i] = a[i-1];
                a[i-1] = t;
                k = 1;   
            }
        }
        x++;
        if(k == 0) break;
    }
    return;
}

// 2. Selection Sort
void selection_sort(int *a, int N) {
    for (int i = 0; i < N-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < N; j++) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        int temp = a[i];
        a[i] = a[min_idx];
        a[min_idx] = temp;
    }
    return;
}

// 3. Insertion Sort
void insertion_sort(int *a, int N) {
    for (int i = 1; i < N; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = key;
    }
    return;
}

// 4. Shaker Sort
void shaker_sort(int *a, int N) {
    int left = 0;
    int right = N - 1;
    int swapped = 1;
    
    while (left < right && swapped) {
        swapped = 0;
        
        // Going forward 
        for (int i = left; i < right; i++) {
            if (a[i] > a[i + 1]) {
                int temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
                swapped = 1;
            }
        }
        right--;
        
        // Going back
        for (int i = right; i > left; i--) {
            if (a[i - 1] > a[i]) {
                int temp = a[i];
                a[i] = a[i - 1];
                a[i - 1] = temp;
                swapped = 1;
            }
        }
        left++;
    }
    return;
}

// 5. Merge Sort
void merge_sort(int *a, int N) {
    if (N <= 1) return;
    
    int mid = N / 2;
    int n1 = mid;
    int n2 = N - mid;
    
    // Spliting our massive
    merge_sort(a, n1);
    merge_sort(a + mid, n2);
    
    // Merging sorted parts
    int *temp = (int*)malloc(N * sizeof(int));
    int i = 0, j = 0, k = 0;
    
    while (i < n1 && j < n2) {
        if (a[i] <= a[mid + j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[mid + j++];
        }
    }
    
    while (i < n1) temp[k++] = a[i++];
    while (j < n2) temp[k++] = a[mid + j++];
    
    // Copy in our original massive
    for (i = 0; i < N; i++) {
        a[i] = temp[i];
    }
    
    free(temp);
}


// 6. Heap Sort
void heapify(int *a, int N, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < N && a[left] > a[largest])
        largest = left;
    
    if (right < N && a[right] > a[largest])
        largest = right;
    
    if (largest != i) {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        
        heapify(a, N, largest);
    }
    return;
}

void heap_sort(int *a, int N) {
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(a, N, i);
    
    for (int i = N - 1; i >= 0; i--) {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        
        heapify(a, i, 0);
    }
    return;
}