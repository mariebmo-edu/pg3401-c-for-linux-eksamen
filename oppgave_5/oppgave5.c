#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/oppgave5.h"

#define BUFFER_SIZE 4096

// Data som er tilgjengelig for alle tråder.
// szBuffer er en buffer som brukes til å lagre data som leses av tråd A fra filen, og hentes av tråd B som teller ganger en byte forekommer.
// iUsedBytes er antall bytes som til en hver tid er lagret i bufferen. Når A legger til en byte går den opp, og når B henter en byte går den ned.
// fileProcessed er en bool som forteller om filen er ferdig lest av tråd A.
// mutex brukes til å sikre at kun en tråd kan lese eller skrive til bufferen samtidig. Når en tråd skal lese eller skrive til bufferen, må den låse. Når den er ferdig, må den låse opp igjen.
struct threadData {
    char szDataBuffer[BUFFER_SIZE];
    int iUsedBytes;
    bool fileProcessed;
    pthread_mutex_t mutex;
};

// Struct for tråden som leser fra fil.
// pThreadData er en peker til structen som inneholder data som er tilgjengelig for alle tråder.
// filename er filnavnet som skal leses.
struct threadAData {
    struct threadData *pThreadData;
    char *fileName;
};

// Struct for tråden som teller antall ganger en byte forekommer.
// pThreadData er en peker til structen som inneholder data som er tilgjengelig for alle tråder.
// bytesProcessed er en array som holder telling på antall ganger en byte forekommer. bytesProcessed[0] er antall ganger 0x00 forekommer, bytesProcessed[1] er antall ganger 0x01 forekommer osv.
struct threadBData {
    struct threadData *pThreadData;
    unsigned int bytesProcessed[256];
};

int main(int argc, char *argv[]) {

    // Lag en struct for å holde data som er tilgjengelig for alle tråder. Initierer mutex for å sikre at kun en tråd kan skrive til bufferen om gangen.
    struct threadData threadData;
    threadData.iUsedBytes = 0;
    threadData.fileProcessed = false;
    pthread_mutex_init(&threadData.mutex, NULL);

    // Initierer struct for tråd A, og gir den en peker til structen som er tilgjengelig for alle tråder - samt hardkoder filnavnet (Dette kan endres ved å sende inn filnavn som argument om ønskelig).
    struct threadAData threadAData;
    threadAData.pThreadData = &threadData;
    threadAData.fileName = "../dokumenter/PG3401-Hjemmeeksamen-14dager-H22.pdf";

    // Initierer struct for tråd B, og gir den en peker til structen som er tilgjengelig for alle tråder, samt lager en array for å holde antall ganger en byte forekommer.
    struct threadBData threadBData;
    memset(threadBData.bytesProcessed, 0, 256*sizeof(unsigned int));
    threadBData.pThreadData = &threadData;

    // Oppretter tråd A og B.
    pthread_t threadA, threadB;
    if (pthread_create(&threadA, NULL, threadAFunction, &threadAData) != 0) {
        perror("Failed to create thread A");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&threadB, NULL, threadBFunction, &threadBData) != 0) {
        perror("Failed to create thread B");
        exit(EXIT_FAILURE);
    }

    // Venter på at tråd A og B skal fullføre.
    if (pthread_join(threadA, NULL) != 0) {
        perror("Failed to join thread A");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(threadB, NULL) != 0) {
        perror("Failed to join thread B");
        exit(EXIT_FAILURE);
    }

    // Skriver ut antall ganger hver byte forekom.
    printf("FINISHED! Character count: \n");

    int i  = 0;
    while(i < 256){
        printf("%02x (%c): %d\n", i, i, threadBData.bytesProcessed[i]);
        i++;
    }

    // Avslutter mutex, og avslutter programmet.
    pthread_mutex_destroy(&threadData.mutex);

    return 0;
}

void* threadAFunction(void* pData) {
    // Henter ut data fra structen som ble sendt inn.
    struct threadAData *pThreadAData = (struct threadAData*) pData;

    // Åpner filen i read binary modus. Hvis filen ikke finnes, avsluttes tråden.
    FILE* pFile = fopen(pThreadAData->fileName, "rb");
    if (pFile == NULL) {
        perror("Failed to open file");
        pthread_exit(NULL);
    }

    // Leser filen inn i bufferen i structen som er tilgjengelig for alle tråder (Sikres med mutex). Hvis filen er større enn bufferen, leses kun BUFFER_SIZE bytes om gangen.
    // Hvis bytesRead er mindre enn BUFFER_SIZE, er filen ferdig lest.
    int bytesRead = 0;
    do {
        // Låser mutex for å sikre at kun en tråd kan skrive til bufferen om gangen.
        pthread_mutex_lock(&pThreadAData->pThreadData->mutex);

        // Leser fra filen og legger opp til BUFFER_SIZE bytes i bufferen.
        bytesRead = fread(pThreadAData->pThreadData->szDataBuffer, 1, BUFFER_SIZE, pFile);
        pThreadAData->pThreadData->iUsedBytes = bytesRead;

        // Låser opp mutex.
        pthread_mutex_unlock(&pThreadAData->pThreadData->mutex);

        // Venter i 1 millisekund for å gi tråd B tid til å lese fra bufferen.
        usleep(1000);
    } while (bytesRead == BUFFER_SIZE);

    // Stenger filen.
    fclose(pFile);

    // Setter filen til å være "ferdig behandlet". Dette gjør at tråd B vet at den ikke trenger å vente på mer data når bufferen er tom.
    pThreadAData->pThreadData->fileProcessed = true;

    pthread_exit(NULL);
}

// Tråd B leser fra bufferen i structen som er tilgjengelig for alle tråder, og teller antall ganger hver byte forekommer.
void* threadBFunction(void* pData) {
    // Henter ut data fra structen som ble sendt inn.
    struct threadBData *pThreadBData = (struct threadBData*) pData;

    // Leser fra bufferen i structen som er tilgjengelig for alle tråder (Sikres med mutex) til filen er behandlet og bufferen er tom.
    while (!pThreadBData->pThreadData->fileProcessed || pThreadBData->pThreadData->iUsedBytes > 0) {
        // Låser mutex for å sikre at kun en tråd kan lese fra bufferen om gangen.
        pthread_mutex_lock(&pThreadBData->pThreadData->mutex);

        // Går gjennom bufferen og teller antall ganger hver byte forekommer.
        int i = 0;
        while (0 < pThreadBData->pThreadData->iUsedBytes) {
            char c = pThreadBData->pThreadData->szDataBuffer[pThreadBData->pThreadData->iUsedBytes - 1];
            pThreadBData->bytesProcessed[(unsigned char)c]++;
            pThreadBData->pThreadData->iUsedBytes--;
            i++;
        }

        // Låser opp mutex.
        pthread_mutex_unlock(&pThreadBData->pThreadData->mutex);

        // Venter i 1 millisekund for å gi tråd A tid til å skrive til bufferen.
        usleep(1000);
    }

    // Avslutter tråden.
    pthread_exit(NULL);
}