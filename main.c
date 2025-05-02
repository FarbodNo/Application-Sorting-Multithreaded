#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 20

int array[SIZE];
int sorted[SIZE];

// Struct to pass start and end indices to sorting threads
typedef struct {
    int start;
    int end;
} Range;

void* bubble_sort(void* arg) {
    Range* r = (Range*) arg;
    for (int i = r->start; i < r->end - 1; i++) {
        for (int j = r->start; j < r->end - 1 - (i - r->start); j++) {
            if (array[j] > array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
    pthread_exit(NULL);
}

void* merge(void* arg) {
    int i = 0, j = SIZE / 2, k = 0;
    while (i < SIZE / 2 && j < SIZE) {
        if (array[i] < array[j])
            sorted[k++] = array[i++];
        else
            sorted[k++] = array[j++];
    }
    while (i < SIZE / 2)
        sorted[k++] = array[i++];
    while (j < SIZE)
        sorted[k++] = array[j++];
    pthread_exit(NULL);
}

int main() {
    printf("Enter 20 numbers:\n");
    for (int i = 0; i < SIZE; i++) {
        scanf("%d", &array[i]);
    }

    pthread_t t1, t2, t_merge;

    // Thread 1: sort first half
    Range* r1 = malloc(sizeof(Range));
    r1->start = 0;
    r1->end = SIZE / 2;
    pthread_create(&t1, NULL, bubble_sort, r1);

    // Thread 2: sort second half
    Range* r2 = malloc(sizeof(Range));
    r2->start = SIZE / 2;
    r2->end = SIZE;
    pthread_create(&t2, NULL, bubble_sort, r2);

    // Wait for sorting threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    free(r1);
    free(r2);

    // Thread 3: merge
    pthread_create(&t_merge, NULL, merge, NULL);
    pthread_join(t_merge, NULL);

    // Main thread prints result
    printf("Sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    return 0;
}
