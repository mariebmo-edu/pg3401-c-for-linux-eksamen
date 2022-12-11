//
// Created by marie on 09.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/menu.h"
#include "include/reservation.h"

// Head <-- next / prev --> Tail

void clearAllocation(struct reservation **ppHead, struct reservation **ppTail){
    struct reservation *pTemp = *ppHead;
    while(pTemp != NULL){
        *ppHead = pTemp->pNext;
        free(pTemp);
        pTemp = *ppHead;
    }
    *ppHead = NULL;
    *ppTail = NULL;
}

int initReservation(struct reservation **ppHead, struct reservation **ppTail, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay){

    struct reservation *pNew = malloc(sizeof(struct reservation));

    if(pNew == NULL){
        printf("Error: Could not allocate memory for new reservation.\n");
        return 0;
    }

    pNew->pNext = NULL;
    pNew->pPrev = NULL;
    pNew->iDate = iDate;
    pNew->iDays = iDays;
    pNew->fPricePerDay = fPricePerDay;
    strcpy(pNew->szName, pszName);
    strcpy(pNew->szRoomNr, pszRoomNr);

    *ppTail = pNew;
    *ppHead = pNew;

    return 1;
} // End initReservation

int insertBefore(struct reservation **ppBeforeRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay){



    struct reservation *pNew = malloc(sizeof(struct reservation));

    if(pNew == NULL){
        printf("Error: Could not allocate memory for new reservation.\n");
        return 0;
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

    return 1;
} // End insertBefore

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
} // End insertAfter

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
} // End deleteReservation

void deleteCompletedReservations(struct reservation **ppHead){

    struct reservation *pTemp = *ppHead;
    time_t dateToday = time(NULL);


    while(pTemp != NULL){

            struct tm date = getTmFromYYYYMMDD(pTemp->iDate);

            date.tm_mday += pTemp->iDays;

            time_t dateReservationFinished = mktime(&date);

            if(dateReservationFinished < dateToday){

                struct reservation *pPrev = pTemp->pPrev;
                deleteReservation(&pTemp);

                pTemp = pPrev;
            } else {
                pTemp = pTemp->pPrev;
            }
        }
}

void searchReservationByName(struct reservation **ppHead, char *pName){

    struct reservation *pTemp = *ppHead;
    int iFoundAtLeastOne = 0;

    while(pTemp != NULL){
        if(strcmp(pTemp->szName, pName) == 0){
            if(iFoundAtLeastOne == 1){
                printSeparator(50, '-');
            }
            printReservation(&pTemp);
            iFoundAtLeastOne = 1;
        }
        pTemp = pTemp->pPrev;
    }

    if(iFoundAtLeastOne == 0){
        printf("Error: No reservation found with name %s.\n", pName);
    }
} // End searchReservationByName

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
        printReservation(&pCurr);
        printSeparator(50, '-');
        pCurr = pCurr->pPrev;
    }
    printf("TAIL\n");
}

void printReservation(struct reservation **ppReservation){

    struct reservation *pCurr = *ppReservation;
    struct tm date = getTmFromYYYYMMDD(pCurr->iDate);

    printf("Name: %s\n", pCurr->szName);
    printf("Room number: %s\n", pCurr->szRoomNr);
    printf("Date: ");
    printDate(&date);
    printf("Days: %d\n", pCurr->iDays);
    printf("Price per day: %f\n", pCurr->fPricePerDay);
}

struct tm getTmFromYYYYMMDD(unsigned int iDate){
    struct tm date;
    memset(&date, 0, sizeof(struct tm));

    date.tm_year = iDate / 10000 - 1900;
    date.tm_mon = iDate % 10000 / 100 - 1;
    date.tm_mday = iDate % 100;

    return date;
}

void printDate(struct tm *pTmDate){

    char szFormattedDate[20];
    printf("%s\n", szFormattedDate);
}

