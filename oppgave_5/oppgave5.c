#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/oppgave5.h"

#define BUFFER_SIZE 4096

struct threadData {
    char szDataBuffer[BUFFER_SIZE];
    int iUsedBytes;
    bool fileProcessed;
    pthread_mutex_t mutex;
};

struct threadAData {
    struct threadData *pThreadData;
    char *fileName;
};

struct threadBData {
    struct threadData *pThreadData;
    unsigned int bytesProcessed[256];
};

void *threadAFunction(void *arg);
void *threadBFunction(void *arg);

int main(int argc, char *argv[]) {

    // Initialize the threadData structure and its mutex
    struct threadData threadData;
    threadData.iUsedBytes = 0;
    threadData.fileProcessed = false;
    pthread_mutex_init(&threadData.mutex, NULL);

    // Pass a pointer to the threadData structure to the thread functions
    struct threadAData threadAData;
    threadAData.pThreadData = &threadData;
    threadAData.fileName = "../dokumenter/PG3401-Hjemmeeksamen-14dager-H22.pdf";

    struct threadBData threadBData;
    memset(threadBData.bytesProcessed, 0, 256*sizeof(unsigned int));
    threadBData.pThreadData = &threadData;

    pthread_t threadA, threadB;
    if (pthread_create(&threadA, NULL, threadAFunction, &threadAData) != 0) {
        perror("Failed to create thread A");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&threadB, NULL, threadBFunction, &threadBData) != 0) {
        perror("Failed to create thread B");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(threadA, NULL) != 0) {
        perror("Failed to join thread A");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Failed to join thread B");
        exit(EXIT_FAILURE);
    }

    printf("FINISHED! Character count: \n");

    int i  = 0;
    while(i < 256){
        printf("%02x: %i\n", i, threadBData.bytesProcessed[i]);
        i++;
    }

    // Destroy the mutex
    pthread_mutex_destroy(&threadData.mutex);

    return 1;
}

void *threadAFunction(void *arg) {

    struct threadAData *pThreadAData = (struct threadAData *) arg;

    FILE *file = fopen(pThreadAData->fileName, "r");

    if (file == NULL) {
        printf("Failed to open file for reading\n");
        pthread_exit(NULL);
    }

    int i = 0;
    while (true) {
        printf("Thread A-%d: Reading file\n", i);
        int c = fgetc(file);
        if (c == EOF) {
            // Set the fileProcessed flag to true to indicate that the file has been read
            pthread_mutex_lock(&pThreadAData->pThreadData->mutex);
            pThreadAData->pThreadData->fileProcessed = true;
            pthread_mutex_unlock(&pThreadAData->pThreadData->mutex);
            break;
        }
        if (pThreadAData->pThreadData->iUsedBytes < BUFFER_SIZE) {
            // Lock the mutex before modifying the shared data
            pthread_mutex_lock(&pThreadAData->pThreadData->mutex);
            pThreadAData->pThreadData->szDataBuffer[pThreadAData->pThreadData->iUsedBytes] = (char) c;
            pThreadAData->pThreadData->iUsedBytes++;
            pthread_mutex_unlock(&pThreadAData->pThreadData->mutex);
        }
        i++;
    }

    fclose(file);

    printf("Thread A: File read\n");
    pthread_exit(NULL);
}

void *threadBFunction(void *arg) {

    struct threadBData *pThreadBData = (struct threadBData *) arg;

    int i = 0;
    while (true) {
        // Check the fileProcessed flag and exit the loop if it is true
        if (pThreadBData->pThreadData->fileProcessed) {
            break;
        }

        printf("Thread B-%d: Processing data\n", i);
        // Lock the mutex before reading from the shared buffer
        pthread_mutex_lock(&pThreadBData->pThreadData->mutex);

        if (pThreadBData->pThreadData->iUsedBytes > 0) {
            char c = pThreadBData->pThreadData->szDataBuffer[pThreadBData->pThreadData->iUsedBytes - 1];
            pThreadBData->bytesProcessed[(unsigned char)c]++;
            pThreadBData->pThreadData->iUsedBytes--;
            pthread_mutex_unlock(&pThreadBData->pThreadData->mutex);
        } else {
            pthread_mutex_unlock(&pThreadBData->pThreadData->mutex);
            // If the buffer is empty, sleep for a short time to avoid busy waiting
            usleep(1000);
        }
        i++;
    }

    printf("Thread B: File processed\n");
    pthread_exit(NULL);
}