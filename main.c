/*
* file: tSums.c
 * author: Chandler Radcliffe
 * course: CSI 3336
 * assignment: project 9
 * due date: 12/7/25
 *
 * date modified: 12/3/25
 *      file created and logic added
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
 * global count variable to add numbers in the file
 * mutex to block when adding to count
 */
int count = 0;
sem_t mutex;

/*
 * thread func header for threads
 */
void *threadFunc(void *args);

/**
 * struct for passing filename, offset and count to thread
 */
struct args {
    char *filename;
    int offset;
    int count;
};

int main(int argc, char *argv[]) {
    char *file;
    int numChild, i;
    FILE *fp;
    pthread_t *threads;
    int size, chunk, leftover, total;
    struct args *args;

    int childCount;

    if (argc != 3) {
        write(2,"Usage: ./a.out <filename> <number>\n", 50);
        exit(1);
    }

    file = argv[1];
    numChild = atoi(argv[2]);
    /* create array of arg structs, 1 for each thread*/
    args = malloc(sizeof(struct args)* numChild);

    /* initalize mutex*/
    sem_init(&mutex, 0, 1);

    /* create array of threads*/
    threads = malloc(sizeof(pthread_t) * numChild);

    fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    /*
     * calculate size of file
     * get chunk size and leftover amount to pass to each thread
     */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    total = size / sizeof(int);
    chunk = total / numChild;
    leftover = total % numChild;

    for (i=0; i < numChild; i++) {
        /*
         * assign values to pass to thread
         * pass filename, offset, and count to each thread
         */
        args[i].filename = file;
        args[i].offset = chunk * i * sizeof(int);

        if (i == numChild - 1) {
            childCount = chunk + leftover;
        }
        else {
            childCount = chunk;
        }
        args[i].count = childCount;

        /* Create thread */
        if (pthread_create(threads + i, NULL, &threadFunc, ((void *)&args[i])) != 0){
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }
    /* join on each thread */
    for (i=0; i < numChild; i++) {
        pthread_join(threads[i], NULL);
    }

    /* caclulate total and free used memory */
    printf("Calculated total: %d\n", count);

    sem_destroy(&mutex);
    free(threads);
    free(args);
    fclose(fp);
    return 0;
}

/**
 * thread func to calculate total in each chunk
 * uses args passed to iterate through its portion
 * of the file and add to global total
 * @param args
 * @return void *
 */
void *threadFunc(void *args) {
    struct args *fargs = (struct args *)args;
    char *filename = fargs->filename;
    FILE *fp2;
    int found, j;
    fp2 = fopen(filename, "r");
    fseek(fp2, fargs->offset, SEEK_SET);
    for (j=0; j < fargs->count; j++) {
        fread(&found, 1, sizeof(int), fp2);
        /* Mutex to block from race condition */
        sem_wait(&mutex);
        count += found;
        sem_post(&mutex);
    }
    /* close file */
    fclose(fp2);
    pthread_exit(NULL);
}