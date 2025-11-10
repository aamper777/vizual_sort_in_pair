#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "sorting_algorithms.h"

void ensure_data_dir() {
    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0777);
    }
}

// Глобальные переменные для логирования
static int frame_count = 0;
FILE *current_json_file = NULL;

// Функция для сохранения состояния массива
void save_array_state(int arr[], int size) {
    if (frame_count > 0) {
        fprintf(current_json_file, ",\n");
    }
    
    fprintf(current_json_file, "  [");
    for (int i = 0; i < size; i++) {
        fprintf(current_json_file, "%d", arr[i]);
        if (i < size - 1) fprintf(current_json_file, ", ");
    }
    fprintf(current_json_file, "]");
    
    frame_count++;
}

// Функция для копирования массива
void copy_array(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

// Модифицированные функции сортировки с логированием

void bubble_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    int x = 0, y = N - 1; 
    int k = 1;  
    
    while ((x < y) && k > 0) {
        k = 0;
        for (int i = x; i < y; i++) {
            if (arr[i] > arr[i+1]) {
                int t = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = t;
                k = 1;
                save_array_state(arr, N);
            }
        }
        y--;
        for (int i = y; i > x; i--) {
            if (arr[i-1] > arr[i]) {
                int t = arr[i];
                arr[i] = arr[i-1];
                arr[i-1] = t;
                k = 1;
                save_array_state(arr, N);
            }
        }
        x++;
        if(k == 0) break;
    }
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

void selection_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    for (int i = 0; i < N-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < N; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            save_array_state(arr, N);
        }
    }
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

void insertion_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    for (int i = 1; i < N; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            save_array_state(arr, N);
        }
        arr[j + 1] = key;
        save_array_state(arr, N);
    }
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

void shaker_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    int left = 0;
    int right = N - 1;
    int swapped = 1;
    
    while (left < right && swapped) {
        swapped = 0;
        
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
                save_array_state(arr, N);
            }
        }
        right--;
        
        for (int i = right; i > left; i--) {
            if (arr[i - 1] > arr[i]) {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                swapped = 1;
                save_array_state(arr, N);
            }
        }
        left++;
    }
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

// Объявление вспомогательных функций
void merge_sort_with_logging(int *arr, int left, int right, int full_size);
void heapify_logging(int *a, int N, int i, int full_size);

void merge_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    // Улучшенная версия с логированием всех слияний
    merge_sort_with_logging(arr, 0, N - 1, N);
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

// Вспомогательная функция для merge sort с полным логированием
void merge_sort_with_logging(int *arr, int left, int right, int full_size) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Рекурсивная сортировка левой и правой частей
        merge_sort_with_logging(arr, left, mid, full_size);
        merge_sort_with_logging(arr, mid + 1, right, full_size);
        
        // Слияние отсортированных частей
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        int *left_arr = malloc(n1 * sizeof(int));
        int *right_arr = malloc(n2 * sizeof(int));
        
        for (int i = 0; i < n1; i++)
            left_arr[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            right_arr[j] = arr[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if (left_arr[i] <= right_arr[j]) {
                arr[k++] = left_arr[i++];
            } else {
                arr[k++] = right_arr[j++];
            }
        }
        
        while (i < n1) {
            arr[k++] = left_arr[i++];
        }
        
        while (j < n2) {
            arr[k++] = right_arr[j++];
        }
        
        // Логируем состояние после завершения слияния
        save_array_state(arr, full_size);
        
        free(left_arr);
        free(right_arr);
    }
}

// Вспомогательная функция для heapify с логированием
void heapify_logging(int *a, int N, int i, int full_size) {
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
        save_array_state(a, full_size);
        
        heapify_logging(a, N, largest, full_size);
    }
}

void heap_sort_logging(int *original, int N) {
    int *arr = malloc(N * sizeof(int));
    copy_array(original, arr, N);
    
    frame_count = 0;
    fprintf(current_json_file, "[\n");
    save_array_state(arr, N);
    
    // Построение кучи (переупорядочивание массива)
    for (int i = N / 2 - 1; i >= 0; i--) {
        heapify_logging(arr, N, i, N);
    }
    
    // Извлечение элементов из кучи по одному
    for (int i = N - 1; i >= 0; i--) {
        // Перемещаем текущий корень в конец
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        save_array_state(arr, N);
        
        // Вызываем heapify на уменьшенной куче
        heapify_logging(arr, i, 0, N);
    }
    
    fprintf(current_json_file, "\n]\n");
    free(arr);
}

int main() {
    ensure_data_dir();
    
    int size = 45;
    int *original = malloc(size * sizeof(int));
    
    srand(time(NULL));
    printf("Создаем массив из %d элементов:\n", size);
    for (int i = 0; i < size; i++) {
        original[i] = rand() % 1000 + 1;
    }

        
    printf("...\n\n");
    
    char *filenames[] = {
        "data/bubble_sort.json",
        "data/selection_sort.json", 
        "data/insertion_sort.json",
        "data/shaker_sort.json",
        "data/merge_sort.json",
        "data/heap_sort.json"
    };


    for (int i = 0; i < 6; i++) {
        current_json_file = fopen(filenames[i], "w");
        if (current_json_file == NULL) {
            printf("Ошибка создания файла: %s\n", filenames[i]);
            continue;
        }
        
        printf("Генерируем %s...\n", filenames[i]);
        
        switch(i) {
            case 0: bubble_sort_logging(original, size); break;
            case 1: selection_sort_logging(original, size); break;
            case 2: insertion_sort_logging(original, size); break;
            case 3: shaker_sort_logging(original, size); break;
            case 4: merge_sort_logging(original, size); break;
            case 5: heap_sort_logging(original, size); break;
        }
        
        fclose(current_json_file);
        printf("✓ Файл создан: %s\n", filenames[i]);
    }
    
    free(original);
    printf("\nВсе JSON файлы созданы!\n");
    return 0;
}