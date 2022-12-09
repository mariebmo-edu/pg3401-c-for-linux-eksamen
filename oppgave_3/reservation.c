//
// Created by marie on 09.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct reservation {
    struct reservation *pNext;
    struct reservation *pPrev;
    char szName[128];
    char szRoomNr[6];
    int iDate;
    int iDays;
    float fPricePerDay;
};

void initReservation(struct reservation **ppTail, struct reservation **ppHead){

    struct reservation *pNew = malloc(sizeof(struct reservation));

    if(pNew == NULL){
        printf("Error: Could not allocate memory for new reservation.\n");
        return;
    }

    pNew->pNext = NULL;
    pNew->pPrev = NULL;

    *ppTail = pNew;
    *ppHead = pNew;
}

void insertBefore(struct reservation **ppBeforeRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay){

    struct reservation *pNew = malloc(sizeof(struct reservation));

    if(pNew == NULL){
        printf("Error: Could not allocate memory for new reservation.\n");
        return;
    }

    pNew->pNext = NULL;
    pNew->pPrev = *ppBeforeRes;
    pNew->iDate = iDate;
    pNew->iDays = iDays;
    pNew->fPricePerDay = fPricePerDay;

    strcpy(pNew->szName, pszName);
    strcpy(pNew->szRoomNr, pszRoomNr);

    if((*ppBeforeRes)->pNext != NULL){
        (*ppBeforeRes)->pNext->pPrev = pNew;
    }

    (*ppBeforeRes)->pNext = pNew;
    *ppBeforeRes = pNew;
}

int insertAfter(struct reservation **ppAfterRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay){

    struct reservation *pNew = malloc(sizeof(struct reservation));

    if(pNew == NULL){
        printf("Error: Could not allocate memory for new reservation.\n");
        return 0;
    }

    pNew->pNext = *ppAfterRes;
    pNew->pPrev = NULL;
    pNew->iDate = iDate;
    pNew->iDays = iDays;
    pNew->fPricePerDay = fPricePerDay;

    strcpy(pNew->szName, pszName);
    strcpy(pNew->szRoomNr, pszRoomNr);

    if((*ppAfterRes)->pPrev != NULL){
        (*ppAfterRes)->pPrev->pNext = pNew;
    }

    (*ppAfterRes)->pPrev = pNew;
    *ppAfterRes = pNew;

    return 1;
}

void deleteReservation(struct reservation **ppReservation){

    struct reservation *pTemp = *ppReservation;

    if(pTemp == NULL){
        printf("Error: No reservations to delete.\n");
        return;
    }

    if(pTemp->pPrev == NULL){
        *ppReservation = pTemp->pNext;
        (*ppReservation)->pPrev = NULL;
    } else if(pTemp->pNext == NULL){
        *ppReservation = pTemp->pPrev;
        (*ppReservation)->pNext = NULL;
    } else {
        pTemp->pPrev->pNext = pTemp->pNext;
        pTemp->pNext->pPrev = pTemp->pPrev;
    }

    free(pTemp);
}

void deleteCompletedReservations(struct reservation **ppHead){

        struct reservation *pTemp = *ppHead;

        while(pTemp != NULL){
            if(pTemp->iDate + pTemp->iDays < 20221209){
                deleteReservation(&pTemp);
            }
            pTemp = pTemp->pPrev;
        }
}

void searchReservationByName(struct reservation **ppHead, char *pName){

    struct reservation *pTemp = *ppHead;

    while(pTemp != NULL){
        if(strcmp(pTemp->szName, pName) == 0){
            printf("Reservation found:\n");
            printf("Name: %s\n", pTemp->szName);
            printf("Room number: %s\n", pTemp->szRoomNr);
            printf("Date: %d\n", pTemp->iDate);
            printf("Days: %d\n", pTemp->iDays);
            printf("Price per day: %f\n", pTemp->fPricePerDay);
            return;
        }
        pTemp = pTemp->pPrev;
    }
    printf("Error: No reservation found with name %s.\n", pName);
}

void sumPriceOfReservations(struct reservation **ppHead){

    struct reservation *pTemp = *ppHead;
    float fSum = 0;

    while(pTemp != NULL){
        fSum += pTemp->fPricePerDay * pTemp->iDays;
        pTemp = pTemp->pPrev;
    }
    printf("Sum of all reservations: %f\n", fSum);
}

void printReservations(struct reservation **ppHead){

    struct reservation *pCurr = *ppHead;

    printf("HEAD\n");

    while(pCurr != NULL){
        printf("Name: %s\n", pCurr->szName);
        printf("Room number: %s\n", pCurr->szRoomNr);
        printf("Date: %d\n", pCurr->iDate);
        printf("Days: %d\n", pCurr->iDays);
        printf("Price per day: %f\n", pCurr->fPricePerDay);
        pCurr = pCurr->pPrev;
    }
    printf("TAIL\n");
}

void printReservation(struct reservation **ppReservation){

    struct reservation *pCurr = *ppReservation;

    printf("Name: %s\n", pCurr->szName);
    printf("Room number: %s\n", pCurr->szRoomNr);
    printf("Date: %d\n", pCurr->iDate);
    printf("Days: %d\n", pCurr->iDays);
    printf("Price per day: %f\n", pCurr->fPricePerDay);
}

