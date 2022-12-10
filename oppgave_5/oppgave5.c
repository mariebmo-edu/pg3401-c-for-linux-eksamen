//
// Created by marie on 09.12.2022.
//
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "include/oppgave5.h"

#define BUFFER_SIZE 4096

struct threadData {
    char szDataBuffer[BUFFER_SIZE];
    int iUsedBytes;
};

struct threadAData {
    struct threadData *pThreadData;
    char *fileName;
    int fileProcessed;
};

struct threadBData {
    struct threadData *pThreadData;
    unsigned int bytesProcessed[256];
    int fileProcessed;
};

void *threadAFunction(void *arg);
void *threadBFunction(void *arg);

int main(int argc, char *argv[]) {


    struct threadData threadData;
    threadData.iUsedBytes = 0;

    struct threadAData threadAData;
    threadAData.pThreadData = &threadData;
    threadAData.fileName = "../dokumenter/PG3401-Hjemmeeksamen-14dager-H22.pdf";
    threadAData.fileProcessed = 0;

    struct threadBData threadBData;
    memset(threadBData.bytesProcessed, 0, 256*sizeof(unsigned int));
    threadBData.pThreadData = &threadData;
    threadBData.fileProcessed = 0;

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

    int i  = 0;
    while(i < 256){
        printf("%d: %i\n", i, threadBData.bytesProcessed[i]);
        i++;
    }

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
            pThreadAData->fileProcessed = true;
            break;
        }
        if (pThreadAData->pThreadData->iUsedBytes < BUFFER_SIZE) {
            pThreadAData->pThreadData->szDataBuffer[pThreadAData->pThreadData->iUsedBytes] = (char) c;
            pThreadAData->pThreadData->iUsedBytes++;
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
        printf("Thread B-%d: Processing data\n", i);
        if (pThreadBData->pThreadData->iUsedBytes && !pThreadBData->fileProcessed) {
            pThreadBData->bytesProcessed[pThreadBData->pThreadData->szDataBuffer[pThreadBData->pThreadData->iUsedBytes - 1]]++;
            pThreadBData->pThreadData->iUsedBytes--;
        }
        if (pThreadBData->fileProcessed) {
            break;
        }
        i++;
    }
    printf("Thread B: Done processing data\n");
    pthread_exit(NULL);
}

