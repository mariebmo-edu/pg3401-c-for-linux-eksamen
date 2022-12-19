//
// Created by 1012
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/menu.h"
#include "include/reservation.h"
#include "include/dateTimeHandling.h"

// Head <-- next / prev --> Tail

// Går gjennom listen og deallokerer minnet til alle reservasjonene. "sletter" alt.
void clearAllocation(struct reservation **ppHead, struct reservation **ppTail){
    struct reservation *pTemp = *ppHead;
    while(pTemp != NULL){
        *ppHead = pTemp->pPrev;
        free(pTemp);
        pTemp = *ppHead;
    }
    *ppHead = NULL;
    *ppTail = NULL;
}

// Hvis det ikke er noen reservasjoner, bruker den init-reservasjon for å sette opp listen.
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

// Legger til en ny reservasjon i listen foran ppBeforeRes-reservasjonen.
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

// Legger til en ny reservasjon i listen etter ppAfterRes-reservasjonen.
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

// Fjerner en reservasjon fra listen og deallokerer minnet.
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

// Går gjennom alle reservasjoner og fjerner de der dato + dager er mindre enn dagens dato. "fullførte" opphold.
void deleteCompletedReservations(struct reservation **ppHead){

    struct reservation *pTemp = *ppHead;

    // Dagens dato som en time_t struct.
    time_t dateToday = time(NULL);


    while(pTemp != NULL){

        // Lager en ny struct tm med dato fra reservasjonen.
        struct tm date = getTmFromYYYYMMDD(pTemp->iDate);

        // Legger til antall dager fra reservasjonen.
        date.tm_mday += pTemp->iDays;

        // Lager en ny time_t med dato fra struct tm.
        time_t dateReservationFinished = mktime(&date);


        // Hvis dato fra reservasjonen er mindre enn dagens dato, sletter den reservasjonen.
        if(dateReservationFinished < dateToday){

            struct reservation *pPrev = pTemp->pPrev;
            deleteReservation(&pTemp);

            pTemp = pPrev;
        } else {
            pTemp = pTemp->pPrev;
        }
    }
}

// Går gjennom listen og finner reservasjonene med navn lik input-navn (pName).
void searchReservationByName(struct reservation **ppHead, char *pName){

    struct reservation *pTemp = *ppHead;
    int iFoundAtLeastOne = 0;

    while(pTemp != NULL){
        if(strcmp(pTemp->szName, pName) == 0){

            //Hvis den finner minst en, printer den ut en separator.
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

// Går gjennom listen og legger sammen dager*pris for alle reservasjoner.
void sumPriceOfReservations(struct reservation **ppHead){

    struct reservation *pTemp = *ppHead;
    float fSum = 0;

    while(pTemp != NULL){
        fSum += pTemp->fPricePerDay * pTemp->iDays;
        pTemp = pTemp->pPrev;
    }
    printf("Sum of all reservations: %f\n", fSum);
}

// Går gjennom listen og printer ut alle reservasjoner.
void printReservations(struct reservation **ppHead){

    struct reservation *pCurr = *ppHead;

    while(pCurr != NULL){
        printReservation(&pCurr);
        printSeparator(50, '-');
        pCurr = pCurr->pPrev;
    }
}

// Printer ut en reservasjon.
void printReservation(struct reservation **ppReservation){

    struct reservation *pCurr = *ppReservation;
    struct tm date = getTmFromYYYYMMDD(pCurr->iDate);

    printf("Name: %s\n", pCurr->szName);
    printf("Room number: %s\n", pCurr->szRoomNr);
    printf("Date: ");
    printDateFormatted(&date);
    printf("\n");
    printf("Days: %d\n", pCurr->iDays);
    printf("Price per day: %f\n", pCurr->fPricePerDay);
}

